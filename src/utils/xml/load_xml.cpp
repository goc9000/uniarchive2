/**
 * utils/xml/load_xml.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/xml/load_xml.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QFile>
#include <QXmlInputSource>
#include <QXmlSimpleReader>

namespace uniarchive2 { namespace utils { namespace xml {

static QDomDocument parse_xml(QXmlInputSource* source, bool keep_whitespace, IMM(QString) source_name);

QDomDocument load_xml_file(IMM(QString) filename, bool keep_whitespace) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    QXmlInputSource source(&file);

    return parse_xml(&source, keep_whitespace, "XML file '" + filename + "'");
}

QDomDocument xml_from_string(IMM(QString) xml_string, bool keep_whitespace, IMM(QString) source_name) {
    QXmlInputSource source;
    source.setData(xml_string);

    return parse_xml(&source, keep_whitespace, source_name);
}

QDomDocument xml_from_fragment_string(
    IMM(QString) xml_string,
    IMM(QString) root_node,
    bool keep_whitespace,
    IMM(QString) source_name
) {
    return xml_from_string("<" + root_node + ">" + xml_string + "</" + root_node + ">", keep_whitespace, source_name);
}

QDomDocument xml_from_raw_data(IMM(QByteArray) raw_data, bool keep_whitespace, IMM(QString) source_name) {
    QXmlInputSource source;
    source.setData(raw_data);

    return parse_xml(&source, keep_whitespace, source_name);
}

static QDomDocument parse_xml(QXmlInputSource* source, bool keep_whitespace, IMM(QString) source_name) {
    QDomDocument xml;
    QString error_message;
    int error_line, error_column;

    QXmlSimpleReader reader;
    reader.setFeature("http://xml.org/sax/features/namespaces", false);
    reader.setFeature("http://qt-project.org/xml/features/report-whitespace-only-CharData", keep_whitespace);

    if (!xml.setContent(source, &reader, &error_message, &error_line, &error_column)) {
        qFatal(
            "Error parsing %s: %s (at line %d, column %d)",
            QP(source_name), QP(error_message), error_line, error_column
        );
    }

    return xml;
}

}}}
