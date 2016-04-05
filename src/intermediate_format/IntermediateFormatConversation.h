/**
 * intermediate_format/IntermediateFormatConversation.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_INTERMEDIATEFORMATCONVERSATION_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_INTERMEDIATEFORMATCONVERSATION_H

#include <memory>
#include <vector>

#include <QDebug>
#include <QString>
#include <QVector>

#include "intermediate_format/ApparentTime.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "protocols/ArchiveFormats.h"
#include "protocols/FullAccountName.h"
#include "utils/external_libs/optional.hpp"

using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::protocols;
using namespace std;
using namespace std::experimental;

namespace uniarchive2 { namespace intermediate_format {

struct IntermediateFormatConversation {
public:
    // Mandatory metadata
    ArchiveFormats originalArchiveFormat;
    FullAccountName localAccount;
    QVector<FullAccountName> declaredRemoteAccounts;

    // Optional metadata
    optional<bool> isConference;
    optional<QString> originalFilename;
    optional<ApparentTime> fileLastModifiedTime;

    QVector<shared_ptr<IntermediateFormatEvent>> events;

    IntermediateFormatConversation();
    IntermediateFormatConversation(ArchiveFormats format, FullAccountName local_account);
};

QDebug operator<< (QDebug stream, const IntermediateFormatConversation& convo);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_INTERMEDIATEFORMATCONVERSATION_H
