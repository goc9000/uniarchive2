#ifndef REGULARRAWCONVERSATIONFILE_H
#define REGULARRAWCONVERSATIONFILE_H

#include "input/file_scanners/RawConversationFile.h"


class RegularRawConversationFile : public RawConversationFile
{
public:
    RegularRawConversationFile(QString filename);

    static QString getFullPathInfo(QString filename);
};

#endif // REGULARRAWCONVERSATIONFILE_H
