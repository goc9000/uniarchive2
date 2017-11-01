/**
 * sources/atomic/AtomicConversationSource.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_SOURCES_ATOMICCONVERSATIONSOURCE_H
#define UNIARCHIVE2_SOURCES_ATOMICCONVERSATIONSOURCE_H

#include "intermediate_format/provenance/Provenance.h"

#include "utils/language/shortcuts.h"

#include <QString>
#include <QByteArray>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::intermediate_format::provenance;

class AtomicConversationSource {
public:
    /**
     * The filename as used for extracting the context of the conversation (e.g. user, date). May not represent an
     * existing file on disk.
     */
    virtual QString logicalFilename() const = 0;

    /**
     * Returns a filename that can be opened to get the source's data. Not available for all sources.
     */
    virtual QString materializedFilename() const = 0;

    virtual QByteArray fullData() const = 0;
    virtual QString fullUTF8Text() const;

    virtual CEDE(Provenance) asProvenance() const = 0;
};

}}

#endif //UNIARCHIVE2_SOURCES_ATOMICCONVERSATIONSOURCE_H
