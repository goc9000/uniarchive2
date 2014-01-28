#include "FileSystemScanner.h"

#include "input/file_scanners/RegularRawConversationFile.h"
#include "utils/fail.h"
#include "utils/utils.h"


FileSystemScanner::FileSystemScanner(QString path)
{
    path_ = path;
}

std::vector<RawConversationFile*> FileSystemScanner::files()
{
    QFileInfo item(path_);
    if (!item.exists()) {
        fail("No file or directory exists at '%s'", qPrintable(path_));
    }

    return _scanItem(item);
}

std::vector<RawConversationFile*> FileSystemScanner::_scanItem(
    const QFileInfo& item)
{
    if (item.isDir()) {
        return _scanDir(QDir(item.filePath()));
    }

    if (!item.isReadable()) {
        fail("Cannot open file '%s' for reading", qPrintable(item.filePath()));
    }

    // TODO: check if it is an archive, instantiante ArchiveScanner for it

    return std::vector<RawConversationFile*>{
        new RegularRawConversationFile(item.filePath())
    };
}

std::vector<RawConversationFile*> FileSystemScanner::_scanDir(const QDir& dir)
{
    if (!dir.isReadable()) {
        fail("Cannot open directory '%s' for reading", qPrintable(dir.path()));
    }

    std::vector<RawConversationFile*> results;

    for (const QFileInfo& subItem : dir.entryInfoList(
             QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden)) {
        appendByMoving(results, _scanItem(subItem));
    }

    return results;
}
