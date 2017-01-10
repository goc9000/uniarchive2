/**
 * intermediate_format/subjects/ImplicitSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/ImplicitSubjectKind.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class ImplicitSubject : public ApparentSubject {
public:
    ImplicitSubjectKind kind;

    ImplicitSubject(ImplicitSubjectKind kind, Hints hints = Hint::NoHints);

    virtual ApparentSubjectSubType subType() const;

    virtual CEDE(ApparentSubject) clone() const;

    static CEDE(ImplicitSubject) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeToStreamSubImpl(QDataStream &mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H
