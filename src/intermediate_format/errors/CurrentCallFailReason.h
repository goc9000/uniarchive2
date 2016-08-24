/**
 * intermediate_format/errors/CurrentCallFailReason.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): c59060c6c02f478f7c63de04f4be0436f79e88cecc8d446d1c46d6d28526a4e8
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H

#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class CurrentCallFailReason {
    UNDETERMINED = 0,
    CONNECTION_DROPPED,
};

QString name_for_current_call_fail_reason(CurrentCallFailReason reason);

QDebug operator<< (QDebug stream, CurrentCallFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H
