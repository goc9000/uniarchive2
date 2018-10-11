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
#include <QDomDocument>
#include <QIODevice>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::intermediate_format::provenance;

class AtomicConversationSource {
public:
    virtual ~AtomicConversationSource() {
        // Do nothing by default
    }

    /**
     * A name to display for the source in debug/error messages.
     */
    virtual QString debugName() const = 0;

    /**
     * The filename as used for extracting the context of the conversation (e.g. user, date). May not represent a
     * currently existing file on disk.
     */
    virtual QString logicalFullFilename() const = 0;
    virtual QString logicalFilename() const;
    virtual QString logicalFilenameSection(int index) const;
    virtual QString logicalFilenameSections(int from, int to) const;
    virtual QString baseName() const = 0;
    virtual QString extension() const = 0;

    /**
     * Returns a filename that can be opened to get the source's data. Not available for all sources.
     * Use only as a last resort!
     */
    virtual QString materializedFilename() const = 0;

    /**
     * Open the data as a Qt I/O device. This is guaranteed to try to use streaming if possible.
     */
    virtual CEDE(QIODevice) openDevice() const = 0;

    /**
     * Loads all data in memory. Fails on excessively large files.
     */
    virtual QByteArray fullData() const = 0;
    virtual QString fullUTF8Text() const;
    virtual QDomDocument fullXML(bool keep_whitespace=false) const;

    virtual CEDE(Provenance) asProvenance() const = 0;
};

}}

#endif //UNIARCHIVE2_SOURCES_ATOMICCONVERSATIONSOURCE_H
