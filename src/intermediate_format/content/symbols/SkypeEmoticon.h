/**
 * intermediate_format/content/symbols/SkypeEmoticon.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): aed583a07d706883622ecd15a670742cd5694c7d258dbb33659307d52bf93ade
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEEMOTICON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEEMOTICON_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

#include <QDataStream>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct SkypeEmoticon : RawMessageContentItem {
public:
    QString semantics;
    QString textEquivalent;

    SkypeEmoticon(IMM(QString) semantics, IMM(QString) text_equivalent);

    virtual RawMessageContentItemSubType subType() const;

    static CEDE(SkypeEmoticon) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEEMOTICON_H
