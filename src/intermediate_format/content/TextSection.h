/**
 * intermediate_format/content/TextSection.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): cdf6a1af9077ddf128947db733c83c7be7a0e30eca1c9307ef6049d15d2f8845
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

#include <QDataStream>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct TextSection : RawMessageContentItem {
public:
    QString text;

    TextSection(IMM(QString) text);

    virtual RawMessageContentItemSubType subType() const;

    static CEDE(TextSection) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H
