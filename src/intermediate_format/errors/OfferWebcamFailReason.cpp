/**
 * intermediate_format/errors/OfferWebcamFailReason.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/errors/OfferWebcamFailReason.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

QString name_for_current_call_fail_reason(OfferWebcamFailReason reason) {
    switch (reason) {
        case OfferWebcamFailReason::UNDETERMINED:
            return "undetermined";
        case OfferWebcamFailReason::UNSUPPORTED:
            return "unsupported";
    }

    invariant_violation("Invalid OfferWebcamFailReason (value: %d)", reason);
}

QDebug operator<< (QDebug stream, OfferWebcamFailReason reason) {
    stream << QP(name_for_current_call_fail_reason(reason));

    return stream;
}

}}}
