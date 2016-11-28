/**
 * intermediate_format/content/RawMessageContentItemSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 880742dd8eefbc20f04b75913624d3132e4ee58058536567124386f2535a7214
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/RawMessageContentItemSubType.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

QString name_for_raw_message_content_item_sub_type(RawMessageContentItemSubType type) {
    switch (type) {
        case RawMessageContentItemSubType::ANSI_COLOR_TAG:
            return "ANSIColorTag";
        case RawMessageContentItemSubType::ANSI_RESET_TAG:
            return "ANSIResetTag";
        case RawMessageContentItemSubType::BOLD_TAG:
            return "BoldTag";
        case RawMessageContentItemSubType::CSS_STYLE_TAG:
            return "CSSStyleTag";
        case RawMessageContentItemSubType::EMPHASIS_TAG:
            return "EmphasisTag";
        case RawMessageContentItemSubType::FONT_TAG:
            return "FontTag";
        case RawMessageContentItemSubType::ITALIC_TAG:
            return "ItalicTag";
        case RawMessageContentItemSubType::LINK_TAG:
            return "LinkTag";
        case RawMessageContentItemSubType::RGB_COLOR_TAG:
            return "RGBColorTag";
        case RawMessageContentItemSubType::UNDERLINE_TAG:
            return "UnderlineTag";
        case RawMessageContentItemSubType::YAHOO_ALT_TAG:
            return "YahooAltTag";
        case RawMessageContentItemSubType::YAHOO_FADE_TAG:
            return "YahooFadeTag";
        case RawMessageContentItemSubType::FLAG_ICON:
            return "FlagIcon";
        case RawMessageContentItemSubType::SKYPE_EMOTICON:
            return "SkypeEmoticon";
        case RawMessageContentItemSubType::LINE_BREAK:
            return "LineBreak";
        case RawMessageContentItemSubType::MEDIA_ATTACHMENT:
            return "MediaAttachment";
        case RawMessageContentItemSubType::SKYPE_QUOTE:
            return "SkypeQuote";
        case RawMessageContentItemSubType::TEXT_SECTION:
            return "TextSection";
    }

    invariant_violation("Invalid RawMessageContentItemSubType value (%d)", type);
}

QDebug operator<< (QDebug stream, RawMessageContentItemSubType type) {
    stream << QP(name_for_raw_message_content_item_sub_type(type));

    return stream;
}

QDataStream& operator<< (QDataStream& mut_stream, RawMessageContentItemSubType type) {
    mut_stream << (uint32_t)type;

    return mut_stream;
}

}}}
