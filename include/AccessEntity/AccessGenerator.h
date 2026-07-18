#pragma once

#include <DefaultEntity/DefaultGenerator.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <sstream>

namespace cg::generate {
    namespace cgs = cg::source;

    class FieldGenerator {
    public:
        static std::string generate(const cgs::Field& f, const cgs::Class& cls, GenStage g);
    };

    class MethodGenerator {
    public:
        static std::string generate(const cgs::Method& m, const cgs::Class& cls, GenStage g);
    };

    class ConstructorGenerator {
    public:
        static std::string generate(const cgs::Constructor& c, const cgs::Class& cls, GenStage g);
    };

    class DestructorGenerator {
    public:
        static std::string generate(const cgs::Destructor& d, const cgs::Class& cls, GenStage g);
    };

    class AliasGenerator {
    public:
        static std::string generate(const cgs::Alias& a);
    };

    class ClassGenerator {
    public:
        static std::string generate(const cgs::Class& cls, GenStage g);
    };
}

namespace cg::generate {
    // Может и странно но мне так нравится
    // Структура генерации такая:
    // 1. генерация приватных полей -> алиасов -> конструкторов
    // 2. генерация публичных полей -> алиасов -> конструкторов -> деструктора -> методы
    // 3. генерация протектных всех сущнойтей
    // 4. и только потом приватные методы
    inline std::string ClassGenerator::generate(const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;

        bool is_template_cls = !cls.get_template_parametrs().empty();
        if (is_template_cls) {
            sstr << TemplateEntityGenerator::generate(cls, false) << "\n";
        }

        if (g == GenStage::Declaration || g == GenStage::Inline) {
            sstr << "class " << cls.get_name();

            if (!cls.get_base_classes().empty()) {
                sstr << " : ";
                for (size_t i = 0; i < cls.get_base_classes().size(); i++) {
                    const auto& class_ = cls.get_base_classes()[i];
                    if (i > 0) sstr << ", ";
                    sstr << to_string(class_.get_visibility()) << " " << TypeNameGenerator::generate(class_);
                }
            }

            sstr << " {\n";
            std::stringstream body_sstr;

            auto generate_section = [&](cgs::Access target_access, bool only_methods) -> std::string {
                std::stringstream section_stream;
                bool need_newline = false;       // Флаг для отступа между группами
                bool prev_was_templated = false; // Флаг для отступа между шаблонами внутри одной группы
                bool is_first_in_group = true;   // Флаг начала текущей группы

                if (!only_methods) {
                    // Поля
                    is_first_in_group = true;
                    prev_was_templated = false;
                    for (auto& f : cls.get_fields()) {
                        if (f.get_visibility() == target_access) {
                            if (!is_first_in_group && (f.is_template() || prev_was_templated)) section_stream << "\n";
                            section_stream << FieldGenerator::generate(f, cls, g) << "\n";
                            need_newline = true;
                            is_first_in_group = false;
                            prev_was_templated = f.is_template();
                        }
                    }

                    // Алиасы
                    if (need_newline) { section_stream << "\n"; need_newline = false; }
                    is_first_in_group = true;
                    prev_was_templated = false;
                    for (auto& a : cls.get_aliases()) {
                        if (a.get_visibility() == target_access) {
                            bool is_tmpl = !a.get_template_parametrs().empty();
                            if (!is_first_in_group && (is_tmpl || prev_was_templated)) section_stream << "\n";
                            section_stream << AliasGenerator::generate(a) << "\n";
                            need_newline = true;
                            is_first_in_group = false;
                            prev_was_templated = is_tmpl;
                        }
                    }

                    // Конструкторы
                    if (need_newline) { section_stream << "\n"; need_newline = false; }
                    is_first_in_group = true;
                    prev_was_templated = false;
                    for (auto& c : cls.get_constructors()) {
                        if (c.get_visibility() == target_access) {
                            bool is_tmpl = !c.get_template_parametrs().empty();
                            if (!is_first_in_group && (is_tmpl || prev_was_templated)) section_stream << "\n";
                            section_stream << ConstructorGenerator::generate(c, cls, g) << "\n";
                            need_newline = true;
                            is_first_in_group = false;
                            prev_was_templated = is_tmpl;
                        }
                    }

                    // Деструктор
                    if (need_newline) { section_stream << "\n"; need_newline = false; }
                    if (cls.has_destructor() && cls.get_destructor().get_visibility() == target_access) {
                        section_stream << DestructorGenerator::generate(cls.get_destructor(), cls, g) << "\n";
                    }
                }

                // Методы
                is_first_in_group = true;
                prev_was_templated = false;
                for (auto& m : cls.get_methods()) {
                    if (m.get_visibility() == target_access) {
                        bool matches_context = only_methods ? (target_access == cgs::Access::Private) : (target_access != cgs::Access::Private);
                        if (matches_context) {
                            bool is_tmpl = !m.get_template_parametrs().empty();
                            if (!is_first_in_group && (is_tmpl || prev_was_templated)) section_stream << "\n";
                            section_stream << MethodGenerator::generate(m, cls, g) << "\n";
                            is_first_in_group = false;
                            prev_was_templated = is_tmpl;
                        }
                    }
                }

                return section_stream.str();
                };

            bool has_previous_section = false;

            auto append_section_to_body = [&](const std::string& name, const std::string& content) {
                if (content.empty()) return;
                if (has_previous_section) body_sstr << "\n";
                body_sstr << name << ":\n" << tabulate(1, content);
                has_previous_section = true;
                };

            append_section_to_body("private", generate_section(cgs::Access::Private, false));
            append_section_to_body("public", generate_section(cgs::Access::Public, false));
            append_section_to_body("protected", generate_section(cgs::Access::Protected, false));
            append_section_to_body("private", generate_section(cgs::Access::Private, true));

            sstr << body_sstr.str();
            sstr << "};";
        }
        else {
            bool need_group_newline = false;

            auto print_realiz_group = [&](const auto& entities, auto gen_func, auto is_tmpl_func) {
                bool generated = false;
                bool is_first = true;
                bool prev_was_templated = false;

                for (const auto& e : entities) {
                    std::string code = gen_func(e);
                    if (code.empty()) continue;

                    if (need_group_newline && !generated) {
                        sstr << "\n";
                        need_group_newline = false;
                    }

                    bool current_is_tmpl = is_tmpl_func(e);
                    if (!is_first && (current_is_tmpl || prev_was_templated)) {
                        sstr << "\n";
                    }

                    sstr << code << "\n";
                    generated = true;
                    is_first = false;
                    prev_was_templated = current_is_tmpl;
                }
                if (generated) need_group_newline = true;
                };

            print_realiz_group(cls.get_fields(),
                [&](auto& f) { return FieldGenerator::generate(f, cls, g); },
                [&](auto& f) { return is_template_cls && f.is_template(); });

            print_realiz_group(cls.get_constructors(),
                [&](auto& c) { return ConstructorGenerator::generate(c, cls, g); },
                [&](auto& c) { return is_template_cls && !c.get_template_parametrs().empty(); });

            if (cls.has_destructor()) {
                std::string dtor = DestructorGenerator::generate(cls.get_destructor(), cls, g);
                if (!dtor.empty()) {
                    if (need_group_newline) sstr << "\n";
                    sstr << dtor << "\n";
                    need_group_newline = true;
                }
            }

            print_realiz_group(cls.get_methods(),
                [&](auto& m) { return MethodGenerator::generate(m, cls, g); },
                [&](auto& m) { return is_template_cls && !m.get_template_parametrs().empty(); });
        }

        return sstr.str();
    }
    
