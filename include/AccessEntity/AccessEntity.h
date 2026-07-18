#pragma once

#include <DefaultEntity/DefaultEntity.h>
#include <DefaultEntity/DefaultBuilder.h>

#include <algorithm>
#include <memory>
#include <iostream>
#include <optional>

namespace cg::source {
    namespace plb = cg::build;

	enum class Access {
		Public, Protected, Private
	};
	inline std::string to_string(Access a) {
		switch (a) {
		    case Access::Private:  return "private";
		    case Access::Protected: return "protected";
		    case Access::Public:   return "public";
            default: return "";
		}
	}

    class AccessEntity {
    protected:
        Access visibility;

    public:
        AccessEntity(const Access& access = Access::Private)
        : visibility(access) {}

        Access get_visibility() const { return visibility; }
        void set_visibility(Access vis) { visibility = vis; }
    };

    class Alias : public AccessEntity, public NamedEntity, public TemplateEntity {
        TypeName underlying_type;

    public:
        Alias(const std::string& name_)
            : NamedEntity(name_) {}

        const TypeName& get_underlying_type() const { return underlying_type; }
        void set_underlying_type(const TypeName& t) { underlying_type = t; }
    };

    inline TypeName::TypeName(const Alias& a)
        : TypeName(build::TypeBuilder(a.get_name()).build()) {}

    class Field : public Variable, public AccessEntity {
        bool mutablable;

    public:
        Field(const std::string& name_)
        : Variable(name_), mutablable(false) {}

        bool is_mutable() const { return mutablable; }
        void toggle_mutable() { mutablable = !mutablable; }

        bool is_template() const { return get_type().is_template(); }
    };

    enum class MethodType {
        NOTHING,
        DEFAULT,
        DELETED,
        VIRTUAL,
        ABSOLUTE_VIRTUAL,
        OVERRIDE
    };

    class Method : public Function, public AccessEntity {
        MethodType method_type;
        bool constantable;

    public:
        Method(const std::string& name_)
            : Function(name_),
            constantable(false),
            method_type(MethodType::NOTHING) {}

        MethodType get_method_type() const { return method_type; }
        void set_method_type(const MethodType& new_type) { method_type = new_type; }

        bool is_template() const {
            if (get_type().is_template()) return true;
            for (const auto& arg : get_args()) {
                if (arg.get_type().is_template()) return true;
            }
            return false;
        }

        bool is_const() const { return constantable; }
        void toggle_const() { constantable = !constantable; }
    };

    class Constructor : public AccessEntity, public ArgumentableEntity, public TemplateEntity {
        std::vector<Field> init_list;

        bool default_;
        bool delete_;
        bool explicit_;

    public:
        Constructor(const Access& access = Access::Public)
        : AccessEntity(access),
        default_(false),
        delete_(false),
        explicit_(false)
        {}

        void add_init(const Field& field) { init_list.push_back(field); }
        void erase_from_init(const std::string& field_name) {
            auto it = std::find_if(init_list.begin(), init_list.end(), [&](const Field& field_) {
                return field_.get_name() == field_name;
                });

            if (it == init_list.end()) {
                std::cerr << "entity doesn't exist in this constructor\n";
            }
            else {
                init_list.erase(it);
            }
        }

        std::vector<Field>& get_init_list() { return init_list; }
        const std::vector<Field>& get_init_list() const { return init_list; }

        bool is_explicit() const { return explicit_; }
        void toggle_explicit() { explicit_ = !explicit_; }

        bool is_default() const { return default_; }
        void toggle_default() { default_ = !default_; }

        bool is_delete() const { return delete_; }
        void toggle_delete() { delete_ = !delete_; }

        bool is_template() const { return !get_template_parametrs().empty(); }
    };

    class Destructor : public AccessEntity {
        bool default_;
        bool virtual_;

    public:
        Destructor(const Access& access = Access::Public)
        : AccessEntity(access),
        virtual_(true),
        default_(false)
         {}

        bool is_default() const { return default_; }
        void toggle_default() { default_ = !default_; }

        bool is_virtual() const { return virtual_; }
        void toggle_virtual() { virtual_ = !virtual_; }
    };

    class Class : public NamedEntity, public AccessEntity, public TemplateEntity {
        std::vector<Field> fields;
        std::vector<Method> methods;
        std::vector<Alias> aliases;

        std::vector<Constructor> constructors;
        std::optional<Destructor> destructor;

        std::vector<Class> base_classes;

    public:
        Class(const std::string& name_) : NamedEntity(name_) {}

        Class(Class&&) noexcept = default;
        Class& operator=(Class&&) noexcept = default;
        Class(const Class&) = default;
        Class& operator=(const Class&) = default;

        operator TypeName() const {
            auto type = plb::TypeBuilder(name)
                .ns(get_namespace())
                .with_template(get_template_parametrs())
                .build();

            return type;
        }

        std::vector<Constructor>& get_constructors() { return constructors; }
        const std::vector<Constructor>& get_constructors() const { return constructors; }
        
        Destructor& get_destructor() {
            if (!destructor) {
                throw std::runtime_error("Class has no destructor");
            }
            return destructor.value();
        }
        const Destructor& get_destructor() const {
            if (!destructor) {
                throw std::runtime_error("Class has no destructor");
            }
            return destructor.value();
        }
        void set_destructor(const Destructor& dtor) { destructor = dtor; }
        bool has_destructor() const { return destructor.has_value(); }

        void add_field(const Field& v) { fields.push_back(v); }
        void add_method(const Method& m) { methods.push_back(m); }
        void add_alias(const Alias& a) { aliases.push_back(a); }
        void add_constructor(const Constructor& c) { constructors.push_back(c); }
        void add_base_class(const Class& base) { base_classes.push_back(base); }

        const std::vector<Class>& get_base_classes() const { return base_classes; }
        const std::vector<Field>& get_fields() const { return fields; }
        const std::vector<Method>& get_methods() const { return methods; }
        const std::vector<Alias>& get_aliases() const { return aliases; }

        Field& get_field(const std::string& name) { 
            auto it = std::find_if(fields.begin(), fields.end(), [&name](const Field& field) {
                return field.get_name() == name;
            });

            if (it == fields.end()) {
                throw std::runtime_error("this variable doesn't exist");
            }

            return (*it);
        }
        const Field& get_field(const std::string& name) const {
            auto it = std::find_if(fields.begin(), fields.end(), [&name](const Field& field) {
                return field.get_name() == name;
                });

            if (it == fields.end()) {
                throw std::runtime_error("this variable doesn't exist");
            }

            return (*it);
        }
        Method& get_method(const std::string& name) {
            auto it = std::find_if(methods.begin(), methods.end(), [&name](const Method& method) {
                return method.get_name() == name;
            });

            if (it == methods.end()) {
                throw std::runtime_error("this method doesn't exist");
            }

            return (*it);
        }
        const Method& get_method(const std::string& name) const {
            auto it = std::find_if(methods.begin(), methods.end(), [&name](const Method& method) {
                return method.get_name() == name;
            });

            if (it == methods.end()) {
                throw std::runtime_error("this method doesn't exist");
            }

            return (*it);
        }

    };
}