#include <gtest/gtest.h>
#include <stubs.h>
#include <format>

#include <core/entities/cpp/entites.h>
#include <generation/generators/cpp/CppGenerators.h>
#include <generation/dispatchers/RegisteredDispatcher.h>

TEST(CPPGen, RelativeFunc) {
    // Empty Node
    EXPECT_EQ(cg::gen::cpp::relative_path(nullptr), "");

    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("external");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
   
    auto* main_ptr = &main;
    auto* ext_ptr = main.take(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    EXPECT_EQ(cg::gen::cpp::relative_path(main_ptr), "main");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr), "main::external");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr, main_ptr), "external");
    EXPECT_TRUE(cg::gen::cpp::relative_path(main_ptr, main_ptr).empty());

    // Path without NamedEntity (catching exception)
    auto node_without_entities = cg::core::Node();
    EXPECT_EQ(cg::gen::cpp::relative_path(&node_without_entities), "undefined");
}

TEST(CPPGen, NamespaceGen) {
    ///

    /// TODO... В перспективе неймспейсы должны генерироваться только в том случае, если
    /// содержимое неймспейса будет что-то содержать (сгенерированное)

    ///
    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("nested");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
    auto* main_ptr = &main; // Для удобства
    auto* ext_ptr = main.take(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    auto dispatcher = cg::RegDispatcherFtor<cg::gen::cpp::NamespaceGenerator>();

    EXPECT_EQ(dispatcher.generate(main_ptr),
R"(namespace main {
    namespace nested {
    
    }
})"
    );
    EXPECT_EQ(dispatcher.generate(ext_ptr), "namespace main::nested {\n\n}");
    EXPECT_EQ(dispatcher.generate(ext_ptr, false, main_ptr), "namespace nested {\n\n}");
    EXPECT_EQ(dispatcher.generate(main_ptr, false, main_ptr), "namespace nested {\n\n}");
}

TEST(CPPGen, ClassGenWithoutTemplates) {
    // namespace main
    //            ├── namespace nested
    //            │               └── class External
    //            │                            └── class Nested
    //            └── class Main

    auto main_namespace_name = cg::entities::cpp::NamedEntity("main");
    auto nested_namespace_name = cg::entities::cpp::NamedEntity("nested");
    
    auto main_class_name = cg::entities::cpp::NamedEntity("Main");
    auto ExternalClass_name = cg::entities::cpp::NamedEntity("External");
    auto Nested_name = cg::entities::cpp::NamedEntity("Nested");

    // Creating main namespace and add external name into him
    auto main_ns = cg::entities::cpp::Namespace(main_namespace_name);
    auto* main_ptr = &main_ns;

    // main childs
    auto* nested_ns_ptr = main_ns.take(std::make_unique<cg::entities::cpp::Namespace>(nested_namespace_name));
    auto* Main_cls_ptr = main_ns.take(std::make_unique<cg::entities::cpp::Class>(main_class_name));
    
    // nested child
    auto* ext_cls_ptr = nested_ns_ptr->take(std::make_unique<cg::entities::cpp::Class>(ExternalClass_name));
    auto* nested_cls_ptr = ext_cls_ptr->take(std::make_unique<cg::entities::cpp::Class>(Nested_name));

    auto dispatcher = cg::RegDispatcherFtor<
        cg::gen::cpp::ClassGenerator,
        cg::gen::cpp::NamespaceGenerator
    >();

    /// Validate all node generation
    // namespace main
    EXPECT_EQ(dispatcher.generate(main_ptr),
R"(namespace main {
    namespace nested {
        class External {
            class Nested {
            
            };
        };
    }
    class Main {
    
    };
})"
    );
    // namespace nested
    EXPECT_EQ(dispatcher.generate(nested_ns_ptr),
R"(namespace main::nested {
    class External {
        class Nested {
        
        };
    };
})"
    );
    // class Main_cls_ptr
    EXPECT_EQ(dispatcher.generate(Main_cls_ptr),
R"(class main::Main {

};)"
    );
    // class ext_cls_ptr
    EXPECT_EQ(dispatcher.generate(ext_cls_ptr),
R"(class main::nested::External {
    class Nested {
    
    };
};)"
    );
    // class nested_cls_ptr
    EXPECT_EQ(dispatcher.generate(nested_cls_ptr),
R"(class main::nested::External::Nested {

};)"
    );

    /// Context generations
    // Validate generations with context
    EXPECT_EQ(dispatcher.generate(main_ptr, true, main_ptr),
R"(namespace nested {
    class External {
        class Nested {
        
        };
    };
}
class Main {

};)"
    );
}

