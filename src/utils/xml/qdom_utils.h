/**
 * utils/xml/qdom_utils.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_XML_QDOM_UTILS_H
#define UNIARCHIVE2_UTILS_XML_QDOM_UTILS_H

#include <QByteArray>
#include <QDateTime>
#include <QDomDocument>
#include <QDomElement>
#include <QString>

namespace uniarchive2 { namespace utils { namespace xml {

QDomDocument load_xml_file(const QString& filename);
QDomElement get_dom_root(const QDomDocument& document, const QString& expected_root_name);
QDomElement child_elem(const QDomElement& node, const QString& child_name);
QDomElement only_child_elem(const QDomElement& node, const QString& child_name);
int read_int_attr(const QDomElement& node, const QString& attr_name);
QString read_string_attr(const QDomElement& node, const QString& attr_name);
QDateTime read_iso_date_attr(const QDomElement& node, const QString& attr_name);
QString read_text_only_content(const QDomElement& node);
QString xml_to_string(const QDomNode& node);
QByteArray xml_to_raw_data(const QDomNode& node);

}}}

#endif //UNIARCHIVE2_UTILS_XML_QDOM_UTILS_H
