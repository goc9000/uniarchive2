/**
 * extraction/yahoo/extract_msn_messenger_xml_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QIODevice>

#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace msn {

IntermediateFormatConversation init_prototype(const QString& filename);
IntermediateFormatConversation extract_conversation_for_session(
    QDomElement& mut_next_element,
    int session_id,
    const IntermediateFormatConversation& prototype,
    unsigned int conversation_no_in_file,
    unsigned int conversation_offset_in_file
);
QDomDocument load_xml_file(const QString& filename);
int read_int_attr(const QDomElement& node, const QString& attr_name);

vector<IntermediateFormatConversation> extract_msn_messenger_xml_conversations(const QString &filename) {
    vector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = init_prototype(filename);

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = xml.documentElement();
    invariant(root_element.tagName() == "Log", "MSN archive root node should be <Log>");

    int first_session_id = read_int_attr(root_element, "FirstSessionID");
    int last_session_id = read_int_attr(root_element, "LastSessionID");
    invariant(first_session_id < last_session_id, "FirstSessionID should be <= LastSessionID");

    QDomElement next_element = root_element.firstChildElement();
    int expected_session_id = first_session_id;
    unsigned int message_offset = 0;

    while (!next_element.isNull()) {
        conversations.push_back(
            extract_conversation_for_session(
                next_element,
                expected_session_id,
                prototype,
                (unsigned int)(conversations.size() + 1),
                message_offset
            )
        );
        expected_session_id++;
    }

    return conversations;
}

IntermediateFormatConversation init_prototype(const QString& filename) {
    IntermediateFormatConversation conversation(ArchiveFormats::MSN_MESSENGER_XML, IMProtocols::MSN);

    // TODO: fill in stub

    return conversation;
}

IntermediateFormatConversation extract_conversation_for_session(
    QDomElement& mut_next_element,
    int expected_session_id,
    const IntermediateFormatConversation& prototype,
    unsigned int conversation_no_in_file,
    unsigned int conversation_offset_in_file
) {
    invariant(!mut_next_element.isNull(), "Attempted to read session when none are left");

    int session_id = read_int_attr(mut_next_element, "SessionID");
    invariant(
        session_id == expected_session_id, "Expected session ID = %d, found %d",  expected_session_id, session_id
    );

    auto conversation = IntermediateFormatConversation::fromPrototype(prototype);
    conversation.numConversationInFile = conversation_no_in_file;
    conversation.conversationOffsetInFileEventBased = conversation_offset_in_file;

    while (!mut_next_element.isNull() && (read_int_attr(mut_next_element, "SessionID") == session_id)) {
        // TODO: create events in conversation
        mut_next_element = mut_next_element.nextSiblingElement();
    }

    return conversation;
}

QDomDocument load_xml_file(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", qUtf8Printable(filename));
    }

    QDomDocument xml;
    QString error_message;
    int error_line, error_column;

    if (!xml.setContent(&file, false, &error_message, &error_line, &error_column)) {
        qFatal(
            "Error reading XML file '%s': %s (at line %d, column %d)",
            qUtf8Printable(filename),
            qUtf8Printable(error_message),
            error_line,
            error_column
        );
    }

    return xml;
}

int read_int_attr(const QDomElement& node, const QString& attr_name) {
    invariant(node.hasAttribute(attr_name), "Node is missing attribute '%s'", qUtf8Printable(attr_name));

    QString value = node.attribute(attr_name);
    bool ok = false;
    int int_value = value.toInt(&ok);

    invariant(ok, "Invalid integer attribute value: '%s'", qUtf8Printable(value));

    return int_value;
}

}}}
