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
                throw std::runtime_error("To extract a relative path in the form, each node must have a NamedEntity.");
            
            sstr << path.top()->get_entities().get<entities::cpp::NamedEntity>()->get_name();
            path.pop();

            if (!path.empty()) sstr << "::";
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

        sstr << "namespace " << relative_path(ent, scoup) << " {\n";

        auto* tree_cast = ent->as_container();
        if (!tree_cast)
            throw std::runtime_error("namespace node would be a tree node!");
        
        if (dispatcher)
            for (auto* node : *tree_cast) {
                tabulate(dispatcher->generate(node, declaration, ent), 1, "    ");
            }

        sstr << "\n}";

        return sstr.str();
    }
}