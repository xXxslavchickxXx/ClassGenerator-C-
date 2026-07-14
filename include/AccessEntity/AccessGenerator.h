#pragma once

#include <AccessEntity/AccessEntity.h>
#include <DefaultEntity/DefaultEntity.h>
#include <DefaultEntity/DefaultGenerator.h>
#include <sstream>
#include <string>

namespace cg::generate {
    namespace cgs = cg::source;

    class AccessEntityGenerator {
    public:
        static std::string generate_access(const cgs::AccessEntity& entity) {
            switch (entity.get_visibility()) {
            case cgs::Access::Public:    return "public";
            case cgs::Access::Protected: return "protected";
            case cgs::Access::Private:   return "private";
            default: return "";
            }
        }

        static std::string generate_access_label(const cgs::AccessEntity& entity) {
            auto access = generate_access(entity);
            if (access.empty()) return "";
            return access + ":";
        }
    };

    class AliasGenerator {
    public:
        static std::string generate(const cgs::Alias& alias) {
            std::stringstream sstr;

            sstr << TemplateEntityGenerator::generate(alias, false);
            if (!alias.get_template_parametrs().empty()) sstr << "\n";

            sstr << "using " << NamedGenerator::generate(alias) << " = ";
            sstr << TypeNameGenerator::generate(alias.get_underlying_type());

            return sstr.str();
        }
    };

    class FieldGenerator {
    public:
        static std::string generate(const cgs::Field& field) {
            std::stringstream sstr;

            if (field.is_mutable()) sstr << "mutable ";

            sstr << VariableGenerator::generate(field);

            return sstr.str();
        }
    };

    class MethodGenerator {
    public:
        static std::string generate(const cgs::Method& method) {
            std::stringstream sstr;

            sstr << TemplateEntityGenerator::generate(method, false);
            if (!method.get_template_parametrs().empty()) sstr << "\n";

            if (method.get_method_type() == cgs::MethodType::VIRTUAL ||
                method.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << "virtual ";
            }

            sstr << OptionalEntityGenerator::generate(method)
                << ArgumentableEntityGenerator::generate(method);

            if (method.is_const()) {
                sstr << " const";
            }

            if (method.get_method_type() == cgs::MethodType::OVERRIDE) {
                sstr << " override";
            }

            if (method.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << " = 0";
            }

            if (method.get_method_type() == cgs::MethodType::DEFAULT) {
                sstr << " = default";
            }
            else if (method.get_method_type() == cgs::MethodType::DELETED) {
                sstr << " = delete";
            }

            return sstr.str();
        }
    };

    class ConstructorGenerator {
    public:
        static std::string generate(const cgs::Constructor& ctor, const cgs::Class& class_) {
            std::stringstream sstr;

            if (ctor.is_explicit()) {
                sstr << "explicit ";
            }

            sstr << TemplateEntityGenerator::generate(ctor, false);
            if (!ctor.get_template_parametrs().empty()) sstr << "\n";

            sstr << class_.get_name();
            sstr << ArgumentableEntityGenerator::generate(ctor);

            if (ctor.is_default()) {
                sstr << " = default";
            }
            else if (ctor.is_delete()) {
                sstr << " = delete";
            }

            const auto& init_list = ctor.get_init_list();
            if (!init_list.empty()) {
                sstr << " : ";
                for (size_t i = 0; i < init_list.size(); i++) {
                    if (i > 0) sstr << ", ";
                    sstr << init_list[i].get_name() << "(" << init_list[i].get_value() << ")";
                }
            }

            return sstr.str();
        }
    };

    class DestructorGenerator {
    public:
        static std::string generate(const cgs::Destructor& dtor, const cgs::Class& class_) {
            std::stringstream sstr;

            if (dtor.is_virtual()) {
                sstr << "virtual ";
            }

            sstr << "~" << class_.get_name() << "()";

            if (dtor.is_default()) {
                sstr << " = default";
            }

            return sstr.str();
        }
    };

    class ClassGenerator {
    public:
        static std::string generate(const cgs::Class& cls) {
            std::stringstream sstr;

            sstr << TemplateEntityGenerator::generate(cls, false);
            if (!cls.get_template_parametrs().empty()) sstr << "\n";

            sstr << "class " << cls.get_name();

            const auto& bases = cls.get_base_classes();
            if (!bases.empty()) {
                sstr << " : ";
                for (size_t i = 0; i < bases.size(); i++) {
                    if (i > 0) sstr << ", ";
                    sstr << AccessEntityGenerator::generate_access(bases[i]) << " ";
                    sstr << bases[i].get_name();
                }
            }

            sstr << " {\n";

            std::vector<cgs::Access> access_order = {
                cgs::Access::Public,
                cgs::Access::Protected,
                cgs::Access::Private
            };

            for (auto access : access_order) {
                bool has_entities = false;

                for (const auto& field : cls.get_fields()) {
                    if (field.get_visibility() == access) { has_entities = true; break; }
                }
                if (!has_entities) {
                    for (const auto& method : cls.get_methods()) {
                        if (method.get_visibility() == access) { has_entities = true; break; }
                    }
                }
                if (!has_entities) {
                    for (const auto& alias : cls.get_aliases()) {
                        if (alias.get_visibility() == access) { has_entities = true; break; }
                    }
                }
                if (!has_entities && cls.has_destructor()) {
                    if (cls.get_destructor().get_visibility() == access) { has_entities = true; }
                }
                if (!has_entities) {
                    for (const auto& ctor : cls.get_constructors()) {
                        if (ctor.get_visibility() == access) { has_entities = true; break; }
                    }
                }

                if (!has_entities) continue;

                sstr << cgs::to_string(access) << ":\n";

                for (const auto& alias : cls.get_aliases()) {
                    if (alias.get_visibility() == access) {
                        sstr << tabulate(1, AliasGenerator::generate(alias)) << ";\n";
                    }
                }

                for (const auto& field : cls.get_fields()) {
                    if (field.get_visibility() == access) {
                        sstr << tabulate(1, FieldGenerator::generate(field)) << ";\n";
                    }
                }

                for (const auto& ctor : cls.get_constructors()) {
                    if (ctor.get_visibility() == access) {
                        sstr << tabulate(1, ConstructorGenerator::generate(ctor, cls)) << ";\n";
                    }
                }

                if (cls.has_destructor() && cls.get_destructor().get_visibility() == access) {
                    sstr << tabulate(1, DestructorGenerator::generate(cls.get_destructor(), cls)) << ";\n";
                }

                for (const auto& method : cls.get_methods()) {
                    if (method.get_visibility() == access) {
                        sstr << tabulate(1, MethodGenerator::generate(method)) << ";\n";
                    }
                }

                sstr << "\n";
            }

            sstr << "};";

            return sstr.str();
        }
    };
}