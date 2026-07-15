#include <ClassGen.h>
#include <iostream>

cg::source::Class build_base_proxy() {
    using namespace cg::build;

    // ============================================================
    // Шаблонные параметры
    // ============================================================
    auto attr_t = TypeBuilder("AttributeVectorT")
        .as_template()
        .build();

    auto const_t = TypeBuilder("IsConst")
        .as_template(TypeBuilder("bool").build())
        .build();

    auto tags_t = TypeBuilder("SelectedTags")
        .as_template()
        .as_variadic()
        .build();

    auto tag_t = TypeBuilder("Tag")
        .as_template()
        .build();

    // ============================================================
    // Типы для алиасов
    // ============================================================
    auto vec_type = TypeBuilder("vec_type")
        .with_template(tag_t)
        .build();

    auto pointer_type = TypeBuilder("PointerType")
        .build();

    // ============================================================
    // Класс base_proxy
    // ============================================================
    auto cls = ClassBuilder("base_proxy")
        .as_public()
        .add_template_parametr(attr_t)
        .add_template_parametr(const_t)
        .add_template_parametr(tags_t)
        .build();

    // ============================================================
    // Алиасы (нужна доработка в билдере)
    // ============================================================
    // using vec_type = typename AttributeVectorT::template vec_type<typename Tag::type>;
    // using DataType = typename AttributeVectorT::data_type;
    // using PointerType = std::conditional_t<IsConst, const DataType*, DataType*>;

    // Временное решение — добавим как поля, потом заменим на алиасы
    auto data_type_alias = AliasBuilder("DataType")
        .underlying_type(
            TypeBuilder("data_type")
            .ns(cg::source::NamespacePrefix("AttributeVectorT"))
            .build()
        )
        .as_public()
        .build();
    cls.add_alias(data_type_alias);

    auto pointer_type_alias = AliasBuilder("PointerType")
        .underlying_type(
            TypeBuilder("PointerType")
            .build()
        )
        .as_public()
        .build();
    cls.add_alias(pointer_type_alias);

    // ============================================================
    // Поле data_
    // ============================================================
    auto data_field = FieldBuilder("data_")
        .with_type(
            TypeBuilder("PointerType")
            .as_ptr()
            .build()
        )
        .as_protected()
        .build();
    cls.add_field(data_field);

    // ============================================================
    // Конструктор
    // ============================================================
    auto ctor = ConstructorBuilder()
        .as_public()
        .add_argument(
            VariableBuilder("data")
            .with_type(
                TypeBuilder("PointerType")
                .as_ptr()
                .build()
            )
            .build()
        )
        .build();
    cls.add_constructor(ctor);

    // ============================================================
    // Метод vector()
    // ============================================================
    auto vec_method = MethodBuilder("vector")
        .with_type(
            TypeBuilder(vec_type)
            .as_const()
            .as_ref()
            .build()
        )
        .as_public()
        .add_template_parametr(tag_t)
        .build();
    cls.add_method(vec_method);

    // ============================================================
    // Метод mutable_vector()
    // ============================================================
    auto mut_vec_method = MethodBuilder("mutable_vector")
        .with_type(
            TypeBuilder(vec_type)
            .as_ref()
            .build()
        )
        .as_public()
        .add_template_parametr(tag_t)
        .build();
    cls.add_method(mut_vec_method);

    // ============================================================
    // Метод size()
    // ============================================================
    auto size_method = MethodBuilder("size")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(size_method);

    // ============================================================
    // Метод capacity()
    // ============================================================
    auto capacity_method = MethodBuilder("capacity")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(capacity_method);

    // ============================================================
    // Метод empty()
    // ============================================================
    auto empty_method = MethodBuilder("empty")
        .with_type(TypeBuilder("bool").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(empty_method);

    // ============================================================
    // Метод get_tag_index()
    // ============================================================
    auto idx_method = MethodBuilder("get_tag_index")
        .with_type(TypeBuilder("size_t").build())
        .as_protected()
        .as_constexpr()
        .as_const()
        .add_template_parametr(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .build();
    cls.add_method(idx_method);

    // ============================================================
    // Метод call()
    // ============================================================
    auto call_method = MethodBuilder("call")
        .with_type(TypeBuilder("void").build())
        .as_protected()
        .as_constexpr()
        .as_inline()
        .add_template_parametr(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .add_template_parametr(
            TypeBuilder("F")
            .as_template()
            .build()
        )
        .add_argument(
            VariableBuilder("f")
            .with_type(
                TypeBuilder("F")
                .as_uni_ref()
                .build()
            )
            .build()
        )
        .build();
    cls.add_method(call_method);

    return cls;
}

auto build_vec_type() {
    auto tag_t = cg::build::TypeBuilder("Tag")
        .as_template()
        .build();
    auto type_t = cg::build::TypeBuilder("type")
        .ns(tag_t)
        .build();
    auto attr_t = cg::build::TypeBuilder("AttributeVectorT")
        .as_template()
        .build();
    auto vec_t = cg::build::TypeBuilder("vec_type")
        .with_template(type_t)
        .ns(attr_t)
        .build();

    return vec_t;
}

auto build_pointer_type() {
    auto Data_t = cg::build::TypeBuilder("DataType")
        .as_ptr()
        .build();
    auto c_Data_t = cg::build::TypeBuilder(Data_t)
        .as_const()
        .build();

    auto IsConst_t = cg::build::TypeBuilder("IsConst")
        .as_template(cg::build::TypeBuilder("bool").build())
        .build();

    auto cond_t = cg::build::TypeBuilder("conditional_t")
        .with_template(IsConst_t)
        .with_template(c_Data_t)
        .with_template(Data_t)
        .ns("std")
        .build();

    return cond_t;
}

auto build_data_type() {
    auto attr_t = cg::build::TypeBuilder("AttributeVectorT")
        .as_template()
        .build();
    auto data_t = cg::build::TypeBuilder("data_type")
        .ns(attr_t)
        .build();

    return data_t;
}

void test_variable_generation() {
    using namespace cg::build;
    using namespace cg::generate;

    auto check = [](const std::string& actual, const std::string& expected, const std::string& test_name) {
        if (actual != expected) {
            std::cerr << "Failed: " << test_name << "\n";
            std::cerr << "  Expected: \"" << expected << "\"\n";
            std::cerr << "  Got:      \"" << actual << "\"\n";
            std::abort();
        }
        std::cout << "Passed: " << test_name << "\n";
        };
    std::cout << "\tVariable generator:\n";

    {
        auto var = VariableBuilder("global_counter")
            .with_type(TypeBuilder("int").build())
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "extern int global_counter", "1. Declaration (global)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "int global_counter", "1. Realization (global)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "int global_counter", "1. Inline (global)");
    }

    {
        auto var = VariableBuilder("global_counter")
            .with_type(TypeBuilder("int").build())
            .with_value("10")
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "extern int global_counter = 10", "2. Declaration (global init)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "int global_counter = 10", "2. Realization (global init)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "int global_counter = 10", "2. Inline (global init)");
    }

    {
        auto var = VariableBuilder("static_counter")
            .with_type(TypeBuilder("int").build())
            .as_static()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "static int static_counter", "3. Declaration (static)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "3. Realization (static - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "static int static_counter", "3. Inline (static)");
    }

    {
        auto var = VariableBuilder("MAX_SIZE")
            .with_type(TypeBuilder("int").build())
            .with_value("100")
            .as_static()
            .as_constexpr()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "static constexpr int MAX_SIZE = 100", "4. Declaration (static constexpr)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "4. Realization (static constexpr - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "static constexpr int MAX_SIZE = 100", "4. Inline (static constexpr)");
    }

    {
        auto var = VariableBuilder("inline_counter")
            .with_type(TypeBuilder("int").build())
            .with_value("42")
            .as_inline()
            .as_static()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "static inline int inline_counter = 42", "5. Declaration (inline static)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "5. Realization (inline static - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "static inline int inline_counter = 42", "5. Inline (inline static)");
    }

    {
        auto var = VariableBuilder("PI")
            .with_type(TypeBuilder("float").build())
            .with_value("3.14159f")
            .as_constexpr()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "constexpr float PI = 3.14159f", "6. Declaration (constexpr global)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "6. Realization (constexpr global - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "constexpr float PI = 3.14159f", "6. Inline (constexpr global)");
    }

    {
        auto var = VariableBuilder("VERSION")
            .with_type(TypeBuilder("int").build())
            .with_value("1")
            .as_static()
            .as_constexpr()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "static constexpr int VERSION = 1", "7. Declaration (static constexpr init)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "7. Realization (static constexpr init - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "static constexpr int VERSION = 1", "7. Inline (static constexpr init)");
    }

    {
        auto var = VariableBuilder("DEFAULT_VALUE")
            .with_type(TypeBuilder("int").build())
            .with_value("0")
            .as_constexpr()
            .as_inline()
            .as_static()
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "static constexpr inline int DEFAULT_VALUE = 0", "8. Declaration (inline static constexpr)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "8. Realization (inline static constexpr - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "static constexpr inline int DEFAULT_VALUE = 0", "8. Inline (inline static constexpr)");
    }

    {
        auto var = VariableBuilder("app_version")
            .with_type(TypeBuilder("int").build())
            .with_value("1")
            .ns(cg::source::NamespacePrefix("myapp"))
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "extern int myapp::app_version = 1", "9. Declaration (namespace)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "int myapp::app_version = 1", "9. Realization (namespace)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "int myapp::app_version = 1", "9. Inline (namespace)");
    }

    {
        auto var = VariableBuilder("MAX_CONNECTIONS")
            .with_type(TypeBuilder("int").build())
            .with_value("1024")
            .as_constexpr()
            .ns(cg::source::NamespacePrefix("network"))
            .build();

        check(VariableGenerator::generate(var, GenStage::Declaration),
            "constexpr int network::MAX_CONNECTIONS = 1024", "10. Declaration (constexpr namespace)");
        check(VariableGenerator::generate(var, GenStage::Realization),
            "", "10. Realization (constexpr namespace - skip)");
        check(VariableGenerator::generate(var, GenStage::Inline),
            "constexpr int network::MAX_CONNECTIONS = 1024", "10. Inline (constexpr namespace)");
    }
}

int main() {
    system("chcp 65001 > nul");
    test_variable_generation();
    return 0;
}