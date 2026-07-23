#include <ClassGen.h>
#include <iostream>

using namespace cg::build;
using namespace cg::generate;

int main() {
    system("chcp 65001 > nul");

    auto var = VariableBuilder("global_counter")
        .with_type(TypeBuilder("int").build())
        .build();

    //std::cout << VariableGenerator::generate(var, GenStage::Declaration);
    
    auto size_t_t = TypeBuilder("size_t")
        .build();

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

    auto var_with_temp_t = cg::build::VariableBuilder(var)
        .with_type(IsConst_t)
        .build();

    auto validator = cg::validate::FunctionValidator();
    //validator.verify(var_with_temp_t);

    //std::cout << cg::generate::VariableGenerator::generate(var_with_temp_t, cg::generate::GenStage::Declaration);
    auto T_t = TypeBuilder("T")
        .as_template()
        .build();

    auto func_template = FunctionBuilder("max")
        .with_type(T_t)
        //.with_template(T_t)
        .add_argument(VariableBuilder("a").with_type(T_t).build())
        .add_argument(VariableBuilder("b").with_type(T_t).build())
        .build();

    auto func = FunctionBuilder("max")
        .with_type(size_t_t)
        .add_argument(VariableBuilder("a").with_type(size_t_t).build())
        .add_argument(VariableBuilder("b").with_type(size_t_t).build())
        .build();

    std::vector<cg::source::TypeName> req_list;

    for (const auto& ns : func_template.get_namespace()) { req_list.push_back(ns.name); }
    for (const auto& arg : func_template.get_args()) { req_list.push_back(arg.get_type()); }
    req_list.push_back(func_template.get_type());

    validator.verify(func_template);

    std::cout << cg::generate::FunctionGenerator::generate(func_template, cg::generate::GenStage::Inline);

    return 0;
}