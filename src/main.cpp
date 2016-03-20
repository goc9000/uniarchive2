#include <QDebug>
#include <QDirIterator>


int main() {
    // Temporary test harness

    QDirIterator yahoo_files(
            QT_STRINGIFY(TEST_DATA_DIR) "/yahoo",
            QStringList() << "*.dat",
            QDir::Files,
            QDirIterator::Subdirectories
    );

    while (yahoo_files.hasNext()) {
        qDebug() << yahoo_files.next();
    }

    return 0;
}
