/**
 * intermediate_format/content/MediaAttachment.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H

#include "intermediate_format/errors/MediaError.h"
#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;
using namespace uniarchive2::intermediate_format::errors;

struct MediaAttachment : RawMessageContentItem {
public:
    optional<QString> filename;
    optional<MediaError> error;

    MediaAttachment();

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H
