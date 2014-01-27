#include "FileSystemScanner.h"

#include "input/file_scanners/RegularRawConversationFile.h"
#include "utils/fail.h"


FileSystemScanner::FileSystemScanner(QString path)
{
    path_ = path;
}

QList<RawConversationFile*> FileSystemScanner::files()
{
    QFileInfo item(path_);
    if (!item.exists()) {
        fail("No file or directory exists at '%s'", qPrintable(path_));
    }

    return _scanItem(item);
}

QList<RawConversationFile*> FileSystemScanner::_scanItem(const QFileInfo& item)
{
    if (item.isDir()) {
        return _scanDir(QDir(item.filePath()));
    }

    if (!item.isReadable()) {
        fail("Cannot open file '%s' for reading", qPrintable(item.filePath()));
    }

    QList<RawConversationFile*> results;

    // TODO: check if it is an archive, instantiante ArchiveScanner for it

    results.append(new RegularRawConversationFile(item.filePath()));

    return results;
}

QList<RawConversationFile*> FileSystemScanner::_scanDir(const QDir& dir)
{
    if (!dir.isReadable()) {
        fail("Cannot open directory '%s' for reading", qPrintable(dir.path()));
    }

    QList<RawConversationFile*> results;

    for (const QFileInfo& subItem : dir.entryInfoList(
             QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden)) {
        results.append(_scanItem(subItem));
    }

    return results;
}
