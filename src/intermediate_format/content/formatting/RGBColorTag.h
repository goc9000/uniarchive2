/**
 * intermediate_format/content/formatting/RGBColorTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e6dd369cfbc30bd4b3ebed922921021aa10efe4c26a997174605fc50fd5efd1b
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H

#include "graphics/Color.h"
#include "intermediate_format/content/SymmetricTag.h"
#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

struct RGBColorTag : SymmetricTag {
public:
    Color color;

    RGBColorTag(bool open, IMM(Color) color);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(RGBColorTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H
