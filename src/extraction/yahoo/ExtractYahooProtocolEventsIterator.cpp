/**
 * extraction/yahoo/ExtractYahooProtocolEventsIterator.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/yahoo/ExtractYahooProtocolEventsIterator.h"
#include "protocols/yahoo/yahoo_account_name.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace extraction { namespace yahoo {

using namespace uniarchive2::protocols::yahoo;

ExtractYahooProtocolEventsIterator::ExtractYahooProtocolEventsIterator(
    IMM(QByteArray) binary_data,
    IMM(QString) local_account_name
) : reader(binary_data) {
    assert_valid_yahoo_account_name(local_account_name);
    this->local_account_name = local_account_name.toUtf8();
    reader.setByteOrder(QDataStream::LittleEndian);
}

bool ExtractYahooProtocolEventsIterator::hasNext() const {
    return !reader.atEnd();
}

YahooProtocolEvent ExtractYahooProtocolEventsIterator::next() {
    quint32 timestamp = readInt();
    YahooProtocolEvent::Type event_type = (YahooProtocolEvent::Type)readInt();
    YahooProtocolEvent::Direction direction = (YahooProtocolEvent::Direction)readInt();

    QByteArray text = readRawString();
    QByteArray extra = readRawString();

    decryptRawString(text);

    return YahooProtocolEvent(timestamp, event_type, direction, text, extra);
}

quint32 ExtractYahooProtocolEventsIterator::readInt() {
    quint32 value;
    reader >> value;
    checkState();

    return value;
}

QByteArray ExtractYahooProtocolEventsIterator::readRawString() {
    quint32 length = readInt();
    invariant(length <= reader.device()->bytesAvailable(), "Corrupt binary data in Yahoo Messenger archive");

    QByteArray raw_string = reader.device()->read(length);
    invariant(raw_string.length() == length, "Read failure while parsing Yahoo Messenger archive");

    return raw_string;
}

void ExtractYahooProtocolEventsIterator::decryptRawString(QByteArray& string) const {
    int key_pos = 0;
    int key_length = local_account_name.length();

    for (char& x : string) {
        x ^= local_account_name[key_pos++];
        if (key_pos == key_length) {
            key_pos = 0;
        }
    }
}

void ExtractYahooProtocolEventsIterator::checkState() const {
    invariant(reader.status() == QDataStream::Ok, "Corrupt binary data in Yahoo Messenger archive");
}

}}}
