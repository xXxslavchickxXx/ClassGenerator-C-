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
    inline std::string AliasGenerator::generate(const cgs::Alias& a) {
        std::stringstream sstr;

        bool is_template = !a.get_template_parametrs().empty() || a.get_underlying_type().is_template();
        if (is_template) {
            sstr << TemplateEntityGenerator::generate(a) << "\n";
        }

        sstr << "using " << a.get_name() << " = " << TypeNameGenerator::generate(a.get_underlying_type()) << ";";

        return sstr.str();
    }

    inline std::string ClassGenerator::generate(const cgs::Class& cls, GenStage g) {
        std::stringstream sstr;

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