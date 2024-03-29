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

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace xml {

QDomElement get_dom_root(IMM(QDomDocument) document, IMM(QString) expected_root_name);

QDomElement child_elem(IMM(QDomElement) node, IMM(QString) child_name);
QDomElement only_child_elem(IMM(QDomElement) node, IMM(QString) child_name);
QDomElement child_elem_with_class(IMM(QDomElement) node, IMM(QString) child_name, IMM(QString) class_name);
QDomElement only_child_elem_with_class(IMM(QDomElement) node, IMM(QString) child_name, IMM(QString) class_name);

int read_int_attr(IMM(QDomElement) node, IMM(QString) attr_name);
unsigned int read_uint_attr(IMM(QDomElement) node, IMM(QString) attr_name);
QString read_string_attr(IMM(QDomElement) node, IMM(QString) attr_name);
QString read_only_string_attr(IMM(QDomElement) node, IMM(QString) attr_name);
QString read_optional_only_string_attr(IMM(QDomElement) node, IMM(QString) attr_name);
QDateTime read_iso_date_attr(IMM(QDomElement) node, IMM(QString) attr_name);

QString read_text_only_content(IMM(QDomElement) node);

QString xml_to_string(IMM(QDomNode) node);
QByteArray xml_to_raw_data(IMM(QDomNode) node);

}}}

#endif //UNIARCHIVE2_UTILS_XML_QDOM_UTILS_H
