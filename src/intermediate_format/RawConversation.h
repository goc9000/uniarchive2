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
#include "protocols/IMProtocol.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"
#include "utils/serialization/ISerializable.h"

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
using namespace uniarchive2::utils::serialization;

class RawConversation : public ISerializable {
public:
    // Mandatory metadata
    IMProtocol protocol;

    // Optional metadata
    unique_ptr<ApparentSubject> identity;
    vector<unique_ptr<ApparentSubject>> declaredPeers; // Note: the identity is excluded from the peers (if recognized)

    optional<bool> isConference;
    optional<QString> conferenceTitle;
    optional<ApparentTime> declaredStartDate;
    unique_ptr<ApparentSubject> declaredInitiator;

    unique_ptr<Provenance> provenance;

    vector<unique_ptr<RawEvent>> events;

    RawConversation();
    RawConversation(IMProtocol protocol);

    virtual void serializeToStream(QDataStream& mut_stream) const;

    void writeToDebugStream(QDebug stream, bool all_messages = false) const;

    static RawConversation fromPrototype(IMM(RawConversation) prototype);
};

QDebug operator<< (QDebug stream, IMM(RawConversation) convo);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATION_H
