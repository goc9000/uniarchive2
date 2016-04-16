#include <QDebug>
#include <QDirIterator>
#include <QList>
#include <QRegularExpression>
#include <QString>

#include "extraction/yahoo/extract_yahoo_messenger_conversations.h"

using namespace uniarchive2::extraction::yahoo;

int main() {
    // Temporary test harness

    QDirIterator yahoo_files(
        QT_STRINGIFY(TEST_DATA_DIR) "/yahoo",
        QStringList() << "*.dat",
        QDir::Files,
        QDirIterator::Subdirectories
    );

    QList<IntermediateFormatConversation> convos;

    while (yahoo_files.hasNext()) {
        QString filename = yahoo_files.next();
        convos += extract_yahoo_messenger_conversations(filename);
    }

    int limit = 50;
    for (auto& convo : convos) {
        if (!convo.events.isEmpty()) {
            qDebug() << convo;
            if (!--limit) {
                break;
            }
        }
    }

    return 0;
}
