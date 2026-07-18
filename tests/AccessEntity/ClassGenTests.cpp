#include <gtest/gtest.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <AccessEntity/AccessGenerator.h>

using namespace cg::build;
using namespace cg::generate;

TEST(ClassGenTest, Declaration) {
	auto cls = ClassBuilder("Class").build();

	EXPECT_EQ(ClassGenerator::generate(cls, GenStage::Declaration), "class Class {\n};");
}