#include "FileSystemRawConversationSource.h"

#include <QScopedPointer>

#include "input/file_scanners/FileSystemScanner.h"
#include "decoders/RawConversationDecoder.h"
#include "utils/fail.h"


FileSystemRawConversationSource::FileSystemRawConversationSource(QString path)
{
    path_ = path;
}

QList<RawConversation> FileSystemRawConversationSource::rawConversations()
{
    QList<RawConversation> results;

    QScopedPointer<FileSystemScanner> scanner(new FileSystemScanner(path_));

    bool anyFound = false;
    for (RawConversationFile *file : scanner->files()) {
        if (!RawConversationDecoder::isSupportedFormat(file)) {
            warn("File '%s' is not a conversation file in any supported format",
                 qPrintable(file->description()));
            continue;
        } else {
            anyFound = true;

            QScopedPointer<RawConversationDecoder> decoder(
                RawConversationDecoder::forRawConversationFile(file));

            results.append(decoder->rawConversations());
        }

        delete file;
    }

    if (!anyFound) {
        warn("No supported conversation files found under '%s'",
             qPrintable(path_));
    }

    return results;
}
