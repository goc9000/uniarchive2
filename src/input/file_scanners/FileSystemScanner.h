#ifndef FILESYSTEMSCANNER_H
#define FILESYSTEMSCANNER_H

#include <QList>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include "input/file_scanners/RawConversationFile.h"


class FileSystemScanner
{
public:
    FileSystemScanner(QString path);

    QList<RawConversationFile*> files();

protected:
    QList<RawConversationFile*> _scanItem(const QFileInfo& item);
    QList<RawConversationFile*> _scanDir(const QDir& item);

    QString path_;
};

#endif // FILESYSTEMSCANNER_H