    inline std::string AliasGenerator::generate(const cgs::Alias& a) {
        std::stringstream sstr;

        bool is_template = !a.get_template_parametrs().empty() || a.get_underlying_type().is_template();
        if (is_template) {
            sstr << TemplateEntityGenerator::generate(a) << "\n";
        }

        sstr << "using " << a.get_name() << " = " << TypeNameGenerator::generate(a.get_underlying_type()) << ";";

        return sstr.str();
    }

    inline std::string FieldGenerator::generate(const cgs::Field& f, const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;

        if (g == GenStage::Realization) {
            if (!f.is_static() || f.is_constexpr() || f.is_inline()) {
                return "";
            }

            bool class_is_template = !cls.get_template_parametrs().empty();
            if (class_is_template) {
                sstr << TemplateEntityGenerator::generate(cls, false) << "\n";
            }

            sstr << TypeNameGenerator::generate(f.get_type()) << " ";

            sstr << NamedGenerator::generate(cls)
                << TemplateEntityGenerator::generate(cls, true) << "::"
                << f.get_name();

            if (!f.get_value().empty()) {
                sstr << " = " << f.get_value();
            }
            sstr << ";";
            return sstr.str();
        }

        if (f.is_static()) {
            sstr << "static ";
        }
        if (f.is_constexpr()) {
            sstr << "constexpr ";
        }
        if (f.is_inline()) {
            sstr << "inline ";
        }
        if (f.is_mutable()) {
            sstr << "mutable ";
        }

        sstr << TypeNameGenerator::generate(f.get_type()) << " ";
        sstr << f.get_name();

        if (!f.get_value().empty()) {
            sstr << " = " << f.get_value();
        }

        sstr << ";";
        return sstr.str();
    }

