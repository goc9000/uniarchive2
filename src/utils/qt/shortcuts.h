/**
 * utils/qt/shortcuts.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_QT_SHORTCUTS_H
#define UNIARCHIVE2_UTILS_QT_SHORTCUTS_H

#include <QtDebug>
#include <QRegularExpression>

#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#define QP qUtf8Printable

#define QREGEX(name, pattern) static const QRegularExpression name = initQRegEx(pattern, \
    QRegularExpression::DotMatchesEverythingOption)
#define QREGEX_CI(name, pattern) static const QRegularExpression name = initQRegEx(pattern, \
    QRegularExpression::DotMatchesEverythingOption | QRegularExpression::CaseInsensitiveOption)

static inline QRegularExpression initQRegEx(IMM(QString) pattern, QRegularExpression::PatternOptions options) {
    QRegularExpression regex(pattern, options);

    invariant(regex.isValid(), "Invalid regex: %s (%s)", QP(pattern), QP(regex.errorString()));

    return regex;
}

#define QREGEX_MATCH(match_name, pattern, input)\
    QREGEX(pattern_ ## match_name, pattern);\
    auto match_name = pattern_ ## match_name.match(input)

#define QREGEX_MUST_MATCH(match_name, pattern, input, error_text)\
    QREGEX_MATCH(match_name, pattern, input);\
    invariant(match_name.hasMatch(), error_text, QP(input))

#define QREGEX_WILL_MATCH(match_name, pattern, input)\
    QREGEX_MUST_MATCH(match_name, pattern, input, "Text \"%s\" unexpectedly failed to match")

#define QREGEX_MATCH_CI(match_name, pattern, input)\
    QREGEX_CI(pattern_ ## match_name, pattern);\
    auto match_name = pattern_ ## match_name.match(input)

#define QREGEX_MUST_MATCH_CI(match_name, pattern, input, error_text)\
    QREGEX_MATCH_CI(match_name, pattern, input);\
    invariant(match_name.hasMatch(), error_text, QP(input))

#define QREGEX_WILL_MATCH_CI(match_name, pattern, input)\
    QREGEX_MUST_MATCH_CI(match_name, pattern, input, "Text \"%s\" unexpectedly failed to match")

#endif //UNIARCHIVE2_UTILS_QT_SHORTCUTS_H
