/**
 * utils/text/decoding.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_TEXT_DECODING_H
#define UNIARCHIVE2_UTILS_TEXT_DECODING_H

#include <QByteArray>
#include <QString>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace text {

QString decode_utf8(IMM(QByteArray) utf8_encoded_text);

}}}

#endif //UNIARCHIVE2_UTILS_TEXT_DECODING_H
