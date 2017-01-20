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
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "intermediate_format/RawConversationCollection.h"
#include "utils/qt/shortcuts.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QDebug>
#include <QDirIterator>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QThreadPool>
#include <QRunnable>

#include <map>
#include <set>
#include <vector>

namespace uniarchive2 {

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::intermediate_format::subjects;


QString remove_trailing_slash(IMM(QString) path) {
    return path.endsWith("/") ? path.left(path.length() - 1) : path;
}

QString parse_maybe_new_directory(QJsonValue json_value) {
    invariant(!json_value.isNull(), "Missing value for directory");
    invariant(json_value.isString(), "Directory should be string");

    QString dir = remove_trailing_slash(json_value.toString());

    return dir;
}

QString parse_existing_directory(QJsonValue json_value) {
    QString dir = parse_maybe_new_directory(json_value);

    invariant(QDir(dir).exists(), "Directory '%s' does not exist", QP(dir));

    return dir;
}

set<ArchiveFormat> parse_formats_set(QJsonValue json_value) {
    set<ArchiveFormat> formats;

    if (json_value.isNull()) {
        return formats;
    }
    if (json_value.isArray()) {
        for (IMM(auto) item : json_value.toArray()) {
            invariant(item.isString(), "Format array value should be JSON string");
            formats.insert(archive_format_from_symbol(item.toString()));
        }
        return formats;
    }
    if (json_value.isObject()) {
        QJsonObject as_object = json_value.toObject();
        for (IMM(auto) key : as_object.keys()) {
            invariant(as_object[key].isBool(), "Format dictionary value should have bools as values");
            if (as_object[key].toBool()) {
                formats.insert(archive_format_from_symbol(key));
            }
        }
        return formats;
    }

    invariant_violation("Invalid format for formats set");

    return formats;
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

RawConversationCollection extract_conversations(
    IMM(QString) base_input_path,
    set<ArchiveFormat> include_formats = set<ArchiveFormat>(),
    set<ArchiveFormat> exclude_formats = set<ArchiveFormat>()
) {
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
        ArchiveFormat format = get<ArchiveFormat>(format_path_glob);

        if (!include_formats.empty() && (include_formats.find(format) == include_formats.end())) {
            continue;
        }
        if (exclude_formats.find(format) != exclude_formats.end()) {
            continue;
        }

        qDebug() << "    - Extracting:" << format;
        QDirIterator files(
            base_input_path + "/" + get<1>(format_path_glob),
            QStringList() << get<2>(format_path_glob),
            QDir::Files,
            QDirIterator::Subdirectories
        );

        vector<unique_ptr<ExtractConversationsTask>> tasks;

        while (files.hasNext()) {
            tasks.push_back(make_unique<ExtractConversationsTask>(format, files.next()));
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

void dump_conversations(IMM(RawConversationCollection) conversations, IMM(QString) output_dir) {
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
            } else if (subject->is<AccountSubject>()) {
                convo_path << subject->as<AccountSubject>()->account.accountName;
            } else if (subject->is<ScreenNameSubject>()) {
                convo_path << subject->as<ScreenNameSubject>()->screenName;
            } else {
                convo_path << "(Unknown)";
            }
        }

        QDir(output_dir).mkpath(convo_path.join(QDir::separator()));

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

        QString base_filename = output_dir + QDir::separator() + convo_path.join(QDir::separator()) +
            QDir::separator() + filename.trimmed() + ".txt";

        filenames_used[base_filename]++;
        if (filenames_used[base_filename] > 1) {
            ss << " (" << filenames_used[base_filename] << ")";
        }

        QString full_filename = output_dir + QDir::separator() + convo_path.join(QDir::separator()) +
            QDir::separator() + filename.trimmed() + ".txt";

        QFile f(full_filename);
        f.open(QFile::WriteOnly);
        invariant(f.isWritable(), "Could not open output file '%s'", QP(full_filename));

        QDebug writer(&f);
        convo.writeToDebugStream(writer, true);
    }
}

RawConversationCollection run_extract_conversations_command(IMM(QJsonObject) command_obj) {
    QString base_path = parse_existing_directory(command_obj["base_path"]);

    qDebug() << "Extracting conversations at:" << QP(base_path);

    return extract_conversations(
        base_path,
        parse_formats_set(command_obj["include_formats"]),
        parse_formats_set(command_obj["exclude_formats"])
    );
}

void run_dump_conversations_command(IMM(QJsonObject) command_obj, IMM(RawConversationCollection) convos) {
    QString output_dir = parse_maybe_new_directory(command_obj["output_dir"]);

    qDebug() << "Dumping conversations to:" << QP(output_dir);

    dump_conversations(convos, output_dir);
}

void run_commands(QJsonValue commands_json) {
    invariant(!commands_json.isNull(), "Missing 'commands' key in config");
    invariant(commands_json.isArray(), "Commands should be an array");

    RawConversationCollection convos;

    for (IMM(auto) command_json : commands_json.toArray()) {
        invariant(command_json.isObject(), "Command JSON should be object");
        QJsonObject command_obj = command_json.toObject();

        if (command_obj.contains("skip")) {
            invariant(command_obj["skip"].isBool(), "'skip' key should be boolean");
            if (command_obj["skip"].toBool()) {
                continue;
            }
        }

        invariant(command_obj["command"].isString(), "Missing valid 'command' key");
        QString command = command_obj["command"].toString();

        if (command == "extract_conversations") {
            convos = run_extract_conversations_command(command_obj);
        } else if (command == "dump_conversations") {
            run_dump_conversations_command(command_obj, convos);
        } else {
            invariant_violation("Unsupported command: '%s'", QP(command));
        }
    }
}

void run_test_harness(IMM(QString) config_file) {
    QFile file(config_file);
    if (!file.open(QIODevice::ReadOnly)) {
        invariant_violation("Config file not found: %s", QP(config_file));
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    invariant(!doc.isNull() && doc.isObject(), "Malformed config file");
    QJsonObject doc_object = doc.object();

    run_commands(doc_object["commands"]);
}

}
