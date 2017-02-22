/**
 * intermediate_format/URLTargetTypeHint.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): d971d0308cb2b06c378339e25102d3baae5e78675a70f6e86755b5b8d4ab2c01
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_URLTARGETTYPEHINT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_URLTARGETTYPEHINT_H

#include "utils/language/shortcuts.h"

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format {

enum class URLTargetTypeHint : uint8_t {
    VIDEO,
    TWEET,
};

QString name_for_url_target_type_hint(URLTargetTypeHint hint);

QString symbol_for_url_target_type_hint(URLTargetTypeHint hint);
URLTargetTypeHint url_target_type_hint_from_symbol(IMM(QString) symbol);

QDebug operator<< (QDebug stream, URLTargetTypeHint hint);

QDataStream& operator>> (QDataStream& mut_stream, URLTargetTypeHint& mut_hint);
QDataStream& operator<< (QDataStream& mut_stream, URLTargetTypeHint hint);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_URLTARGETTYPEHINT_H