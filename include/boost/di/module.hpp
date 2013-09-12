//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_HPP
    #define BOOST_DI_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/detail/module.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/module.hpp"           \
        )                                   \
    )

    namespace boost {
    namespace di {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class module
        : public detail::module<
              typename mpl::fold<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          mpl::is_sequence<mpl::_2>
                        , mpl::_2
                        , mpl::if_<
                              has_deps<mpl::_2>
                            , mpl::vector1<mpl::_2>
                            , default_scope<mpl::_2>
                          >
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
          >
    {
    public:
        detail::module<> operator()() const {
            return detail::module<>();
        }

        #include BOOST_PP_ITERATE()
    };

    } // namespace di
    } // namespace boost

    #endif

#else
    template<BOOST_DI_TYPES(Args)>
    detail::module<typename module<BOOST_DI_TYPES_PASS(Args)>::deps>
    operator()(BOOST_DI_ARGS(Args, args)) const {
        return detail::module<typename module<BOOST_DI_TYPES_PASS(Args)>::deps>(
            BOOST_DI_ARGS_FORWARD(args)
        );
    }

#endif

