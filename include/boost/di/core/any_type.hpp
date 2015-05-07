//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class TParent, class TInjector>
struct any_type {
    template<class T>
    struct is_referable_impl {
        static constexpr auto value =
            std::remove_reference_t<decltype(binder::resolve<T>((TInjector*)nullptr))>::template
                is_referable<T>::value;
    };

    template<class T>
    struct is_creatable_impl {
        static constexpr auto value = TInjector::template is_creatable<T>::value;
    };

    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T>
    using is_referable = std::enable_if_t<is_referable_impl<T>::value>;

    template<class T>
    using is_creatable = std::enable_if_t<is_creatable_impl<T>::value>;

    template<class T, class = is_not_same<T>, class = is_creatable<T>>
    operator T() {
        return injector_.template create_impl<T>();
    }

    BOOST_DI_WKND(BOOST_DI_GCC)(
        template<class T, class = is_not_same<T>, class = is_creatable<T&&>>
        operator T&&() const {
            return injector_.template create_impl<T&&>();
        }
    )()

    template<class T, class = is_not_same<T>, class = is_referable<T&>, class = is_creatable<T&>>
    operator T&() const {
        return injector_.template create_impl<T&>();
    }

    template<class T, class = is_not_same<T>, class = is_referable<const T&>, class = is_creatable<const T&>>
    operator const T&() const {
        return injector_.template create_impl<const T&>();
    }

    const TInjector& injector_;
};

template<class TParent>
struct any_type<TParent, aux::none_t> {
    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T, class = is_not_same<T>>
    operator T();

    template<class T, class = is_not_same<T>>
    operator T&() const;

    BOOST_DI_WKND(BOOST_DI_GCC)(
        template<class T, class = is_not_same<T>>
        operator T&&() const;
    )()

    template<class T, class = is_not_same<T>>
    operator const T&() const;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}}} // boost::di::v1::core

#endif

