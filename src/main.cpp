#include <QDebug>
#include <QDirIterator>

#include "extraction/yahoo/ExtractYahooProtocolEventsIterator.h"

using namespace uniarchive2::extraction::yahoo;

int main() {
    // Temporary test harness

    QDirIterator yahoo_files(
        QT_STRINGIFY(TEST_DATA_DIR) "/yahoo",
        QStringList() << "*.dat",
        QDir::Files,
        QDirIterator::Subdirectories
    );

    while (yahoo_files.hasNext()) {
        QString filename = yahoo_files.next();
        qDebug() << filename;
        qDebug() << "===";

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qFatal("Can't open file: %s", qUtf8Printable(filename));
        }

        QByteArray data = file.readAll();
        QString account_name = filename.section("-", -1).section(".", -2, -2);

        ExtractYahooProtocolEventsIterator proto_events(data, account_name);

        while (proto_events.hasNext()) {
            auto proto_event = proto_events.next();
            qDebug() << proto_event;
        }

        qDebug() << "---";
    }

    return 0;
}
