#include <generation/generators/cpp/CppGenerators.h>
#include <core/base/node/Node.h>
#include <generation/utils.h>

#include <sstream>
#include <stack>

namespace cg::gen::cpp {

    
    std::string relative_path(
        const core::Node* ent,
        const core::Node* scope
    ) {
        std::stack<const core::Node*> path;

        auto* iter = ent;
        while (iter) {
            if (iter == scope) break;

            path.push(iter);
            iter = iter->get_parent();
        }

        std::stringstream sstr;
        while(!path.empty()) {
            if (!path.top()->get_entities().has<entities::cpp::NamedEntity>())
                sstr << "undefined";
            else
                sstr << path.top()->get_entities().get<entities::cpp::NamedEntity>()->get_name();
            path.pop();

            if (!path.empty()) sstr << "::";
        }

        return sstr.str();
    }

    bool AliasGenerator::can_generate(const core::Node* node) const {
        if (!node) return false;

        if (node->get_entities().has<cg::entities::cpp::IAlias>()) return true;

        return false;
    }

    std::string AliasGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scope,
        const IDispatcher* dispatcher
    ) const {
        using namespace cg::entities::cpp;

        std::stringstream sstr;
        auto& entities = ent->get_entities();

        sstr << "using ";

        if (!entities.has<NamedEntity>()) sstr << "undefined";
        else sstr << entities.get<NamedEntity>()->get_name();

        sstr << " = ";

        if (entities.has<TypeHandler>())
            sstr << dispatcher->generate(entities.get<TypeHandler>()->get_type(), declaration, scope);

        sstr << ";";

        return sstr.str();
    }

    bool TypeGenerator::can_generate(const core::Node* node) const {
        if (!node) return false;

        if (node->get_entities().has<cg::entities::cpp::IType>()) return true;

        return false;
    }

    std::string TypeGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scope,
        const IDispatcher* dispatcher
    ) const {
        using namespace cg::entities::cpp;

        std::stringstream sstr;
        auto& entities = ent->get_entities();

        if (entities.has<TypeQualificator>()) {
            auto is_const = entities.get<TypeQualificator>()->is_const();
            auto is_volatile = entities.get<TypeQualificator>()->is_volatile();

            sstr
                << (is_const ? "const" : "")
                << (is_volatile ? is_const ? " volatile" : "volatile" : "")
                << (is_volatile || is_const ? " " : "");
        }

        // Пускай валидатор разбирается с тем, кто в чем и где лежит, генератор просто генерирует
        if (
            (
                !entities.has<ClassHandler>()
                ||
                !entities.get<ClassHandler>()->get_class()
            )
            &&
            !entities.has<IAlias>()
        ) sstr << "undefined";
        else {
            sstr << (
                entities.has<IAlias>() ? relative_path(ent, scope)
                :
                relative_path(entities.get<ClassHandler>()->get_class(), scope)
            );
        }

        if (
            entities.has<TypeConstructor>()
            &&
            entities.get<TypeConstructor>()->has_qualificator()
        ) {
            std::stringstream tc_sstr;

            switch (entities.get<TypeConstructor>()->get_qualificator()) {
                case TYPE_CONSTRUCTORS::POINTER:
                    sstr << "*";
                    break;
                case TYPE_CONSTRUCTORS::REFERENCE:
                    sstr << "&";
                    break;
                case TYPE_CONSTRUCTORS::UNIVERSAL_REFERENCE:
                    sstr << "&&";
                    break;
            }

            auto is_const = entities.get<TypeConstructor>()->is_const();
            auto is_volatile = entities.get<TypeConstructor>()->is_volatile();

            sstr
                << (is_volatile || is_const ? " " : "")
                << (is_const ? "const" : "")
                << (is_volatile ? is_const ? " volatile" : "volatile" : "");
        }

        return sstr.str();
    }
    
    bool NamespaceGenerator::can_generate(const core::Node* node) const {
        if (!node) return false;

        if (node->get_entities().has<cg::entities::cpp::INamespace>()) return true;

        return false;
    }

    std::string NamespaceGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scoup,
        const IDispatcher* dispatcher
    ) const {
        std::stringstream sstr;

        auto path = relative_path(ent, scoup);
        
        if (!path.empty())
            sstr << "namespace " << path << " {\n";

        if (ent) {
            if (!ent->as_container())
                throw std::runtime_error("namespace node would be a tree node!");
                
            auto& tree_cast = *ent->as_container();
            
            if (dispatcher) {
                std::stringstream nested_sstr; // Для удобства и дебага разделил на отдельный поток
                for (size_t i = 0; i < tree_cast.children_size(); i++) {
                    if (i > 0) nested_sstr << "\n";
                    
                    auto* node = *(tree_cast.begin() + i);
                    nested_sstr <<
                        (
                            path.empty() ?
                                dispatcher->generate(node, declaration, ent)
                                :
                                tabulate(dispatcher->generate(node, declaration, ent), 1, "    ")
                        );
                }
                sstr << nested_sstr.str();
            }
        }

        if (!path.empty())
            sstr << "\n}";

        return sstr.str();
    }

    bool ClassGenerator::can_generate(const core::Node* node) const {
        if (!node) return false;

        if (node->get_entities().has<cg::entities::cpp::IClass>()) return true;

        return false;
    }

    std::string ClassGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scoup,
        const IDispatcher* dispatcher
    ) const {
        std::stringstream sstr;

        auto path = relative_path(ent, scoup);

        if (declaration && !path.empty())
            sstr << "class " << path << " {\n";

        if (ent) {
            if (!ent->as_container())
                throw std::runtime_error("namespace node would be a tree node!");
                
            auto& tree_cast = *ent->as_container();
            
            if (dispatcher) {
                std::stringstream nested_sstr;

                for (size_t i = 0; i < tree_cast.children_size(); i++) {
                    if (i > 0) nested_sstr << "\n";
                    
                    auto* node = *(tree_cast.begin() + i);
                    nested_sstr
                    << (declaration && !path.empty() ?
                        tabulate(dispatcher->generate(node, declaration, ent), 1, "    ")
                        :
                        dispatcher->generate(node, declaration, ent->get_parent()));
                }

                sstr << nested_sstr.str();
            }
        }

        if (declaration && !path.empty())
            sstr << "\n};";

        return sstr.str();
    }
}