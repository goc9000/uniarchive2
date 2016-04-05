#include <QDebug>
#include <QDirIterator>
#include <QRegularExpression>
#include <QString>
#include <QVector>

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

    QVector<IntermediateFormatConversation> convos;

    while (yahoo_files.hasNext()) {
        QString filename = yahoo_files.next();
        convos += extract_yahoo_messenger_conversations(filename);
    }

    int limit = 10;
    for (auto& convo : convos) {
        qDebug() << convo;
        if (!--limit) {
            break;
        }
    }

    return 0;
}
