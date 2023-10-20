#pragma once
#include <insound/thirdparty/crow.hpp>
#include <insound/middleware/UserAuth.hpp>

namespace Insound {
    using App = crow::App<UserAuth>;

    /**
     * Initialize the app, only needs to be called once to start the server.
     */
    void initApp();

    /**
     * Get the application singleton object.
     */
    App &getApp();


    /**
     * Get a context object ref from app middleware.
     * `MiddlewareType` must be middleware registered with the App.
     *
     * @param    req - a ref to the current route's request object
     * @returns  a reference to the context object.
     */
    template <typename MiddlewareType>
    typename MiddlewareType::context &getContext(const crow::request &req)
    {
        return getApp().get_context<MiddlewareType>(req);
    }
}
