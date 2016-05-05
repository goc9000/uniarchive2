/**
 * extraction/digsby/extract_digsby_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "extraction/digsby/extract_digsby_conversations.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace digsby {

vector<IntermediateFormatConversation> extract_digsby_conversations(IMM(QString)filename) {
    vector<IntermediateFormatConversation> conversations;

    return conversations;
}

}}}
