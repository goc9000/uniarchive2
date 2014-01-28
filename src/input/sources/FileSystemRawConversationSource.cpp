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
    for (RawConversationFile *file : scanner.files()) {
        if (!RawConversationDecoder::isSupportedFormat(file)) {
            warn("File '%s' is not a conversation file in any supported format",
                 qPrintable(file->description()));
        } else {
            anyFound = true;

            std::unique_ptr<RawConversationDecoder> decoder(
                RawConversationDecoder::forRawConversationFile(file));

            appendByMoving(results, decoder->rawConversations());
        }

        delete file;
    }

    if (!anyFound) {
        warn("No supported conversation files found under '%s'",
             qPrintable(path_));
    }

    return results;
}
