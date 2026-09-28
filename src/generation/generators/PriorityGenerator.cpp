#include <generation/generators/PriorityGenerator.h>
#include <core/base/node/Node.h>

#include <sstream>
#include <stdexcept>

namespace cg {
    IGenerator* PriorityGenerator::take(int priority, std::unique_ptr<IGenerator> gen) {
        if (priority < 1)
            throw std::runtime_error("priority can't be lower than 1");

        if (priority_map.find(priority) != priority_map.end())
            throw std::runtime_error("PriorityGenerator can't owner several generators with same priority");

        auto* raw = gen.get();
        priority_map[priority] = std::move(gen);

        if (priority > lowest_priority) lowest_priority = priority;
        
        return raw;
    }
    IGenerator* PriorityGenerator::put_back(std::unique_ptr<IGenerator> gen) {
        int priority = lowest_priority + 1;

        auto* raw = gen.get();

        return take(priority, std::move(gen));
    }
    IGenerator* PriorityGenerator::put_front(std::unique_ptr<IGenerator> gen) {
        int highest_priority = 1;

        auto* raw = gen.get();

        // Up all priorities to put new gen in front
        std::map<int, std::unique_ptr<cg::IGenerator>> new_map;
        for (auto& it : priority_map) {
            new_map[it.first + 1] = std::move(it.second);
        }
        priority_map = std::move(new_map);
        
        return take(highest_priority, std::move(gen));
    }

    std::unique_ptr<IGenerator> PriorityGenerator::release(int priority) {
        auto it = priority_map.find(priority);

        if (it == priority_map.end())
            throw std::runtime_error("This generator doesn't handle generator with this priority");

        if (priority == lowest_priority)
            if (priority_map.size() == 1) {
                lowest_priority = 0;
            }
            else {
                auto copy_it = it;
                lowest_priority = (--copy_it)->first;
            }

        auto uptr = std::move(it->second);

        priority_map.erase(it);

        return uptr;
    }

    bool PriorityGenerator::can_generate(const core::Node* node) const {
        for (auto& [priority, gen] : priority_map)
            if (gen->can_generate(node)) return true;

        return false;
    }

    std::string PriorityGenerator::generate(
        const core::Node* ent,
        bool declaration,
        const core::Node* scoup,
        const IDispatcher* dispatcher
    ) const {
        std::stringstream sstr;

        for (auto it = priority_map.begin(); it != priority_map.end(); it++) {
            if (it != priority_map.begin()) sstr << " ";

            auto& gen = it->second;
            if (gen->can_generate(ent))
                sstr << gen->generate(ent, declaration, scoup, dispatcher);
        }

        return sstr.str();
    }
}