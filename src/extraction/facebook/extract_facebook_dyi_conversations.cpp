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
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>

#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "utils/xml/qdom_utils.h"
#include "utils/language/invariant.h"
#include "utils/external_libs/make_unique.hpp"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::utils::xml;

namespace uniarchive2 { namespace extraction { namespace facebook {

IntermediateFormatConversation init_prototype(IMM(QString) filename);
QString read_identity_screen_name(IMM(QDomElement) root_element);
void extract_conversations_in_section(
    QDomElement& mut_next_element,
    vector<IntermediateFormatConversation>& mut_conversations,
    IMM(IntermediateFormatConversation) prototype
);
IntermediateFormatConversation extract_thread(
    IMM(QDomElement) thread_element,
    IMM(IntermediateFormatConversation) prototype
);

vector<IntermediateFormatConversation> extract_facebook_dyi_conversations(IMM(QString) filename) {
    vector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = init_prototype(filename);

    QDomDocument xml = load_xml_file(filename);
    auto root_element = get_dom_root(xml, "html");

    QString identity_screen_name = read_identity_screen_name(root_element);
    prototype.identity = make_unique<SubjectGivenAsScreenName>(identity_screen_name);

    auto body_element = child_elem(root_element, "body");
    auto contents_div = only_child_elem_with_class(body_element, "div", "contents");

    auto element = contents_div.firstChildElement();
    invariant(element.tagName() == "h1", "Expected first element in contents div to be <h1>");

    element = element.nextSiblingElement();
    while (!element.isNull()) {
        extract_conversations_in_section(element, conversations, prototype);
    }

    return conversations;
}

IntermediateFormatConversation init_prototype(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    QString parent = full_filename.section(QDir::separator(), -2, -2);
    QString full_base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        (parent == "html") && (full_base_name == "messages.htm"),
        "Facebook DYI archive filename should have the form html/messages.htm, instead it looks like: %s",
        qUtf8Printable(filename.section(QDir::separator(), -2, -1))
    );

    IntermediateFormatConversation conversation(ArchiveFormats::FACEBOOK_DYI, IMProtocols::FACEBOOK);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );

    return conversation;
}

QString read_identity_screen_name(IMM(QDomElement) root_element) {
    auto head_element = child_elem(root_element, "head");
    auto title_element = child_elem(head_element, "title");

    QString title = read_text_only_content(title_element);

    static QRegularExpression title_pattern("^(.*) - Messages$");
    auto match = title_pattern.match(title);

    invariant(
        match.hasMatch(),
        "Expected title to be \"(screen name) - Messages\", but it is \"%s\"",
        qUtf8Printable(title)
    );

    return match.captured(1);
}

void extract_conversations_in_section(
    QDomElement& mut_next_element,
    vector<IntermediateFormatConversation>& mut_conversations,
    IMM(IntermediateFormatConversation) prototype
) {
    invariant(mut_next_element.tagName() == "div", "Expected section <div> to follow");

    auto thread_element = mut_next_element.firstChildElement();
    while (!thread_element.isNull()) {
        mut_conversations.push_back(extract_thread(thread_element, prototype));
        thread_element = thread_element.nextSiblingElement();
    }

    mut_next_element = mut_next_element.nextSiblingElement();
}

IntermediateFormatConversation extract_thread(
    IMM(QDomElement) thread_element,
    IMM(IntermediateFormatConversation) prototype
) {
    invariant(
        (thread_element.tagName() == "div") && (thread_element.attribute("class", "") == "thread"),
        "Expected thread to be defined by a <div class=\"thread\">"
    );

    IntermediateFormatConversation conversation = IntermediateFormatConversation::fromPrototype(prototype);

    return conversation;
}

}}}
