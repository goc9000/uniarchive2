#include <QCoreApplication>
#include <QTextCodec>
#include <QScopedPointer>
#include <QDebug>

#include "core/args.h"
#include "utils/fail.h"

#include "input/sources/RawConversationSource.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    try {
        Args args(argc, argv);

        for (const auto& sourceSpec : args.sources) {
            note("Processing source: %s", qPrintable(sourceSpec));

            QScopedPointer<RawConversationSource> source(
                RawConversationSource::fromSpecification(sourceSpec));
            for (const auto& rawConv : source->rawConversations()) {
            }
        }
        //QCoreApplication a(argc, argv);
        //return a.exec();
    } catch (const std::exception& e) {
        printException(e);
    }
}
