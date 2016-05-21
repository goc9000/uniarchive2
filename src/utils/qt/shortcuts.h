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

#endif //UNIARCHIVE2_UTILS_QT_SHORTCUTS_H
