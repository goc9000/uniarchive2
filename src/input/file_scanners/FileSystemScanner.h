#ifndef FILESYSTEMSCANNER_H
#define FILESYSTEMSCANNER_H

#include <vector>

#include <QString>
#include <QFileInfo>
#include <QDir>

#include "input/file_scanners/RawConversationFile.h"


class FileSystemScanner
{
public:
    FileSystemScanner(QString path);

    std::vector<RawConversationFile*> files();

protected:
    std::vector<RawConversationFile*> _scanItem(const QFileInfo& item);
    std::vector<RawConversationFile*> _scanDir(const QDir& item);

    QString path_;
};

#endif // FILESYSTEMSCANNER_H
