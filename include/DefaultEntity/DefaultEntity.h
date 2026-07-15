#pragma once

#include <string>
#include <sstream>
#include <format>
#include <vector>
#include <unordered_map>
#include <memory>

namespace cg::source {
    enum class DeclSpecifiers : uint8_t {
        None = 0,
        Const = 1 << 0,
        ConstExpression = 1 << 1,
        Static = 1 << 2,
        Inline = 1 << 3
    };

    inline DeclSpecifiers operator|(DeclSpecifiers a, DeclSpecifiers b) {
        return static_cast<DeclSpecifiers>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline bool operator&(DeclSpecifiers a, DeclSpecifiers b) {
        return (static_cast<int>(a) & static_cast<int>(b)) != 0;
    }

    inline DeclSpecifiers operator&=(DeclSpecifiers& a, DeclSpecifiers b) {
        a = static_cast<DeclSpecifiers>(static_cast<int>(a) & static_cast<int>(b));
        return a;
    }

    struct NamespacePrefix;

    class NamedEntity {
    protected:
        std::string name;
        std::vector<NamespacePrefix> namespace_prefix;

    public:
        NamedEntity(const std::string& name_)
        : name(name_) {}
        virtual ~NamedEntity() = default;

        const std::string& get_name() const { return name; }
        void set_name(const std::string& new_name) { name = new_name; }
        
        const std::vector<NamespacePrefix>& get_namespace() const { return namespace_prefix; }
        std::vector<NamespacePrefix>& get_namespace() { return namespace_prefix; }
        void add_namespace_prefix(const NamespacePrefix& prefix) { namespace_prefix.push_back(prefix); }

        NamedEntity(const NamedEntity&) = default;
        NamedEntity& operator=(const NamedEntity&) = default;
        NamedEntity(NamedEntity&&) = default;
        NamedEntity& operator=(NamedEntity&&) = default;
    };

    enum class Qualificator {
        None,
        Reference,
        UniversalReference,
        Pointer
    };

    class TypeName;

    class TemplateEntity {
        mutable std::vector<TypeName> template_parametrs;

    public:
        TemplateEntity() = default;
        TemplateEntity(TemplateEntity&&) = default;
        TemplateEntity& operator=(TemplateEntity&&) = default;
        TemplateEntity(const TemplateEntity&) = default;
        TemplateEntity& operator=(const TemplateEntity&) = default;

        std::vector<TypeName>& get_template_parametrs() { return template_parametrs; }
        const std::vector<TypeName>& get_template_parametrs() const { return template_parametrs; }

        void add_template_parametr(const TypeName& type) const { template_parametrs.push_back(type); }
    };

    class Alias;

    class TypeName : public NamedEntity, public TemplateEntity {
        std::unique_ptr<TypeName> type_name_ptr;
        Qualificator qual;
        bool constantable;
        bool variadic;
    
    public:
        TypeName(const std::string& name_ = "void")
        : NamedEntity(name_),
            constantable(false), variadic(false), qual(Qualificator::None)
        {}

        TypeName(const Alias& a);

        TypeName& operator=(const TypeName& other) {
            if (this != &other) {
                NamedEntity::operator=(other);
                TemplateEntity::operator=(other);
                qual = other.qual;
                constantable = other.constantable;
                variadic = other.variadic;
                type_name_ptr = other.type_name_ptr ? std::make_unique<TypeName>(*other.type_name_ptr) : nullptr;
            }
            return *this;
        }
        TypeName(const TypeName& other) : NamedEntity(other), TemplateEntity(other) {
            if (this != &other) {
                qual = other.qual;
                constantable = other.constantable;
                variadic = other.variadic;
                type_name_ptr = other.type_name_ptr ? std::make_unique<TypeName>(*other.type_name_ptr) : nullptr;
            }
        }

        TypeName(TypeName&&) = default;
        TypeName& operator=(TypeName&&) = default;

        bool is_const() const { return constantable; }
        void toggle_const() { constantable = !constantable; }

        bool is_variadic() const { return variadic; }
        void toggle_variadic() {
            if (!is_template()) throw std::runtime_error("[Error] this type cant be variadic. He is doesn't templateable");
            variadic = !variadic;
        }

        bool is_template() const { return type_name_ptr != nullptr; }
        void toggle_templatebale() {
            if (is_template()) {
                type_name_ptr.reset();
            }
            else {
                type_name_ptr = std::make_unique<TypeName>("typename");
            }
        }

        const TypeName& get_typename() const {
            if (!type_name_ptr) {
                throw std::runtime_error("Type name is not set");
            }
            return *type_name_ptr;
        }
        void set_typename(const TypeName& new_typename) {
            type_name_ptr = std::make_unique<TypeName>(new_typename);
        }

        void set_qualificator(const Qualificator& qualificator_) { qual = qualificator_; }
        Qualificator get_qualificator() const { return qual; }
    };

    struct NamespacePrefix {
        TypeName name;

        NamespacePrefix(const std::string& name_)
            : name(name_) {}
        NamespacePrefix(const char* name_)
            : name(std::string(name_)) {}
        NamespacePrefix(const TypeName& t)
            : name(t) {}
    };

    class Arguement : public NamedEntity {
        TypeName type;
        std::string value;

    public:
        Arguement(const std::string& name_)
            : NamedEntity(name_)
        {}

        Arguement(const Arguement&) = default;
        Arguement& operator=(const Arguement&) = default;

        TypeName& get_type() { return type; }
        const TypeName& get_type() const { return type; }

        const std::string& get_value() const { return value; }
        void set_value(const std::string& new_value) { value = new_value; }
    };

    class OptionalEntity : public NamedEntity {
        TypeName type;
        bool constexpr_;
        bool static_;
        bool inline_;

    public:
        OptionalEntity(const std::string& name_)
        : NamedEntity(name_),
        constexpr_(false),
        static_(false),
        inline_(false)
        {}

        OptionalEntity(const OptionalEntity&) = default;
        OptionalEntity& operator=(const OptionalEntity&) = default;

        bool is_constexpr() const { return constexpr_; }
        void toggle_constexpr() { constexpr_ = !constexpr_; }

        bool is_static() const { return static_; }
        void toggle_static() { static_ = !static_; }

        bool is_inline() const { return inline_; }
        void toggle_inline() { inline_ = !inline_; }

        TypeName& get_type() { return type; }
        const TypeName& get_type() const { return type; }
    };

    class Variable : public OptionalEntity {
        std::string value;

    public:
        Variable(const std::string& name_)
        : OptionalEntity(name_)
        {}

        const std::string& get_value() const { return value; }
        void set_value(const std::string& new_value) { value = new_value; }

        Variable(const Variable&) = default;
        Variable& operator=(const Variable&) = default;
        Variable(Variable&&) = default;
        Variable& operator=(Variable&&) = default;

        operator Arguement() const {
            auto arg = Arguement(name);
            arg.get_type() = get_type();
            arg.set_value(value);
            for (const auto& ns : namespace_prefix) {
                arg.add_namespace_prefix(ns);
            }
            return arg;
        }
    };

    class ArgumentableEntity {
        std::vector<Arguement> arguments;

    public:
        std::vector<Arguement>& get_args() { return arguments; }
        const std::vector<Arguement>& get_args() const { return arguments; }
    };

    class Function : public OptionalEntity, public ArgumentableEntity, public TemplateEntity {
    public:
        Function(const std::string& name_)
        : OptionalEntity(name_)
        {}

        Function(const Function&) = default;
        Function& operator=(const Function&) = default;
        Function(Function&&) = default;
        Function& operator=(Function&&) = default;
    };
}