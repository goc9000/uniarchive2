/**
 * utils/text/load_text_file.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QFile>

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/decoding.h"

namespace uniarchive2 { namespace utils { namespace text {

QString load_utf8_text_file(IMM(QString) filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    return decode_utf8(file.readAll());
}

}}}
