/**
 * protocols/IMStatus.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_IMSTATUS_H
#define UNIARCHIVE2_PROTOCOLS_IMSTATUS_H

#include <QDebug>

namespace uniarchive2 { namespace protocols {

enum class IMStatus {
    INVALID = 0,
    AVAILABLE,
    AWAY,
    IDLE,
    OFFLINE,
};

QString name_for_im_protocol(IMStatus status);

QDebug operator<< (QDebug stream, IMStatus status);

}}

#endif //UNIARCHIVE2_PROTOCOLS_IMSTATUS_H
