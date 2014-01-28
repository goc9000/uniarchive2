#ifndef ARGS_H
#define ARGS_H

#include <vector>

#include <QString>

class Args
{
public:
    static const QString DEFAULT_OUTPUT_FILENAME;

    Args();
    Args(int argc, char* argv[]);

    void sanityCheck();

    bool produceArchive;
    bool produceEpub;
    QString outArchiveFilename;
    QString outEpubFilename;
    std::vector<QString> sources;
};

#endif // ARGS_H
