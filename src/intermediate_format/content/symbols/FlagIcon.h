/**
 * intermediate_format/content/symbols/FlagIcon.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 7df1d62449b6ca68f26aa03718f73043e609bf2e3b7334978d2149cdb625b19c
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

#include <QDataStream>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct FlagIcon : RawMessageContentItem {
public:
    QString countryCode;
    QString textEquivalent;

    FlagIcon(IMM(QString) country_code, IMM(QString) text_equivalent);

    virtual RawMessageContentItemSubType subType() const;

    static CEDE(FlagIcon) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H
