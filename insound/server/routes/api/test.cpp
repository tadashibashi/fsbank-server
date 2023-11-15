#include "test.h"

#include <insound/core/BankBuilder.h>
#include <insound/core/MultipartMap.h>
#include <insound/core/Response.h>

#include <crow/common.h>

#include <utility>

namespace Insound
{

    TestRouter::TestRouter() : Router("api/test")
    {

    }

    static Response make_fsb(const crow::request &req)
    {
        auto map = MultipartMap::from(req);

        BankBuilder builder;
        BankBuilder::Result result;
        for (auto &[name, file]: map.files)
        {
            result = builder.addFile(file.data.data(), file.data.length());
            if (result != BankBuilder::OK)
                IN_ERR("Failed to add file \"{}\" to bank: {}",
                    name, result);
        }

        result = builder.build();
        if (result != BankBuilder::OK)
        {
            return Response::json( f("Failed to build bank: {}", result), 500);
        }

        return {"application/octet-stream", std::move(builder.data())};
    }

    void TestRouter::init()
    {
        CROW_BP_ROUTE(bp, "make-fsb")
            .methods("POST"_method)
            (make_fsb);
    }

}
