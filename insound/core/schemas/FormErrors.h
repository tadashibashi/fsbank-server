#pragma once

#include <unordered_map>
#include <vector>

namespace Insound {
    /**
    * Provides descriptive form field errors to the frontend
    */
    class FormErrors
    {
    public:
        std::unordered_map<std::string, std::vector<std::string>> errors;

        inline
        void append(std::string_view fieldName, std::string_view errMsg)
        {
            errors[fieldName.data()].emplace_back(errMsg);
        }

        [[nodiscard]]
        inline
        bool empty() const
        {
            return errors.empty();
        }
    };
}
