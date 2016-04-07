/**
 * utils/language/invariant.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_INVARIANT_H
#define UNIARCHIVE2_UTILS_LANGUAGE_INVARIANT_H

#include <QtDebug>

#define invariant(condition, format, ...) if (!(condition)) invariant_violation(format, ##__VA_ARGS__)
#define invariant_violation(format, ...) qFatal(format, ##__VA_ARGS__)
#define never_reached() invariant_violation("This code should never be reached")

#endif //UNIARCHIVE2_UTILS_LANGUAGE_INVARIANT_H
