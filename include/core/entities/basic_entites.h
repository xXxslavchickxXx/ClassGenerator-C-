#pragma once

#include <string>
#include <core/base/IEntity.h>

namespace cg::entities {
    class NamedEntity : public cg::core::IEntity {
        std::string name;

    public:
        NamedEntity(const std::string& new_name = "") : name(new_name) {}

        void set_name(const std::string& new_name) { name = new_name; }
        const std::string& get_name() const { return name; }
    };

    class 
}