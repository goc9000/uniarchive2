/**
 * utils/html/entities.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QMap>

#include "utils/language/invariant.h"
#include "utils/html/entities.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace html {

QString decode_html_entities(IMM(QString) html_text) {
    // Note that this isn't very optimized, but it'll do for most cases

    QREGEX_CI(
        entity_pattern,
        "&("
        "(?<by_name>[a-z]+)|"
        "#((?<by_code>[0-9]+)|x(?<by_hex>[a-f0-9]+))"
        ");"
    );

    QString result;

    int position = 0;
    auto matches = entity_pattern.globalMatch(html_text);
    while (matches.hasNext()) {
        auto match = matches.next();
        result += html_text.mid(position, match.capturedStart(0) - position);

        if (match.capturedLength("by_name")) {
            result += html_entity_for_name(match.captured("by_name"));
        } else if (match.capturedLength("by_code")) {
            result += QChar(match.captured("by_code").toInt(nullptr, 10));
        } else if (match.capturedLength("by_hex")) {
            result += QChar(match.captured("by_hex").toInt(nullptr, 16));
        }

        position = match.capturedEnd(0);
    }

    result += html_text.mid(position);

    return result;
}

QChar html_entity_for_name(IMM(QString) entity_name) {
    // Source for this table: http://www.w3schools.com/charsets/ref_html_entities_4.asp
    // Note that HTML5 supports more entities, but most IM services far predate that standard, so we're safe.

    static QMap<QString, QChar> entities {
        {"quot",     '"'},
        {"amp",      '&'},
        {"lt",       '<'},
        {"gt",       '>'},
        { "nbsp",    160 },
        { "iexcl",   161 },
        { "cent",    162 },
        { "pound",   163 },
        { "curren",  164 },
        { "yen",     165 },
        { "brvbar",  166 },
        { "sect",    167 },
        { "uml",     168 },
        { "copy",    169 },
        { "ordf",    170 },
        { "laquo",   171 },
        { "not",     172 },
        { "shy",     173 },
        { "reg",     174 },
        { "macr",    175 },
        { "deg",     176 },
        { "plusmn",  177 },
        { "sup2",    178 },
        { "sup3",    179 },
        { "acute",   180 },
        { "micro",   181 },
        { "para",    182 },
        { "middot",  183 },
        { "cedil",   184 },
        { "sup1",    185 },
        { "ordm",    186 },
        { "raquo",   187 },
        { "frac14",  188 },
        { "frac12",  189 },
        { "frac34",  190 },
        { "iquest",  191 },
        { "Agrave",  192 },
        { "Aacute",  193 },
        { "Acirc",   194 },
        { "Atilde",  195 },
        { "Auml",    196 },
        { "Aring",   197 },
        { "AElig",   198 },
        { "Ccedil",  199 },
        { "Egrave",  200 },
        { "Eacute",  201 },
        { "Ecirc",   202 },
        { "Euml",    203 },
        { "Igrave",  204 },
        { "Iacute",  205 },
        { "Icirc",   206 },
        { "Iuml",    207 },
        { "ETH",     208 },
        { "Ntilde",  209 },
        { "Ograve",  210 },
        { "Oacute",  211 },
        { "Ocirc",   212 },
        { "Otilde",  213 },
        { "Ouml",    214 },
        { "times",   215 },
        { "Oslash",  216 },
        { "Ugrave",  217 },
        { "Uacute",  218 },
        { "Ucirc",   219 },
        { "Uuml",    220 },
        { "Yacute",  221 },
        { "THORN",   222 },
        { "szlig",   223 },
        { "agrave",  224 },
        { "aacute",  225 },
        { "acirc",   226 },
        { "atilde",  227 },
        { "auml",    228 },
        { "aring",   229 },
        { "aelig",   230 },
        { "ccedil",  231 },
        { "egrave",  232 },
        { "eacute",  233 },
        { "ecirc",   234 },
        { "euml",    235 },
        { "igrave",  236 },
        { "iacute",  237 },
        { "icirc",   238 },
        { "iuml",    239 },
        { "eth",     240 },
        { "ntilde",  241 },
        { "ograve",  242 },
        { "oacute",  243 },
        { "ocirc",   244 },
        { "otilde",  245 },
        { "ouml",    246 },
        { "divide",  247 },
        { "oslash",  248 },
        { "ugrave",  249 },
        { "uacute",  250 },
        { "ucirc",   251 },
        { "uuml",    252 },
        { "yacute",  253 },
        { "thorn",   254 },
        { "yuml",    255 },
        { "fnof",    402 },
        { "Alpha",   913 },
        { "Beta",    914 },
        { "Gamma",   915 },
        { "Delta",   916 },
        { "Epsilon", 917 },
        { "Zeta",    918 },
        { "Eta",     919 },
        { "Theta",   920 },
        { "Iota",    921 },
        { "Kappa",   922 },
        { "Lambda",  923 },
        { "Mu",      924 },
        { "Nu",      925 },
        { "Xi",      926 },
        { "Omicron", 927 },
        { "Pi",      928 },
        { "Rho",     929 },
        { "Sigma",   931 },
        { "Tau",     932 },
        { "Upsilon", 933 },
        { "Phi",     934 },
        { "Chi",     935 },
        { "Psi",     936 },
        { "Omega",   937 },
        { "alpha",   945 },
        { "beta",    946 },
        { "gamma",   947 },
        { "delta",   948 },
        { "epsilon", 949 },
        { "zeta",    950 },
        { "eta",     951 },
        { "theta",   952 },
        { "iota",    953 },
        { "kappa",   954 },
        { "lambda",  955 },
        { "mu",      956 },
        { "nu",      957 },
        { "xi",      958 },
        { "omicron", 959 },
        { "pi",      960 },
        { "rho",     961 },
        { "sigmaf",  962 },
        { "sigma",   963 },
        { "tau",     964 },
        { "upsilon", 965 },
        { "phi",     966 },
        { "chi",     967 },
        { "psi",     968 },
        { "omega",   969 },
        { "thetasym", 977 },
        { "upsih",   978 },
        { "piv",     982 },
        { "bull",    8226 },
        { "hellip",  8230 },
        { "prime",   8242 },
        { "Prime",   8243 },
        { "oline",   8254 },
        { "frasl",   8260 },
        { "weierp",  8472 },
        { "image",   8465 },
        { "real",    8476 },
        { "trade",   8482 },
        { "alefsym", 8501 },
        { "larr",    8592 },
        { "uarr",    8593 },
        { "rarr",    8594 },
        { "darr",    8595 },
        { "harr",    8596 },
        { "crarr",   8629 },
        { "lArr",    8656 },
        { "uArr",    8657 },
        { "rArr",    8658 },
        { "dArr",    8659 },
        { "hArr",    8660 },
        { "forall",  8704 },
        { "part",    8706 },
        { "exist",   8707 },
        { "empty",   8709 },
        { "nabla",   8711 },
        { "isin",    8712 },
        { "notin",   8713 },
        { "ni",      8715 },
        { "prod",    8719 },
        { "sum",     8721 },
        { "minus",   8722 },
        { "lowast",  8727 },
        { "radic",   8730 },
        { "prop",    8733 },
        { "infin",   8734 },
        { "ang",     8736 },
        { "and",     8743 },
        { "or",      8744 },
        { "cap",     8745 },
        { "cup",     8746 },
        { "int",     8747 },
        { "there4",  8756 },
        { "sim",     8764 },
        { "cong",    8773 },
        { "asymp",   8776 },
        { "ne",      8800 },
        { "equiv",   8801 },
        { "le",      8804 },
        { "ge",      8805 },
        { "sub",     8834 },
        { "sup",     8835 },
        { "nsub",    8836 },
        { "sube",    8838 },
        { "supe",    8839 },
        { "oplus",   8853 },
        { "otimes",  8855 },
        { "perp",    8869 },
        { "sdot",    8901 },
        { "lceil",   8968 },
        { "rceil",   8969 },
        { "lfloor",  8970 },
        { "rfloor",  8971 },
        { "lang",    9001 },
        { "rang",    9002 },
        { "loz",     9674 },
        { "spades",  9824 },
        { "clubs",   9827 },
        { "hearts",  9829 },
        { "diams",   9830 },
        // extras
        { "euro",    8364 },
        { "apos",    '\'' },
    };

    if (entities.contains(entity_name)) {
        return entities[entity_name];
    }

    invariant_violation("Unsupported HTML entity: &%s;", QP(entity_name));
}

}}}
