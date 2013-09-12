//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MAKE_MODULE_HPP
    #define BOOST_DI_MAKE_MODULE_HPP

    #include "boost/di/module.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/make_module.hpp"      \
        )                                   \
    )

    namespace boost {
    namespace di {

    detail::module<> make_module() {
        return module<>()();
    }

    #include BOOST_PP_ITERATE()

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    detail::module<typename module<BOOST_DI_TYPES_PASS(Args)>::deps>
    make_module(BOOST_DI_ARGS(Args, args)) {
        return detail::module<typename module<BOOST_DI_TYPES_PASS(Args)>::deps>(
            BOOST_DI_ARGS_FORWARD(args)
        );
    }

#endif

