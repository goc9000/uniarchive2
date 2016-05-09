#include <vector>

#include <QDebug>
#include <QDirIterator>
#include <QString>

#include "extraction/adium/extract_adium_conversations.h"
#include "extraction/digsby/extract_digsby_conversations.h"
#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "extraction/yahoo/extract_yahoo_messenger_dat_conversations.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::extraction::adium;
using namespace uniarchive2::extraction::digsby;
using namespace uniarchive2::extraction::facebook;
using namespace uniarchive2::extraction::msn;
using namespace uniarchive2::extraction::yahoo;

int main() {
    // Temporary test harness

    vector<IntermediateFormatConversation> convos;

    QDirIterator adium_files(QT_STRINGIFY(TEST_DATA_DIR) "/adium", QStringList() << "*).xml", QDir::Files, QDirIterator::Subdirectories);
    while (adium_files.hasNext()) {
        convos.push_back(extract_adium_conversation(adium_files.next()));
    }
    QDirIterator digsby_files(QT_STRINGIFY(TEST_DATA_DIR) "/digsby", QStringList() << "*.html", QDir::Files, QDirIterator::Subdirectories);
    while (digsby_files.hasNext()) {
        convos.push_back(extract_digsby_conversation(digsby_files.next()));
    }

    QDirIterator fb_files(QT_STRINGIFY(TEST_DATA_DIR) "/facebook", QStringList() << "messages.htm", QDir::Files, QDirIterator::Subdirectories);
    while (fb_files.hasNext()) {
        auto file_convos = extract_facebook_dyi_conversations(fb_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }
    QDirIterator yahoo_files(QT_STRINGIFY(TEST_DATA_DIR) "/yahoo", QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (yahoo_files.hasNext()) {
        auto file_convos = extract_yahoo_messenger_dat_conversations(yahoo_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    QDirIterator msn_files(QT_STRINGIFY(TEST_DATA_DIR) "/msn", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (msn_files.hasNext()) {
        auto file_convos = extract_msn_messenger_xml_conversations(msn_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    int limit = 50;
    for (IMM(auto) convo : convos) {
        qDebug() << convo;
        if (!--limit) {
            break;
        }
    }

    return 0;
}
