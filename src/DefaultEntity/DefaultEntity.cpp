#include <AccessEntity/AccessEntity.h>

namespace cg::source {
    Variable::operator Arguement() const {
        auto arg = Arguement(name);
        arg.get_type() = get_type();
        arg.set_value(value);
        for (const auto& ns : namespace_prefix) {
            arg.add_namespace_prefix(ns);
        }
        return arg;
    }

    Namespace::Namespace(const std::string& name_)
        : name(name_) {}
    Namespace::Namespace(const char* name_)
        : name(std::string(name_)) {}
    Namespace::Namespace(const TypeName& t)
        : name(t) {}

    TypeName& Namespace::get_name() {
        return name;
    }
    std::vector<Variable>& Namespace::get_vars() {
        return variables;
    }
    std::vector<Alias>& Namespace::get_aliases() {
        return aliases;
    }
    std::vector<Function>& Namespace::get_functions() {
        return functions;
    }
    std::vector<Class>& Namespace::get_classes() {
        return classes;
    }

    const TypeName& Namespace::get_name() const {
        return name;
    }
    const std::vector<Variable>& Namespace::get_vars() const {
        return variables;
    }
    const std::vector<Alias>& Namespace::get_aliases() const {
        return aliases;
    }
    const std::vector<Function>& Namespace::get_functions() const {
        return functions;
    }
    const std::vector<Class>& Namespace::get_classes() const {
        return classes;
    }
}