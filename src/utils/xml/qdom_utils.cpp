/**
 * utils/xml/qdom_utils.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QFile>

#include "utils/language/invariant.h"
#include "utils/xml/qdom_utils.h"

namespace uniarchive2 { namespace utils { namespace xml {

QDomDocument load_xml_file(IMM(QString) filename) {
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

QDomElement get_dom_root(IMM(QDomDocument) document, IMM(QString) expected_root_name) {
    QDomElement root_node = document.documentElement();
    invariant(
        root_node.tagName() == expected_root_name,
        "Expected root node to be <%s>, found <%s>",
        qUtf8Printable(expected_root_name),
        qUtf8Printable(root_node.tagName())
    );

    return root_node;
}

QDomElement child_elem(IMM(QDomElement) node, IMM(QString) child_name) {
    QDomElement child_node = node.firstChildElement(child_name);
    invariant(
        !child_node.isNull(),
        "Expected to find <%s> node under <%s>",
        qUtf8Printable(child_name),
        qUtf8Printable(node.tagName())
    );

    return child_node;
}

QDomElement only_child_elem(IMM(QDomElement) node, IMM(QString) child_name) {
    QDomElement child_node = node.firstChildElement();
    invariant(
        (child_node.tagName() == child_name) && child_node.nextSiblingElement().isNull(),
        "Expected to find only a <%s> node under <%s>",
        qUtf8Printable(child_name),
        qUtf8Printable(node.tagName())
    );

    return child_node;
}

QDomElement child_elem_with_class(IMM(QDomElement) node, IMM(QString) child_name, IMM(QString) class_name) {
    for (
        auto child_node = node.firstChildElement(child_name);
        !child_node.isNull();
        child_node = child_node.nextSiblingElement(child_name)
        ) {
        if (child_node.attribute("class", "") == class_name) {
            return child_node;
        }
    }

    invariant_violation(
        "Could not find a child node of type <%s> and class \"%s\"",
        qUtf8Printable(child_name),
        qUtf8Printable(class_name)
    );
}

QDomElement only_child_elem_with_class(IMM(QDomElement) node, IMM(QString) child_name, IMM(QString) class_name) {
    QDomElement found_node;

    for (
        auto child_node = node.firstChildElement(child_name);
        !child_node.isNull();
        child_node = child_node.nextSiblingElement(child_name)
        ) {
        if (child_node.attribute("class", "") == class_name) {
            invariant(
                found_node.isNull(),
                "Expected to find only one node of type <%s> and class \"%s\"",
                qUtf8Printable(child_name),
                qUtf8Printable(class_name)
            );

            found_node = child_node;
        }
    }

    invariant(
        !found_node.isNull(),
        "Could not find a child node of type <%s> and class \"%s\"",
        qUtf8Printable(child_name),
        qUtf8Printable(class_name)
    );

    return found_node;
}

int read_int_attr(IMM(QDomElement) node, IMM(QString) attr_name) {
    QString value_text = read_string_attr(node, attr_name);
    bool ok = false;
    int value = value_text.toInt(&ok);

    invariant(ok, "Invalid integer attribute value: '%s'", qUtf8Printable(value_text));

    return value;
}

QString read_string_attr(IMM(QDomElement) node, IMM(QString) attr_name) {
    invariant(node.hasAttribute(attr_name), "Node is missing attribute '%s'", qUtf8Printable(attr_name));

    return node.attribute(attr_name);
}

QDateTime read_iso_date_attr(IMM(QDomElement) node, IMM(QString) attr_name) {
    QString value_text = read_string_attr(node, attr_name);

    QDateTime value = QDateTime::fromString(value_text, Qt::ISODate);
    invariant(value.isValid(), "Invalid ISO datetime attribute value: '%s'", qUtf8Printable(value_text));

    return value;
}

QString read_text_only_content(IMM(QDomElement) node) {
    invariant(
        node.firstChildElement().isNull(),
        "Not expecting <%s> node to have sub-elements",
        qUtf8Printable(node.tagName())
    );

    return node.text();
}

QString xml_to_string(IMM(QDomNode) node) {
    QString text;
    QTextStream stream(&text);

    node.save(stream, 0);

    return text;
}

QByteArray xml_to_raw_data(IMM(QDomNode) node) {
    QByteArray raw_data;
    QTextStream stream(&raw_data);

    node.save(stream, 0);

    return raw_data;
}

}}}
