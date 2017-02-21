/**
 * intermediate_format/RawSharedURLInfo.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWSHAREDURLINFO_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWSHAREDURLINFO_H

#include "intermediate_format/URLTargetTypeHint.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"
#include "utils/serialization/IDeserializableStatic.h"
#include "utils/serialization/ISerializable.h"

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format {

using namespace std::experimental;
using namespace uniarchive2::utils::serialization;

struct RawSharedURLInfo : public ISerializable, public IDeserializableStatic {
public:
    optional<QString> title;
    optional<QString> description;
    optional<QString> siteName;

    optional<QString> mimeType;

    optional<URLTargetTypeHint> typeHint;

    RawSharedURLInfo();

    static RawSharedURLInfo deserializeFromStream(QDataStream& mut_stream);
    virtual void serializeToStream(QDataStream& mut_stream) const;
};

QDebug operator<< (QDebug stream, IMM(RawSharedURLInfo) url_info);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWSHAREDURLINFO_H
