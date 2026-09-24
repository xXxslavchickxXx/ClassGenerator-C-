#include <generation/generators/cpp/basic_generators.h>
#include <core/base/node/Node.h>

#include <sstream>

namespace cg {
    std::string NamedEntityGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scoup,
        const IGenerator* dispetcher
    ) {
        // TODO...
        // На самом деле тут дофига чего надо сделать:
        // 1. Самое важно это релативацию пути, типа получение относительного пути
        // 2. Это собственно отображение относительного пути
        // Суть в том, что я собираюсь этот генератор использовать не только
        // для неймспейс генератора, но и для других сущностей, например функции
        // там тоже надо генерить имя с относительным путем, и в целом все как надо будеь

        auto* ne_ptr = ent->get_entities().get<cg::entities::cpp::NamedEntity>();
        if (!ne_ptr) return "";

        std::stringstream sstr;

        sstr << ne_ptr->get_name();

        return sstr.str();
    }
}