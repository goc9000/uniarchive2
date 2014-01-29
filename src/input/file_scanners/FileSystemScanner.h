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

    std::vector<RawConversationFileUqPtr> files();

protected:
    std::vector<RawConversationFileUqPtr> _scanItem(const QFileInfo& item);
    std::vector<RawConversationFileUqPtr> _scanDir(const QDir& item);

    QString path_;
};

#endif // FILESYSTEMSCANNER_H
