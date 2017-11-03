/**
 * intermediate_format/content/formatting/YahooFadeTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): cd3a5d1f0565523d98311aa0608ade9f24e09430b489feef9c4679027c49f2e8
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H

#include "graphics/Color.h"
#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;
using namespace uniarchive2::graphics;

struct YahooFadeTag : StandardTag {
public:
    optional<vector<Color>> colors;

    YahooFadeTag(bool open);
    YahooFadeTag(IMM(vector<Color>) colors);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(YahooFadeTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H
