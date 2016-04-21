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

using namespace uniarchive2::intermediate_format;

using namespace std;

namespace uniarchive2 { namespace extraction { namespace msn {

QDomDocument load_xml_file(const QString& filename);

vector<IntermediateFormatConversation> extract_msn_messenger_xml_conversations(const QString &filename) {
    vector<IntermediateFormatConversation> conversations;

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = xml.documentElement();
    invariant(root_element.tagName() == "Log", "MSN archive root node should be <Log>");

    return conversations;
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

}}}
