#include "Track.h"
#include <insound/core/mongo/Model.h>
#include <insound/core/ZipWriter.h>
#include <insound/core/s3.h>

namespace Insound
{
    Mongo::Document<User> Track::getOwner() const
    {
        Mongo::Model<User> user;
        auto ownerDoc = user.findById(this->owner);
        if (!ownerDoc)
        {
            throw std::runtime_error("Failed to find owner of Track. Track may "
                "contain an invalid owner id.");
        }

        return ownerDoc.value();
    }

    std::string Track::downloadZip() const
    {
        throw "not implemented";
    }
}
