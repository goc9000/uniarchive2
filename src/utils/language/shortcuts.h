/**
 * utils/language/shortcuts.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_SHORTCUTS_H
#define UNIARCHIVE2_UTILS_LANGUAGE_SHORTCUTS_H

#include <memory>

using namespace std;

// Marker for a structure parameter that is not modified
#define IMM(type) const type&

// Marker for a structure passed by unique pointer, whose ownership will be taken by the called function
#define TAKE(type) unique_ptr<type>

// Marker for a return value passed by unique pointer, whose ownership is ceded to the caller
#define CEDE(type) unique_ptr<type>

#endif //UNIARCHIVE2_UTILS_LANGUAGE_SHORTCUTS_H
