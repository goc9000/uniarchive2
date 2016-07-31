/**
 * intermediate_format/errors/SendMessageFailReason.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_SENDMESSAGEFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_SENDMESSAGEFAILREASON_H

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class SendMessageFailReason {
    UNDETERMINED = 0,
    MESSAGE_TOO_LARGE,
    RECIPIENT_OFFLINE,
    CONNECTION_ERROR,
};

QString name_for_send_message_fail_reason(SendMessageFailReason protocol);

QDebug operator<< (QDebug stream, SendMessageFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_SENDMESSAGEFAILREASON_H