    inline std::string MethodGenerator::generate(const cgs::Method& m, const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;

        bool is_template = !m.get_template_parametrs().empty() || m.get_type().is_template();
        bool class_is_template = !cls.get_template_parametrs().empty();

        if (g == GenStage::Realization && (is_template || class_is_template)) {
            auto m_inline = m;
            g = GenStage::Inline;
        }

        if (m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL ||
            m.get_method_type() == cgs::MethodType::DEFAULT ||
            m.get_method_type() == cgs::MethodType::DELETED)
        {
            if (g == GenStage::Realization) {
                return "";
            }
            g = GenStage::Declaration;
        }

        if (g != GenStage::Declaration) {
            if (class_is_template) {
                sstr << TemplateEntityGenerator::generate(cls, false) << "\n";
            }
        }
        if (is_template) {
            sstr << TemplateEntityGenerator::generate(m, false) << "\n";
        }

        if (g == GenStage::Declaration) {
            if (m.is_friend()) sstr << "friend ";
            if (m.is_static()) sstr << "static ";
            if (m.get_method_type() == cgs::MethodType::VIRTUAL ||
                m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << "virtual ";
            }
        }

        if (m.is_constexpr()) sstr << "constexpr ";
        if (m.is_inline() && g == GenStage::Inline) sstr << "inline ";

        sstr << TypeNameGenerator::generate(m.get_type()) << " ";

        if (g != GenStage::Declaration) {
            sstr << NamedGenerator::generate(cls)
                << TemplateEntityGenerator::generate(cls, true) << "::"
                << m.get_name();
        }
        else {
            sstr << m.get_name();
        }

        sstr << ArgumentableEntityGenerator::generate(m, g == GenStage::Declaration);

        if (m.is_const()) sstr << " const";

        if (g == GenStage::Declaration) {
            if (m.get_method_type() == cgs::MethodType::OVERRIDE) sstr << " override";

            if (m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << " = 0;";
            }
            else if (m.get_method_type() == cgs::MethodType::DEFAULT) {
                sstr << " = default;";
            }
            else if (m.get_method_type() == cgs::MethodType::DELETED) {
                sstr << " = delete;";
            }
            else if (m.is_inline() || m.is_constexpr()) {
                sstr << " {\n\t//TODO...\n}";
            }
            else {
                sstr << ";";
            }
        }
        else {
            sstr << " {\n\t//TODO...\n}";
        }

        return sstr.str();
    }

    inline std::string ConstructorGenerator::generate(const cgs::Constructor& c, const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;
        bool class_is_template = !cls.get_template_parametrs().empty();

        if (g == GenStage::Realization) {
            if (class_is_template || c.is_default() || c.is_delete()) return "";

            sstr << NamedGenerator::generate(cls) << "::" << cls.get_name();
            sstr << ArgumentableEntityGenerator::generate(c, false);

            auto& init = c.get_init_list();
            if (!init.empty()) {
                sstr << "\n\t: ";
                for (size_t i = 0; i < init.size(); ++i) {
                    if (i > 0) sstr << ", ";
                    sstr << init[i].get_name() << "(" << init[i].get_value() << ")";
                }
            }

            sstr << " {\n\t//TODO...\n}";
            return sstr.str();
        }

        if (c.is_explicit()) sstr << "explicit ";

        sstr << cls.get_name();
        sstr << ArgumentableEntityGenerator::generate(c, true);

        if (c.is_default()) {
            sstr << " = default;";
        }
        else if (c.is_delete()) {
            sstr << " = delete;";
        }
        else {
            sstr << ";";
        }

        return sstr.str();
    }

    inline std::string DestructorGenerator::generate(const cgs::Destructor& d, const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;
        bool class_is_template = !cls.get_template_parametrs().empty();

        if (g == GenStage::Realization) {
            if (class_is_template || d.is_default()) return "";

            sstr << NamedGenerator::generate(cls) << "::~" << cls.get_name() << "() {\n\t//TODO...\n}";
            return sstr.str();
        }

        if (d.is_virtual()) sstr << "virtual ";
        sstr << "~" << cls.get_name() << "()";

        if (d.is_default()) {
            sstr << " = default;";
        }
        else {
            sstr << ";";
        }

        return sstr.str();
    }
}