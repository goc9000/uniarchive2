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
#include "extraction/adium/extract_adium_conversations.h"
#include "extraction/digsby/extract_digsby_conversations.h"
#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "extraction/pidgin/extract_pidgin_html_conversations.h"
#include "extraction/pidgin/extract_pidgin_txt_conversations.h"
#include "extraction/skype/extract_skype_conversations.h"
#include "extraction/whatsapp/extract_whatsapp_email_conversations.h"
#include "extraction/yahoo/extract_yahoo_messenger_dat_conversations.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "utils/qt/shortcuts.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QDebug>
#include <QDirIterator>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

#include <map>
#include <vector>

namespace uniarchive2 {

using namespace std;
using namespace uniarchive2::extraction::adium;
using namespace uniarchive2::extraction::digsby;
using namespace uniarchive2::extraction::facebook;
using namespace uniarchive2::extraction::msn;
using namespace uniarchive2::extraction::pidgin;
using namespace uniarchive2::extraction::skype;
using namespace uniarchive2::extraction::whatsapp;
using namespace uniarchive2::extraction::yahoo;
using namespace uniarchive2::intermediate_format::subjects;

QString remove_trailing_slash(IMM(QString) path);
vector<RawConversation> extract_conversations(IMM(QString) base_input_path);
void dump_conversations(IMM(vector<RawConversation>) conversations, IMM(QString) base_output_path);


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

vector<RawConversation> extract_conversations(IMM(QString) base_input_path) {
    vector<RawConversation> convos;

    qDebug() << "Skype";
    QDirIterator skype_files(base_input_path + "/skype", QStringList() << "main.db", QDir::Files, QDirIterator::Subdirectories);
    while (skype_files.hasNext()) {
        auto file_convos = extract_skype_conversations(skype_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "WhatsApp";
    QDirIterator whatsapp_files(base_input_path + "/whatsapp", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (whatsapp_files.hasNext()) {
        convos.push_back(extract_whatsapp_email_conversation(whatsapp_files.next()));
    }
    qDebug() << "Pidgin TXT";
    QDirIterator pidgin_txt_files(base_input_path + "/pidgin", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (pidgin_txt_files.hasNext()) {
        convos.push_back(extract_pidgin_txt_conversation(pidgin_txt_files.next()));
    }
    qDebug() << "Pidgin HTML";
    QDirIterator pidgin_html_files(base_input_path + "/pidgin", QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (pidgin_html_files.hasNext()) {
        convos.push_back(extract_pidgin_html_conversation(pidgin_html_files.next()));
    }
    qDebug() << "Adium";
    QDirIterator adium_files(base_input_path + "/adium", QStringList() << "*).xml", QDir::Files, QDirIterator::Subdirectories);
    while (adium_files.hasNext()) {
        convos.push_back(extract_adium_conversation(adium_files.next()));
    }
    qDebug() << "Digsby";
    QDirIterator digsby_files(base_input_path + "/digsby", QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (digsby_files.hasNext()) {
        convos.push_back(extract_digsby_conversation(digsby_files.next()));
    }
    qDebug() << "Facebook";
    QDirIterator fb_files(base_input_path + "/facebook", QStringList() << "messages.htm", QDir::Files, QDirIterator::Subdirectories);
    while (fb_files.hasNext()) {
        auto file_convos = extract_facebook_dyi_conversations(fb_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "Yahoo";
    QDirIterator yahoo_files(base_input_path + "/yahoo", QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (yahoo_files.hasNext()) {
        auto file_convos = extract_yahoo_messenger_dat_conversations(yahoo_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "MSN";
    QDirIterator msn_files(base_input_path + "/msn", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (msn_files.hasNext()) {
        auto file_convos = extract_msn_messenger_xml_conversations(msn_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    return convos;
}

void dump_conversations(IMM(vector<RawConversation>) conversations, IMM(QString) base_output_path) {
    map<QString, unsigned int> filenames_used;

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
