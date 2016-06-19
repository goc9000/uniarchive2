#include <vector>

#include <QDebug>
#include <QDirIterator>
#include <QString>

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

#include <set>

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


int main(int argc, char* argv[]) {
    // Temporary test harness

    vector<RawConversation> convos;

    qDebug() << "Skype";
    QDirIterator skype_files(QT_STRINGIFY(TEST_DATA_DIR) "/skype", QStringList() << "main.db", QDir::Files, QDirIterator::Subdirectories);
    while (skype_files.hasNext()) {
        auto file_convos = extract_skype_conversations(skype_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "WhatsApp";
    QDirIterator whatsapp_files(QT_STRINGIFY(TEST_DATA_DIR) "/whatsapp", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (whatsapp_files.hasNext()) {
        convos.push_back(extract_whatsapp_email_conversation(whatsapp_files.next()));
    }
    qDebug() << "Pidgin TXT";
    QDirIterator pidgin_txt_files(QT_STRINGIFY(TEST_DATA_DIR) "/pidgin", QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    while (pidgin_txt_files.hasNext()) {
        convos.push_back(extract_pidgin_txt_conversation(pidgin_txt_files.next()));
    }
    qDebug() << "Pidgin HTML";
    QDirIterator pidgin_html_files(QT_STRINGIFY(TEST_DATA_DIR) "/pidgin", QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (pidgin_html_files.hasNext()) {
        convos.push_back(extract_pidgin_html_conversation(pidgin_html_files.next()));
    }
    qDebug() << "Adium";
    QDirIterator adium_files(QT_STRINGIFY(TEST_DATA_DIR) "/adium", QStringList() << "*).xml", QDir::Files, QDirIterator::Subdirectories);
    while (adium_files.hasNext()) {
        convos.push_back(extract_adium_conversation(adium_files.next()));
    }
    qDebug() << "Digsby";
    QDirIterator digsby_files(QT_STRINGIFY(TEST_DATA_DIR) "/digsby", QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (digsby_files.hasNext()) {
        convos.push_back(extract_digsby_conversation(digsby_files.next()));
    }
    qDebug() << "Facebook";
    QDirIterator fb_files(QT_STRINGIFY(TEST_DATA_DIR) "/facebook", QStringList() << "messages.htm", QDir::Files, QDirIterator::Subdirectories);
    while (fb_files.hasNext()) {
        auto file_convos = extract_facebook_dyi_conversations(fb_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "Yahoo";
    QDirIterator yahoo_files(QT_STRINGIFY(TEST_DATA_DIR) "/yahoo", QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (yahoo_files.hasNext()) {
        auto file_convos = extract_yahoo_messenger_dat_conversations(yahoo_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    qDebug() << "MSN";
    QDirIterator msn_files(QT_STRINGIFY(TEST_DATA_DIR) "/msn", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (msn_files.hasNext()) {
        auto file_convos = extract_msn_messenger_xml_conversations(msn_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    QString base_output_path = QT_STRINGIFY(TEST_OUTPUT_DIR);
    invariant(QDir(base_output_path).exists(), "Test dir %s does not exist", QP(base_output_path));

    set<QString> already;


    for (IMM(auto) convo : convos) {
        QStringList convo_path;
        convo_path << name_for_im_protocol(convo.protocol);

        if (
            (convo.isConference && *convo.isConference) ||
            (!convo.isConference && convo.declaredPeers.size() > 2)
            ) {
            convo_path << "Conferences";
        }
        if (convo.declaredPeers.empty()) {
            convo_path << "(Unknown)";
        } else {
            ApparentSubject* subject = convo.declaredPeers.front().get();
            auto fss = dynamic_cast<FullySpecifiedSubject*>(subject);
            auto sgaa = dynamic_cast<SubjectGivenAsAccount*>(subject);
            auto sgasn = dynamic_cast<SubjectGivenAsScreenName*>(subject);
            if (fss) {
                convo_path << fss->accountName.accountName;
            } else if (sgaa) {
                convo_path << sgaa->account.accountName;
            } else if (sgasn) {
                convo_path << sgasn->screenName;
            } else {
                convo_path << "(Unknown)";
            }
        }

        QDir(base_output_path).mkpath(convo_path.join(QDir::separator()));

        QString filename;
        QDebug ss(&filename);
        if (convo.declaredStartDate) {
            ss << *convo.declaredStartDate;
        } else if (!convo.events.empty()) {
            ss << convo.events.front()->timestamp;
        } else {
            ss << "(Unknown date)";
        }

        QString full_filename = base_output_path + QDir::separator() + convo_path.join(QDir::separator()) +
            QDir::separator() + filename.trimmed() + ".txt";

        invariant(!already.count(full_filename), "Duplicate convo filename '%s'", QP(full_filename));
        already.insert(full_filename);

        QFile f(full_filename);
        f.open(QFile::WriteOnly);
        invariant(f.isWritable(), "Could not open output file '%s'", QP(full_filename));

        QDebug writer(&f);
        convo.writeToDebugStream(writer, true);
    }

    return 0;
}
