/**
 * temp_test_harness.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "temp_test_harness.h"
#include "extraction/extract_conversations_generic.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "intermediate_format/RawConversationCollection.h"
#include "utils/qt/shortcuts.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QDebug>
#include <QDirIterator>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>
#include <QRunnable>


#include <map>
#include <vector>

namespace uniarchive2 {

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::intermediate_format::subjects;

QString remove_trailing_slash(IMM(QString) path);
RawConversationCollection extract_conversations(IMM(QString) base_input_path);
void dump_conversations(IMM(RawConversationCollection) conversations, IMM(QString) base_output_path);


void run_test_harness(IMM(QString) config_file) {
    QFile file(config_file);
    if (!file.open(QIODevice::ReadOnly)) {
        invariant_violation("Config file not found: %s", QP(config_file));
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    invariant(!doc.isNull() && doc.isObject(), "Malformed config file");

    QString base_input_path = remove_trailing_slash(doc.object()["base_input_path"].toString());
    QString base_output_path = remove_trailing_slash(doc.object()["base_output_path"].toString());

    invariant(QDir(base_input_path).exists(), "Test input dir %s does not exist", QP(base_input_path));
    invariant(QDir(base_output_path).exists(), "Test output dir %s does not exist", QP(base_output_path));

    auto convos = extract_conversations(base_input_path);
    dump_conversations(convos, base_output_path);
}

QString remove_trailing_slash(IMM(QString) path) {
    return path.endsWith("/") ? path.left(path.length() - 1) : path;
}

class ExtractConversationsTask : public QRunnable {
public:
    ArchiveFormat format;
    QString filename;

    RawConversationCollection result;

    ExtractConversationsTask(ArchiveFormat format, IMM(QString) filename) : format(format), filename(filename) {
        setAutoDelete(false);
    }

    void run() {
        result.take(extract_conversations_generic(format, filename));
    }
};

RawConversationCollection extract_conversations(IMM(QString) base_input_path) {
    RawConversationCollection convos;

    QThreadPool* tp = QThreadPool::globalInstance();
    tp->setMaxThreadCount(QThread::idealThreadCount() * 2); // oversubscribe 2x

    vector<tuple<ArchiveFormat, QString, QString>> inputs {
        { ArchiveFormat::SKYPE,               "skype",    "main.db" },
        { ArchiveFormat::WHATSAPP_EMAIL,      "whatsapp", "*.txt" },
        { ArchiveFormat::PIDGIN_TXT,          "pidgin",   "*.txt" },
        { ArchiveFormat::PIDGIN_HTML,         "pidgin",   "*.html" },
        { ArchiveFormat::ADIUM,               "adium",    "*).xml" },
        { ArchiveFormat::DIGSBY,              "digsby",   "*.html" },
        { ArchiveFormat::FACEBOOK_DYI,        "facebook", "messages.htm" },
        { ArchiveFormat::YAHOO_MESSENGER_DAT, "yahoo",    "*.dat" },
        { ArchiveFormat::MSN_MESSENGER_XML,   "msn",      "*xml" },
    };

    for (IMM(auto) format_path_glob : inputs) {
        qDebug() << get<ArchiveFormat>(format_path_glob);
        QDirIterator files(
            base_input_path + "/" + get<1>(format_path_glob),
            QStringList() << get<2>(format_path_glob),
            QDir::Files,
            QDirIterator::Subdirectories
        );

        vector<unique_ptr<ExtractConversationsTask>> tasks;

        while (files.hasNext()) {
            tasks.push_back(make_unique<ExtractConversationsTask>(
                get<ArchiveFormat>(format_path_glob),
                files.next()
            ));
        }

        for (IMM(auto) task : tasks) {
            tp->start(task.get());
        }

        tp->waitForDone();

        for (IMM(auto) task : tasks) {
            convos.take(move(task->result));
        }
    }

    return convos;
}

void dump_conversations(IMM(RawConversationCollection) conversations, IMM(QString) base_output_path) {
    map<QString, uint> filenames_used;

    for (IMM(auto) convo : conversations) {
        QStringList convo_path;
        convo_path << name_for_im_protocol(convo.protocol);

        if (
            (convo.isConference && *convo.isConference) ||
            (!convo.isConference && convo.declaredPeers.size() > 1)
            ) {
            convo_path << "Conferences";
        }
        if (convo.declaredPeers.empty()) {
            convo_path << "(Unknown)";
        } else {
            ApparentSubject* subject = convo.declaredPeers.front().get();
            if (subject->is<FullySpecifiedSubject>()) {
                convo_path << subject->as<FullySpecifiedSubject>()->accountName.accountName;
            } else if (subject->is<SubjectGivenAsAccount>()) {
                convo_path << subject->as<SubjectGivenAsAccount>()->account.accountName;
            } else if (subject->is<SubjectGivenAsScreenName>()) {
                convo_path << subject->as<SubjectGivenAsScreenName>()->screenName;
            } else {
                convo_path << "(Unknown)";
            }
        }

        QDir(base_output_path).mkpath(convo_path.join(QDir::separator()));

        QString filename;
        QDebug ss(&filename);
        ss.nospace();
        if (convo.declaredStartDate) {
            ss << " " << *convo.declaredStartDate;
        } else if (!convo.events.empty()) {
            ss << " " << convo.events.front()->timestamp;
        } else {
            ss << " (Unknown date)";
        }

        QString base_filename = base_output_path + QDir::separator() + convo_path.join(QDir::separator()) +
            QDir::separator() + filename.trimmed() + ".txt";

        filenames_used[base_filename]++;
        if (filenames_used[base_filename] > 1) {
            ss << " (" << filenames_used[base_filename] << ")";
        }

        QString full_filename = base_output_path + QDir::separator() + convo_path.join(QDir::separator()) +
            QDir::separator() + filename.trimmed() + ".txt";

        QFile f(full_filename);
        f.open(QFile::WriteOnly);
        invariant(f.isWritable(), "Could not open output file '%s'", QP(full_filename));

        QDebug writer(&f);
        convo.writeToDebugStream(writer, true);
    }
}

}
