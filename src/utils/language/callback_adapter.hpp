/**
 * utils/language/callback_adapter.hpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_CALLBACK_ADAPTER_HPP
#define UNIARCHIVE2_UTILS_LANGUAGE_CALLBACK_ADAPTER_HPP

#include <functional>

namespace uniarchive2 { namespace utils { namespace language {

namespace detail {
    template<typename T>
    struct memfun_type {
        using type = void;
    };
    template<typename Ret, typename Class, typename... Args>
    struct memfun_type<Ret (Class::*)(Args...) const> {
        using type = std::function<Ret(Args...)>;
    };
}

template<typename F>
typename detail::memfun_type<decltype (&F::operator())>::type callback_adapter(F const& func) {
    return func;
};

}}}

#endif //UNIARCHIVE2_UTILS_LANGUAGE_CALLBACK_ADAPTER_HPP
