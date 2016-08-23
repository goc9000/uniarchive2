/**
 * intermediate_format/errors/PingFailReason.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6d95b898b203910e000baede8e08e7d643a9341a12741cbd461b8ce97c34edea
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H

#include <QString>
#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class PingFailReason {
    UNDETERMINED = 0,
    BLOCKED_OR_UNSUPPORTED,
};

QString name_for_ping_fail_reason(PingFailReason reason);

QDebug operator<< (QDebug stream, PingFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H
