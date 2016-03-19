#include "args.h"

#include <algorithm>

#include <tclap/CmdLine.h>

#include "core/about.h"
#include "utils/fail.h"


#define NONE "none"

const QString Args::DEFAULT_OUTPUT_FILENAME = "unified.db";


Args::Args()
{
}

Args::Args(int argc, char* argv[])
{
    try {
        TCLAP::CmdLine cmdLine(
            About::APP_NAME + "\n" + About::APP_DESCRIPTION,
            ' ',
            About::APP_VERSION);

        TCLAP::ValueArg<std::string> outEpubArg(
            "e",
            "output-epub",
            "Specify a filename here to produce an EPUB e-book with all "
                "conversations",
            false,
            NONE,
            "archive.epub",
            cmdLine);

        TCLAP::ValueArg<std::string> outFileArg(
            "o",
            "output-file",
            "A filename for the unified archive produced. Use 'none' to "
                "produce no archive.",
            false,
            DEFAULT_OUTPUT_FILENAME.toStdString(),
            DEFAULT_OUTPUT_FILENAME.toStdString(),
            cmdLine);

        TCLAP::UnlabeledMultiArg<std::string> sourcesArg(
            "sources",
            "A list of archives, files, directories, URLs etc.",
            true,
            "sources",
            cmdLine);

        cmdLine.parse(argc, argv);

        produceArchive = (outFileArg.getValue() != NONE);
        outArchiveFilename =
            produceArchive ? QString::fromStdString(outFileArg.getValue()) : "";

        produceEpub = (outEpubArg.getValue() != NONE);
        outEpubFilename =
            produceEpub ? QString::fromStdString(outEpubArg.getValue()) : "";

        for (const std::string& rawValue : sourcesArg.getValue()) {
            sources.push_back(QString::fromStdString(rawValue));
        }

        sanityCheck();
    } catch (TCLAP::ArgException& e) {
        fail("Error processing argument %s: %s", e.argId().c_str(),
             e.error().c_str());
    }
}

void Args::sanityCheck()
{
    if (!produceArchive && !produceEpub) {
        fail("You must specify either an archive or an EPUB file to be created");
    }
}
