/**
 * utils/xml/load_xml.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_XML_LOAD_XML_H
#define UNIARCHIVE2_UTILS_XML_LOAD_XML_H

#include <QByteArray>
#include <QDomDocument>
#include <QString>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace xml {

QDomDocument load_xml_file(IMM(QString) filename, bool keep_whitespace=false);
QDomDocument xml_from_string(IMM(QString) xml_string, bool keep_whitespace=false, IMM(QString) source_name="XML");
QDomDocument xml_from_fragment_string(
    IMM(QString) xml_string,
    IMM(QString) root_node,
    bool keep_whitespace=false,
    IMM(QString) source_name="XML"
);
QDomDocument xml_from_raw_data(IMM(QByteArray) raw_data, bool keep_whitespace=false, IMM(QString) source_name="XML");

}}}

#endif //UNIARCHIVE2_UTILS_XML_LOAD_XML_H
