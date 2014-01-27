#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>

#include "core/args.h"
#include "utils/fail.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    try {
        Args args(argc, argv);

        //QCoreApplication a(argc, argv);
        //return a.exec();
    } catch (const std::exception& e) {
        printException(e);
    }
}
