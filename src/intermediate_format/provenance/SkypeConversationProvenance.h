/**
 * intermediate_format/provenance/SkypeConversationProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_SKYPECONVERSATIONPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_SKYPECONVERSATIONPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QString>

#include <cstdint>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace std;
using namespace std::experimental;

class SkypeConversationProvenance : public Provenance {
public:
    unique_ptr<Provenance> base;

    uint64_t conversationDBID;
    QString chatname;
    optional<uint64_t> chatDBID;
    optional<uint64_t> callDBID;

    SkypeConversationProvenance(TAKE(Provenance) base, uint64_t convo_dbid, QString chatname);

    virtual CEDE(Provenance) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_SKYPECONVERSATIONPROVENANCE_H
