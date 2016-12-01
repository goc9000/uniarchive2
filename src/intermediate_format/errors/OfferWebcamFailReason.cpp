/**
 * intermediate_format/errors/OfferWebcamFailReason.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6346af504b375d52a520c68df4833830c8a9d60e3747cfc6b304d2edefed7e1a
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

QString name_for_offer_webcam_fail_reason(OfferWebcamFailReason reason) {
    switch (reason) {
        case OfferWebcamFailReason::UNDETERMINED:
            return "undetermined";
        case OfferWebcamFailReason::UNSUPPORTED:
            return "unsupported";
    }

    invariant_violation("Invalid OfferWebcamFailReason value (%lld)", (int64_t)reason);
}

QDebug operator<< (QDebug stream, OfferWebcamFailReason reason) {
    stream << QP(name_for_offer_webcam_fail_reason(reason));

    return stream;
}

QDataStream& operator<< (QDataStream& mut_stream, OfferWebcamFailReason reason) {
    mut_stream << (uint8_t)reason;

    return mut_stream;
}

}}}
