/**
 * utils/language/JavaStyleIterator.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_JAVASTYLEITERATOR_H
#define UNIARCHIVE2_UTILS_LANGUAGE_JAVASTYLEITERATOR_H

namespace uniarchive2 { namespace utils { namespace language {

template <class T> class JavaStyleIterator {
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
};

}}}

#endif //UNIARCHIVE2_UTILS_LANGUAGE_JAVASTYLEITERATOR_H
