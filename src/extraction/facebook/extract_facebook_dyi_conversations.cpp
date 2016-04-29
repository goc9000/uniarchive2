/**
 * extraction/facebook/extract_facebook_dyi_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "utils/xml/qdom_utils.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::utils::xml;

namespace uniarchive2 { namespace extraction { namespace facebook {

vector<IntermediateFormatConversation> extract_facebook_dyi_conversations(const QString &filename) {
    vector<IntermediateFormatConversation> conversations;

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = get_dom_root(xml, "html");

    return conversations;
}

}}}
