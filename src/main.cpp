#include <memory>

#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>

#include "core/args.h"
#include "utils/fail.h"

#include "input/sources/RawConversationSource.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    try {
        Args args(argc, argv);

        for (const QString& sourceSpec : args.sources) {
            note("Processing source: %s", qPrintable(sourceSpec));

            std::unique_ptr<RawConversationSource> source(
                RawConversationSource::fromSpecification(sourceSpec));

            for (const RawConversation& rawConv : source->rawConversations()) {
            }
        }
        //QCoreApplication a(argc, argv);
        //return a.exec();
    } catch (const std::exception& e) {
        printException(e);
    }
}
