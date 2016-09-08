/**
 * intermediate_format/content/formatting/CSSStyleTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H

#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;

struct CSSStyleTag : StandardTag {
public:
    optional<QString> css;

    CSSStyleTag(IMM(QString) css);
    CSSStyleTag();

protected:
    virtual QString tagName() const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
