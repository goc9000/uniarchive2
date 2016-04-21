#include <vector>

#include <QDebug>
#include <QDirIterator>
#include <QString>

#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "extraction/yahoo/extract_yahoo_messenger_conversations.h"

using namespace std;
using namespace uniarchive2::extraction::msn;
using namespace uniarchive2::extraction::yahoo;

int main() {
    // Temporary test harness

    vector<IntermediateFormatConversation> convos;

    QDirIterator yahoo_files(QT_STRINGIFY(TEST_DATA_DIR) "/yahoo", QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (yahoo_files.hasNext()) {
        auto file_convos = extract_yahoo_messenger_conversations(yahoo_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    QDirIterator msn_files(QT_STRINGIFY(TEST_DATA_DIR) "/msn", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (msn_files.hasNext()) {
        auto file_convos = extract_msn_messenger_xml_conversations(msn_files.next());
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    int limit = 50;
    for (const auto& convo : convos) {
        qDebug() << convo;
        if (!--limit) {
            break;
        }
    }

    return 0;
}
