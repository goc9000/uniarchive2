#include "FileSystemRawConversationSource.h"

#include <memory>

#include "input/file_scanners/FileSystemScanner.h"
#include "decoders/RawConversationDecoder.h"
#include "utils/fail.h"
#include "utils/utils.h"


FileSystemRawConversationSource::FileSystemRawConversationSource(QString path)
{
    path_ = path;
}

std::vector<RawConversation> FileSystemRawConversationSource::rawConversations()
{
    std::vector<RawConversation> results;

    FileSystemScanner scanner(path_);

    bool anyFound = false;
    for (RawConversationFileUqPtr& file : scanner.files()) {
        if (!RawConversationDecoder::isSupportedFormat(file.get())) {
            //warn("File '%s' is not a conversation file in any supported format",
            //     qPrintable(file->description()));
        } else {
            anyFound = true;

            ErrorContext errContext("in file '%s'",
                                    qPrintable(file->description()));

            RawConversationDecoderUqPtr decoder =
                RawConversationDecoder::forRawConversationFile(file.get());

            appendByMoving(results, decoder->rawConversations());
        }
    }

    if (!anyFound) {
        warn("No supported conversation files found under '%s'",
             qPrintable(path_));
    }

    return results;
}
