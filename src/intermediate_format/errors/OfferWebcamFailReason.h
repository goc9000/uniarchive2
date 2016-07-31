/**
 * intermediate_format/errors/OfferWebcamFailReason.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_OFFERWEBCAMFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_OFFERWEBCAMFAILREASON_H

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class OfferWebcamFailReason {
    UNDETERMINED = 0,
    UNSUPPORTED,
};

QString name_for_offer_webcam_fail_reason(OfferWebcamFailReason protocol);

QDebug operator<< (QDebug stream, OfferWebcamFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_OFFERWEBCAMFAILREASON_H
