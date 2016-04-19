#include <vector>

#include <QDebug>
#include <QDirIterator>
#include <QString>

#include "extraction/yahoo/extract_yahoo_messenger_conversations.h"

using namespace std;
using namespace uniarchive2::extraction::yahoo;

int main() {
    // Temporary test harness

    QDirIterator yahoo_files(
        QT_STRINGIFY(TEST_DATA_DIR) "/yahoo",
        QStringList() << "*.dat",
        QDir::Files,
        QDirIterator::Subdirectories
    );

    vector<IntermediateFormatConversation> convos;

    while (yahoo_files.hasNext()) {
        QString filename = yahoo_files.next();
        auto file_convos = extract_yahoo_messenger_conversations(filename);
        move(file_convos.begin(), file_convos.end(), back_inserter(convos));
    }

    int limit = 50;
    for (const auto& convo : convos) {
        if (!convo.events.isEmpty()) {
            qDebug() << convo;
            if (!--limit) {
                break;
            }
        }
    }

    return 0;
}
