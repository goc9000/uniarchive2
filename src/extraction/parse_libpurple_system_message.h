/**
 * extraction/parse_libpurple_system_message.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_PARSE_LIBPURPLE_SYSTEM_MESSAGE_H
#define UNIARCHIVE2_EXTRACTION_PARSE_LIBPURPLE_SYSTEM_MESSAGE_H

#include <QString>

#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/IMProtocol.h"
#include "utils/language/shortcuts.h"

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction {

CEDE(RawEvent) parse_libpurple_system_message(
    unsigned int event_index,
    IMM(ApparentTime) event_time,
    IMM(QString) content,
    bool is_html,
    IMProtocol protocol
);

CEDE(ApparentSubject) parse_libpurple_subject(IMM(QString) subject, IMProtocol protocol, bool is_html);

}}

#endif //UNIARCHIVE2_EXTRACTION_PARSE_LIBPURPLE_SYSTEM_MESSAGE_H