TEST(CPPGen, TypeAndAliasGeneration) {
    // namespace std
    //            └── namespace filesystem
    //                            └── class path
    //                                       └── using const_iterator
    // class int
    using namespace cg::entities::cpp;

    auto std_ns = Namespace(NamedEntity("std"));
    auto* fs = std_ns.create_child<Namespace>(NamedEntity("filesystem"));
    auto* path = fs->create_child<Class>(NamedEntity("path"));
    
    auto int_cls = Class(NamedEntity("int"));

    // just a int type
    auto type_int = Type(ClassHandler(&int_cls));

    // just test
    auto* const_itertor = path->create_child<Alias>(NamedEntity("const_iterator"), TypeHandler(&type_int));

    // const int
    auto const_int = Type(ClassHandler(&int_cls), TypeQualificator().toggle_const());

    // const int*
    auto const_int_ptr = Type(ClassHandler(&int_cls),
                              TypeQualificator().toggle_const(),
                              TypeConstructor().toggle_to_pointer());

    // const int* const
    auto const_int_ptr_const = Type(ClassHandler(&int_cls),
                                    TypeQualificator().toggle_const(),
                                    TypeConstructor().toggle_to_pointer().toggle_const());

    auto dispatcher = cg::RegDispatcherFtor<
        cg::gen::cpp::TypeGenerator,
        cg::gen::cpp::AliasGenerator
    >();

    EXPECT_EQ(dispatcher.generate(&type_int), "int");
    EXPECT_EQ(dispatcher.generate(&const_int), "const int");
    EXPECT_EQ(dispatcher.generate(&const_int_ptr), "const int*");
    EXPECT_EQ(dispatcher.generate(&const_int_ptr_const), "const int* const");

    // just a std::filesystem::path type
    auto type_path = Type(ClassHandler(path));

    // const std::filesystem::path
    auto const_path = Type(ClassHandler(path), TypeQualificator().toggle_const());

    // const std::filesystem::path*
    auto const_path_ptr = Type(ClassHandler(path),
                              TypeQualificator().toggle_const(),
                              TypeConstructor().toggle_to_pointer());

    // const std::filesystem::path* const
    auto const_path_ptr_const = Type(ClassHandler(path),
                                    TypeQualificator().toggle_const(),
                                    TypeConstructor().toggle_to_pointer().toggle_const());
    
    EXPECT_EQ(dispatcher.generate(&type_path), "std::filesystem::path");
    EXPECT_EQ(dispatcher.generate(&const_path), "const std::filesystem::path");
    EXPECT_EQ(dispatcher.generate(&const_path_ptr), "const std::filesystem::path*");
    EXPECT_EQ(dispatcher.generate(&const_path_ptr_const), "const std::filesystem::path* const");

    // a surprise test with context
    EXPECT_EQ(dispatcher.generate(&const_path_ptr, true, &std_ns), "const filesystem::path*");

    EXPECT_EQ(dispatcher.generate(const_itertor), "using const_iterator = int;");
}

TEST(CPPGen, ValueGeneration) {
    // namespace std
    //            └── namespace filesystem
    //                            └── class path
    // class int
    using namespace cg::entities::cpp;

    /// Tree
    auto std_ns = Namespace(NamedEntity("std"));
    auto* fs = std_ns.create_child<Namespace>(NamedEntity("filesystem"));
    auto* path = fs->create_child<Class>(NamedEntity("path"));
    
    auto int_cls = Class(NamedEntity("int"));

    /// Types
    // const int
    auto const_int = Type(ClassHandler(&int_cls), TypeQualificator().toggle_const());
    // just a std::filesystem::path type
    auto type_path = Type(ClassHandler(path));

    /// Values
    auto some_value = Variable(NamedEntity("some_value"), TypeHandler(&const_int));
    auto some_value_with_def = Variable(NamedEntity("some_value"),
                                        TypeHandler(&const_int),
                                        ValueHandler("42"));

    auto path_value = Variable(NamedEntity("path_val"),
                               TypeHandler(&type_path),
                               ValueHandler("\"C:/DevTools\""));

    auto dispatcher = cg::RegDispatcherFtor<
        cg::gen::cpp::TypeGenerator,
        cg::gen::cpp::VariableGenerator
    >();
    
    // Without context
    EXPECT_EQ(dispatcher.generate(&some_value), "const int some_value");
    EXPECT_EQ(dispatcher.generate(&some_value_with_def), "const int some_value = 42");
    EXPECT_EQ(dispatcher.generate(&path_value), "std::filesystem::path path_val = \"C:/DevTools\"");  
    
    // With context
    EXPECT_EQ(dispatcher.generate(&path_value, true, fs), "path path_val = \"C:/DevTools\"");
}