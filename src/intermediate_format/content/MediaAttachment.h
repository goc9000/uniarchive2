/**
 * intermediate_format/content/MediaAttachment.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3f31cf5f7cc0599e842b31aaeac7991437e57d7e1e3a09a169dfe3d42b076742
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "intermediate_format/errors/MediaError.h"
#include "utils/external_libs/optional.hpp"

#include <QDataStream>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;
using namespace uniarchive2::intermediate_format::errors;

struct MediaAttachment : RawMessageContentItem {
public:
    optional<QString> filename;
    optional<MediaError> error;

    MediaAttachment();

    virtual RawMessageContentItemSubType subType() const;

    static CEDE(MediaAttachment) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H
