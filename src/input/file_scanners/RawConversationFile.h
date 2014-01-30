#ifndef RAWCONVERSATIONFILE_H
#define RAWCONVERSATIONFILE_H

#include <memory>

#include <QString>
#include <QIODevice>
#include <QByteArray>

#include "utils/qt_utils.h"


class RawConversationFile
{
public:
    RawConversationFile(QString filePath, QString fullPathInfo);
    virtual ~RawConversationFile();

    QString filePath() const;
    QString filename() const;
    QString basename() const;

    QString fullPathInfo() const;

    virtual QIODeviceUqPtr openBinary() const = 0;
    virtual QTextStreamUqPtr openUtf8();

    virtual QByteArray allData();
    virtual QString allUtf8Text();

    QString description() const;

protected:
    QString filePath_;
    QString fullPathInfo_;
};

typedef std::unique_ptr<RawConversationFile> RawConversationFileUqPtr;

#endif // RAWCONVERSATIONFILE_H
