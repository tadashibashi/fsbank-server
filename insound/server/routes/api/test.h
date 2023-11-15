/**
 * @file test.h
 *
 * Contains test-related routes at "/api/test"
 */
#pragma once

#include "insound/core/Router.h"
namespace Insound {
    class TestRouter : public Router
    {
    public:
        TestRouter();

        void init() override;
    private:

    };
}
