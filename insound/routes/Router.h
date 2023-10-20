#pragma once
#include <insound/thirdparty/crow.hpp>

namespace Insound {
    class Router
    {
    public:
        explicit Router(const std::string &route) : bp(route) { }
        
        Router(Router &&router) : bp(std::move(router.bp))
        {}

        virtual ~Router() = default;

        virtual crow::Blueprint &config() = 0;
    protected:
        crow::Blueprint bp;
    };
}
