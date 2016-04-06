/**
 * utils/text/decoding.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QTextCodec>

#include "utils/language/invariant.h"
#include "utils/text/decoding.h"

namespace uniarchive2 { namespace utils { namespace text {

QString decode_utf8(const QByteArray& utf8_encoded_text) {
    QTextCodec::ConverterState state;
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");

    QString text = codec->toUnicode(utf8_encoded_text.constData(), utf8_encoded_text.size(), &state);

    invariant(!state.invalidChars, "Invalid UTF-8 string: \"%s\"", qPrintable(utf8_encoded_text));

    return text;
}

}}}
