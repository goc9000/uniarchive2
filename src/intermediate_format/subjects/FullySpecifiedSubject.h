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

#include <memory>

#include <QtDebug>
#include <QString>

#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

using namespace std;

using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class FullySpecifiedSubject : public ApparentSubject {
public:
    FullAccountName accountName;
    QString screenName;

    FullySpecifiedSubject(IMM(FullAccountName) account_name, IMM(QString) screen_name, Hints hints = Hint::NoHints);

    virtual CEDE(ApparentSubject) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_FULLYSPECIFIEDSUBJECT_H
