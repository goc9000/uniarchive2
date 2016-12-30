/**
 * utils/language/template_utils.hpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_TEMPLATE_UTILS_HPP
#define UNIARCHIVE2_UTILS_LANGUAGE_TEMPLATE_UTILS_HPP

#include <type_traits>

namespace uniarchive2 { namespace utils { namespace language {

// Source: http://stackoverflow.com/questions/22564254/resolve-ambiguous-template

template<typename T> struct is_vector_type { static const bool value = false; };
template<typename T> struct is_vector_type< std::vector<T> > { static const bool value = true; };

template<typename T> struct extract_vector_type {
private:
    template<typename U> struct ident {
        typedef U type;
    };

    template<typename C> static ident<C> test(vector<C>);

    static ident<void> test(...);

    typedef decltype(test(T())) vec_type;
public:
    typedef typename vec_type::type type;
};

}}}

#endif //UNIARCHIVE2_UTILS_LANGUAGE_TEMPLATE_UTILS_HPP
