#pragma once

#include <DefaultEntity/DefaultGenerator.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <sstream>

namespace cg::generate {
    namespace cgs = cg::source;

    class FieldGenerator {
    public:
        static std::string generate(
        const cgs::Field& f, const cgs::Class& cls, bool realization = false);
    };

    class MethodGenerator {
    public:
        static std::string generate(
        const cgs::Method& m, const cgs::Class& cls, bool realization = false);
    };

    class ConstructorGenerator {
    public:
        static std::string generate(
        const cgs::Constructor& c, const cgs::Class& cls, bool realization = false);
    };

    class DestructorGenerator {
    public:
        static std::string generate(
        const cgs::Destructor& d, const cgs::Class& cls, bool realization = false);
    };

    class AliasGenerator {
    public:
        static std::string generate(const cgs::Alias& a);
    };

    class ClassGenerator {
    public:
        static std::string generate(
        const cgs::Class& cls, bool realization = false);
    };
}

namespace cg::generate {
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    inline std::string ClassGenerator::generate(
    const cgs::Class& cls, bool realization) {
        std::stringstream sstr;

        bool is_template_cls = !cls.get_template_parametrs().empty();
        if (is_template_cls) {
            sstr << TemplateEntityGenerator::generate(cls, false) << "\n";
        }

        if (!realization) {
            sstr << "class " << cls.get_name();

            // Генерация базовых классов
            if (!cls.get_base_classes().empty()) {
                sstr << " : ";
                for (size_t i = 0; i < cls.get_base_classes().size(); i++) {
                    const auto& class_ = cls.get_base_classes()[i];
                    if (i > 0) sstr << ", ";
                    sstr << to_string(class_.get_visibility()) << " " << TypeNameGenerator::generate(class_);
                }
            }

            sstr << " {\n";

            std::optional<cgs::Access> current_access = cgs::Access::Private;
            bool first_section = true;

            // Функция для смены области видимости
            auto switch_access = [&](cgs::Access new_access) {
                if (!current_access.has_value() || current_access.value() != new_access) {
                    if (!first_section) sstr << "\n";
                    sstr << to_string(new_access) << ":\n";
                    current_access = new_access;
                    first_section = false;
                }
                };

            // Генерация всех сущностей в порядке добавления
            for (const auto& entity : cls.get_entities()) {
                std::visit(overloaded{
                    [&](const cgs::Field& f) {
                        switch_access(f.get_visibility());
                        std::string code =
                            FieldGenerator::generate(f, cls, realization);
                        if (!code.empty()) {
                            sstr << tabulate(1, code) << "\n";
                        }
                    },
                    [&](const cgs::Alias& a) {
                        switch_access(a.get_visibility());
                        std::string code = AliasGenerator::generate(a);
                        if (!code.empty()) {
                            sstr << tabulate(1, code) << "\n";
                        }
                    },
                    [&](const cgs::Method& m) {
                        switch_access(m.get_visibility());
                        std::string code =
                            MethodGenerator::generate(m, cls, realization);
                        if (!code.empty()) {
                            sstr << tabulate(1, code) << "\n";
                        }
                    },
                    [&](const cgs::Constructor& c) {
                        switch_access(c.get_visibility());
                        std::string code =
                            ConstructorGenerator::generate(c, cls, realization);
                        if (!code.empty()) {
                            sstr << tabulate(1, code) << "\n";
                        }
                    },
                    [&](const cgs::Class& nested_cls) {
                        switch_access(nested_cls.get_visibility());
                        std::string code = generate(nested_cls, realization);
                        if (!code.empty()) {
                            sstr << tabulate(1, code) << "\n";
                        }
                    }
                    }, entity);
            }

            // Деструктор генерируется отдельно
            if (cls.has_destructor()) {
                const auto& d = cls.get_destructor();
                switch_access(d.get_visibility());
                std::string code = DestructorGenerator::generate(d, cls, realization);
                if (!code.empty()) {
                    sstr << tabulate(1, code) << "\n";
                }
            }

            sstr << "};";
        }
        else {
            bool first = true;

            // Реализация методов, конструкторов и деструктора
            for (const auto& entity : cls.get_entities()) {
                std::visit(overloaded{
                    [&](const cgs::Field& f) {
                        std::string code =
                            FieldGenerator::generate(f, cls, realization);
                        if (!code.empty()) {
                            if (!first) sstr << "\n";
                            sstr << code;
                            first = false;
                        }
                    },
                    [&](const cgs::Method& m) {
                        std::string code =
                            MethodGenerator::generate(m, cls, realization);
                        if (!code.empty()) {
                            if (!first) sstr << "\n";
                            sstr << code;
                            first = false;
                        }
                    },
                    [&](const cgs::Constructor& c) {
                        std::string code =
                            ConstructorGenerator::generate(c, cls, realization);
                        if (!code.empty()) {
                            if (!first) sstr << "\n";
                            sstr << code;
                            first = false;
                        }
                    },
                    [&](const cgs::Class& nested_cls) {
                        // Для вложенных классов реализация генерируется отдельно
                        std::string code = generate(nested_cls, realization);
                        if (!code.empty()) {
                            if (!first) sstr << "\n";
                            sstr << code;
                            first = false;
                        }
                    },
                    [&](const cgs::Alias& a) {
                        // Алиасы не имеют реализации
                    }
                    }, entity);
            }

            // Реализация деструктора
            if (cls.has_destructor()) {
                const auto& d = cls.get_destructor();
                std::string code = DestructorGenerator::generate(d, cls, realization);
                if (!code.empty()) {
                    if (!first) sstr << "\n";
                    sstr << code;
                    first = false;
                }
            }
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

    inline std::string FieldGenerator::generate(
    const cgs::Field& f, const cgs::Class& cls, bool realization) {
        std::stringstream sstr;

        if (realization) {
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

    inline std::string MethodGenerator::generate(
    const cgs::Method& m, const cgs::Class& cls, bool realization) {
        std::stringstream sstr;

        // Идет проверка возможности генерации при реализации
        // Если не проходит, то возвращаем пустую строку
        if (realization)
            // Если у метода нет определения, то он инлайн,
            // а инлайн генерируется только в генерации определения
            if (!m.has_definition() ||
                // Если метод абсолютно виртуальный или удален, то у него
                // нет ни тела, ни определения
                (
                    m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL ||
                    m.get_method_type() == cgs::MethodType::DELETED
                ) ||
                // Если метод инлайн или констекспр, то он инлайн по умолчанию
                (
                    m.is_inline() ||
                    m.is_constexpr()
                )
            ) return "";
        
        bool is_template = !m.get_template_parametrs().empty() || m.get_type().is_template();
        bool class_is_template = !cls.get_template_parametrs().empty();

        // Если это реализация, то есть вероятность что класс шаблонный,
        // а при определении вне объявления, расширение области требует шаблонов
        if (realization) {
            if (class_is_template) {
                sstr << TemplateEntityGenerator::generate(cls, false) << "\n";
            }
        }
        // Если метод шаблонный генерируем список шаблонов
        if (is_template) {
            sstr << TemplateEntityGenerator::generate(m, false) << "\n";
        }

        if (!realization) {
            if (m.is_friend()) sstr << "friend ";
            if (m.is_static()) sstr << "static ";
            if (m.get_method_type() == cgs::MethodType::VIRTUAL ||
                m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << "virtual ";
            }
            if (m.is_constexpr()) sstr << "constexpr ";
            if (m.is_inline()) sstr << "inline ";
        }

        sstr << TypeNameGenerator::generate(m.get_type()) << " ";

        if (realization) {
            sstr << NamedGenerator::generate(cls)
                << TemplateEntityGenerator::generate(cls, true) << "::"
                << m.get_name();
        }
        else {
            sstr << m.get_name();
        }

        sstr << ArgumentableEntityGenerator::generate(m, !realization);
        if (m.is_const()) sstr << " const";

        if (!realization) {
            if (m.get_method_type() == cgs::MethodType::OVERRIDE) sstr << " override";

            if (m.get_method_type() == cgs::MethodType::ABSOLUTE_VIRTUAL) {
                sstr << " = 0;";
            }
            else if (m.get_method_type() == cgs::MethodType::DELETED) {
                sstr << " = delete;";
            }
            else if (!m.has_definition()) {
                sstr << " {\n" << tabulate(1, "//TODO...") << "\n}";
            }
        }
        if (realization) {
            sstr << " {\n" << tabulate(1, "//TODO...") << "\n}";
        }
        else {
            sstr << ";";
        }

        return sstr.str();
    }

    inline std::string ConstructorGenerator::generate(
    const cgs::Constructor& c, const cgs::Class& cls, bool realization) {
        std::stringstream sstr;
        bool class_is_template = !cls.get_template_parametrs().empty();

        if (realization && c.has_definition()) {
            if (class_is_template || c.is_default() || c.is_delete()) return "";

            sstr << NamedGenerator::generate(cls) << "::" << cls.get_name();
            sstr << ArgumentableEntityGenerator::generate(c, false);

            auto& init = c.get_init_list();
            if (!init.empty()) {
                sstr << "\n" << tabulate(1, ": ");
                for (size_t i = 0; i < init.size(); ++i) {
                    if (i > 0) sstr << ", ";
                    sstr << init[i].get_name() << "(" << init[i].get_value() << ")";
                }
            }

            sstr << " {\n" << tabulate(1, "//TODO...") << "\n}";
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
        else if (!c.has_definition()) {
            sstr << " {\n" << tabulate(1, "//TODO...") << "\n}";
        }
        else {
            sstr << ";";
        }

        return sstr.str();
    }

    inline std::string DestructorGenerator::generate(
    const cgs::Destructor& d, const cgs::Class& cls, bool realization) {
        std::stringstream sstr;
        bool class_is_template = !cls.get_template_parametrs().empty();

        if (realization && d.has_definition()) {
            if (class_is_template || d.is_default()) return "";

            sstr << NamedGenerator::generate(cls) << "::~"
            << cls.get_name() << "() {\n" << tabulate(1, "//TODO...") << "\n }";
            return sstr.str();
        }

        if (d.is_virtual()) sstr << "virtual ";
        sstr << "~" << cls.get_name() << "()";

        if (d.is_default()) {
            sstr << " = default;";
        }
        else if (!d.has_definition()) {
            sstr << " {\n" << tabulate(1, "//TODO...") << "\n }";
        }
        else {
            sstr << ";";
        }

        return sstr.str();
    }
}