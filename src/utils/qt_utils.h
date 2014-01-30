#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <memory>

#include <QIODevice>
#include <QFile>
#include <QTextStream>

typedef std::unique_ptr<QIODevice> QIODeviceUqPtr;
typedef std::unique_ptr<QFile> QFileUqPtr;
typedef std::unique_ptr<QTextStream> QTextStreamUqPtr;

class QOwningTextStream : public QTextStream
{
public:
    QOwningTextStream(QIODevice *device)
        : QTextStream(device), _device(device) { }

protected:
    QIODeviceUqPtr _device;
};

#endif // QT_UTILS_H
