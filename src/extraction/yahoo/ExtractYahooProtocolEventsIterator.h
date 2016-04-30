/**
 * extraction/yahoo/ExtractYahooProtocolEventsIterator.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACTYAHOOPROTOCOLEVENTSITERATOR_H
#define UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACTYAHOOPROTOCOLEVENTSITERATOR_H

#include <QString>
#include <QByteArray>

#include <QDataStream>

#include "extraction/yahoo/YahooProtocolEvent.h"
#include "utils/language/JavaStyleIterator.h"
#include "utils/language/shortcuts.h"

using namespace uniarchive2::utils::language;

namespace uniarchive2 { namespace extraction { namespace yahoo {

class ExtractYahooProtocolEventsIterator : JavaStyleIterator<YahooProtocolEvent> {
public:
    ExtractYahooProtocolEventsIterator(IMM(QByteArray) binary_data, IMM(QString) local_account_name);
    bool hasNext() const;
    YahooProtocolEvent next();

private:
    QDataStream reader;
    QByteArray local_account_name;

    quint32 readInt();
    QByteArray readRawString();
    void decryptRawString(QByteArray& string) const;
    void checkState() const;
};

}}}

#endif //UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACTYAHOOPROTOCOLEVENTSITERATOR_H
