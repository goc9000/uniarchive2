/**
 * intermediate_format/content/SymmetricTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SYMMETRICTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SYMMETRICTAG_H

#include "intermediate_format/content/AbstractTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct SymmetricTag : AbstractTag {
public:
    bool open;

    SymmetricTag(bool open);

protected:
    virtual bool isClosed() const;

    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void serializeAttributesToStream(QDataStream& mut_stream) const = 0;

    virtual void writeAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SYMMETRICTAG_H
