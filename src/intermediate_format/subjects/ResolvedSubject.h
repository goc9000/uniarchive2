/**
 * intermediate_format/subjects/ResolvedSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_RESOLVEDSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_RESOLVEDSUBJECT_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class ResolvedSubject : public ApparentSubject {
    friend class ApparentSubject;

public:
    QString canonicalSubjectID;
    unique_ptr<ApparentSubject> originalSubject;

    ResolvedSubject(IMM(QString) subject_id, TAKE(ApparentSubject) original_subject, Hints hints = Hint::NoHints);

    virtual ApparentSubjectSubType subType() const;

    virtual CEDE(ApparentSubject) clone() const;

    static CEDE(ResolvedSubject) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeToStreamSubImpl(QDataStream &mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_RESOLVEDSUBJECT_H
