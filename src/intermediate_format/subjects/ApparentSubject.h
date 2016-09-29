/**
 * intermediate_format/subjects/ApparentSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H

#include "intermediate_format/subjects/ApparentSubjectSubType.h"
#include "utils/language/polymorphic_helpers.h"
#include "utils/language/shortcuts.h"
#include "utils/polymorphics/StandardPolymorphic.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace uniarchive2::utils::polymorphics;

class ApparentSubject : public StandardPolymorphic<ApparentSubjectSubType> {
public:
    enum class Hint {
        NoHints    = 0,
        IsIdentity = (1 << 0),
        IsPeer     = (1 << 1),
    };
    Q_DECLARE_FLAGS(Hints, Hint);

    Hints hints;

    ApparentSubject(Hints hints = Hint::NoHints);

    POLYMORPHIC_HELPERS

    virtual CEDE(ApparentSubject) clone() const = 0;

    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void serializeToStreamSubImpl(QDataStream& mut_stream) const = 0;

    virtual void writeToDebugStreamImpl(QDebug stream) const = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ApparentSubject::Hints);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
