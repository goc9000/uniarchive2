#include "PidginTextFormatDecoder.h"

#include <QRegExp>


static QRegExp PAT_PIDGIN_FILE(
    R"(^\d{4}-\d{2}-\d{2}\.\d{6}([+-]\d{4}\w*)?\.txt$)");


std::vector<RawConversation> PidginTextFormatDecoder::rawConversations()
{
    std::vector<RawConversation> result;

    return result;
}

bool PidginTextFormatDecoder::recognize(RawConversationFile *convFile)
{
    return PAT_PIDGIN_FILE.exactMatch(convFile->filename());
}
