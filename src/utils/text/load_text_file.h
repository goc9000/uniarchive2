/**
 * utils/text/load_text_file.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_TEXT_LOAD_TEXT_FILE_H
#define UNIARCHIVE2_UTILS_TEXT_LOAD_TEXT_FILE_H

#include <QByteArray>
#include <QString>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace text {

QString load_utf8_text_file(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_UTILS_TEXT_LOAD_TEXT_FILE_H
