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
                    sstr << to_string(class_.get_visibility())
                        << " " << TypeNameGenerator::generate(class_);
                }
            }

            sstr << " {\n";

            std::stringstream body_sstr;

            std::stringstream priv_top;
            for (auto& f : cls.get_fields())
                if (f.get_visibility() == cgs::Access::Private) priv_top << FieldGenerator::generate(f, cls, g) << "\n";
            for (auto& a : cls.get_aliases()) 
                if (a.get_visibility() == cgs::Access::Private) priv_top << AliasGenerator::generate(a) << "\n";
            for (auto& c : cls.get_constructors())
                if (c.get_visibility() == cgs::Access::Private) priv_top << ConstructorGenerator::generate(c, cls, g) << "\n";
            if (cls.has_destructor() && cls.get_destructor().get_visibility() == cgs::Access::Private) {
                priv_top << DestructorGenerator::generate(cls.get_destructor(), cls, g) << "\n";
            }
            if (!priv_top.str().empty()) body_sstr << "private:\n" << tabulate(1, priv_top.str());

            std::stringstream pub;
            for (auto& f : cls.get_fields())
                if (f.get_visibility() == cgs::Access::Public) pub << FieldGenerator::generate(f, cls, g) << "\n";
            for (auto& a : cls.get_aliases())
                if (a.get_visibility() == cgs::Access::Public) pub << AliasGenerator::generate(a) << "\n";
            for (auto& c : cls.get_constructors())
                if (c.get_visibility() == cgs::Access::Public) pub << ConstructorGenerator::generate(c, cls, g) << "\n";
            if (cls.has_destructor() && cls.get_destructor().get_visibility() == cgs::Access::Public) {
                pub << DestructorGenerator::generate(cls.get_destructor(), cls, g) << "\n";
            }
            for (auto& m : cls.get_methods())
                if (m.get_visibility() == cgs::Access::Public) pub << MethodGenerator::generate(m, cls, g) << "\n";
            if (!pub.str().empty()) body_sstr << "public:\n" << tabulate(1, pub.str());

            std::stringstream prot;
            for (auto& a : cls.get_aliases())
                if (a.get_visibility() == cgs::Access::Protected) prot << AliasGenerator::generate(a) << "\n";
            for (auto& f : cls.get_fields())
                if (f.get_visibility() == cgs::Access::Protected) prot << FieldGenerator::generate(f, cls, g) << "\n";
            for (auto& c : cls.get_constructors())
                if (c.get_visibility() == cgs::Access::Protected) prot << ConstructorGenerator::generate(c, cls, g) << "\n";
            if (cls.has_destructor() && cls.get_destructor().get_visibility() == cgs::Access::Protected) {
                prot << DestructorGenerator::generate(cls.get_destructor(), cls, g) << "\n";
            }
            for (auto& m : cls.get_methods())
                if (m.get_visibility() == cgs::Access::Protected) prot << MethodGenerator::generate(m, cls, g) << "\n";
            if (!prot.str().empty()) body_sstr << "protected:\n" << tabulate(1, prot.str());

            std::stringstream priv_bot;
            for (auto& m : cls.get_methods())
                if (m.get_visibility() == cgs::Access::Private) priv_bot << MethodGenerator::generate(m, cls, g) << "\n";
            if (!priv_bot.str().empty()) body_sstr << "private:\n" << tabulate(1, priv_bot.str());
           
            sstr << body_sstr.str();
            sstr << "};";
        }
        else {
            for (auto& field : cls.get_fields()) {
                bool is_template = is_template_cls && field.is_template();
                sstr << FieldGenerator::generate(field, cls, g) << (is_template ? "\n\n" : "\n");
            }

            sstr << "\n";

            for (auto& constructor : cls.get_constructors()) {
                bool is_template = is_template_cls && constructor.is_template();
                sstr << ConstructorGenerator::generate(constructor, cls, g) << (is_template ? "\n\n" : "\n");
            }

            sstr << "\n";

            if (cls.has_destructor()) {
                bool is_template = is_template_cls && cls.get_destructor().is_template();
                sstr << DestructorGenerator::generate(cls.get_destructor(), cls, g) << (is_template ? "\n\n" : "\n");
            }

            sstr << "\n";

            for (auto& method : cls.get_methods()) {
                bool is_template = is_template_cls && method.is_template();
                sstr << MethodGenerator::generate(method, cls, g) << (is_template ? "\n\n" : "\n");
            }

            sstr << "\n";
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