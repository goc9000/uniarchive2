/**
 * extraction/extract_conversations_generic.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/extract_conversations_generic.h"
#include "extraction/adium/extract_adium_conversations.h"
#include "extraction/digsby/extract_digsby_conversations.h"
#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "extraction/pidgin/extract_pidgin_html_conversations.h"
#include "extraction/pidgin/extract_pidgin_txt_conversations.h"
#include "extraction/skype/extract_skype_conversations.h"
#include "extraction/whatsapp/extract_whatsapp_email_conversations.h"
#include "extraction/yahoo/extract_yahoo_messenger_dat_conversations.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace extraction {

using namespace uniarchive2::extraction::adium;
using namespace uniarchive2::extraction::digsby;
using namespace uniarchive2::extraction::facebook;
using namespace uniarchive2::extraction::msn;
using namespace uniarchive2::extraction::pidgin;
using namespace uniarchive2::extraction::skype;
using namespace uniarchive2::extraction::whatsapp;
using namespace uniarchive2::extraction::yahoo;

RawConversationCollection extract_conversations_generic(ArchiveFormat format, IMM(AtomicConversationSource) source) {
    QString filename = source.materializedFilename();

    switch (format) {
        case ArchiveFormat::ADIUM:
            return RawConversationCollection::from(extract_adium_conversation(source));
        case ArchiveFormat::DIGSBY:
            return RawConversationCollection::from(extract_digsby_conversation(source));
        case ArchiveFormat::FACEBOOK_DYI:
            return RawConversationCollection::from(extract_facebook_dyi_conversations(source));
        case ArchiveFormat::MSN_MESSENGER_XML:
            return RawConversationCollection::from(extract_msn_messenger_xml_conversations(source));
        case ArchiveFormat::PIDGIN_HTML:
            return RawConversationCollection::from(extract_pidgin_html_conversation(filename));
        case ArchiveFormat::PIDGIN_TXT:
            return RawConversationCollection::from(extract_pidgin_txt_conversation(filename));
        case ArchiveFormat::SKYPE:
            return RawConversationCollection::from(extract_skype_conversations(source));
        case ArchiveFormat::WHATSAPP_EMAIL:
            return RawConversationCollection::from(extract_whatsapp_email_conversation(source));
        case ArchiveFormat::YAHOO_MESSENGER_DAT:
            return RawConversationCollection::from(extract_yahoo_messenger_dat_conversations(source));
        default:
            invariant_violation(
                "Unsupported format for generic extraction of conversations: %s",
                 QP(name_for_archive_format(format))
            );
    }
}

}}
