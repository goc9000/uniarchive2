/**
 * intermediate_format/RawConversation.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATION_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATION_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/provenance/Provenance.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <memory>
#include <vector>

#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;

class RawConversation {
public:
    // Mandatory metadata
    ArchiveFormat originalArchiveFormat;
    IMProtocol protocol;

    // Optional metadata
    unique_ptr<ApparentSubject> identity;
    vector<unique_ptr<ApparentSubject>> declaredPeers;
    optional<bool> isConference;
    QString conferenceTitle;
    optional<ApparentTime> declaredStartDate;

    unique_ptr<Provenance> provenance;

    optional<unsigned int> numConversationInFile;
    optional<unsigned int> conversationOffsetInFileEventBased;

    vector<unique_ptr<RawEvent>> events;

    RawConversation();
    RawConversation(ArchiveFormat format, IMProtocol protocol);

    void writeToDebugStream(QDebug stream, bool all_messages = false) const;

    static RawConversation fromPrototype(IMM(RawConversation) prototype);
};

QDebug operator<< (QDebug stream, IMM(RawConversation) convo);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATION_H
