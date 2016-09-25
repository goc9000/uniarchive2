/**
 * intermediate_format/content/formatting/LinkTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 437a633e2b433868731641433f48d63970e21a149226f891c2846611102a99e6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H

#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QUrl>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;

struct LinkTag : StandardTag {
public:
    optional<QUrl> url;

    LinkTag(bool open);
    LinkTag(IMM(QUrl) url);

protected:
    virtual QString tagName() const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
