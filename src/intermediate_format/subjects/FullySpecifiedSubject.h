/**
 * intermediate_format/subjects/FullySpecifiedSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_FULLYSPECIFIEDSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_FULLYSPECIFIEDSUBJECT_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace uniarchive2::protocols;

class FullySpecifiedSubject : public ApparentSubject {
public:
    FullAccountName accountName;
    QString screenName;

    FullySpecifiedSubject(IMM(FullAccountName) account_name, IMM(QString) screen_name, Hints hints = Hint::NoHints);

    virtual ApparentSubjectSubType subType() const;

    virtual CEDE(ApparentSubject) clone() const;

protected:
    virtual void serializeToStreamSubImpl(QDataStream &mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_FULLYSPECIFIEDSUBJECT_H
