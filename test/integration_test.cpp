#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "controller/controller.hpp"
#include "model/model.hpp"

namespace viewer3d {
namespace {

class IntegrationTest : public ::testing::Test {
   protected:
    void SetUp() override { CreateTestObjFile("test_cube.obj"); }

    void TearDown() override { std::remove("test_cube.obj"); }

    void CreateTestObjFile(const std::string& filename) {
        std::ofstream file(filename);
        file << "v 1.0 1.0 1.0\n";
        file << "v 1.0 1.0 -1.0\n";
        file << "v 1.0 -1.0 1.0\n";
        file << "v 1.0 -1.0 -1.0\n";
        file << "v -1.0 1.0 1.0\n";
        file << "v -1.0 1.0 -1.0\n";
        file << "v -1.0 -1.0 1.0\n";
        file << "v -1.0 -1.0 -1.0\n";
        file << "f 1 2 4 3\n";
        file << "f 5 6 8 7\n";
        file << "f 1 5 7 3\n";
        file << "f 2 6 8 4\n";
        file << "f 1 2 6 5\n";
        file << "f 3 4 8 7\n";
        file.close();
    }

    Model model_;
    Controller controller_{model_};
};

TEST_F(IntegrationTest, LoadAndTransform) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetVertexCount(), 8);
    EXPECT_EQ(controller_.GetEdgeCount(), 24);

    controller_.ScaleModel(2.0f);
    controller_.RotateModel(90.0f, 0.0f, 0.0f);
    controller_.TranslateModel(1.0f, 1.0f, 1.0f);

    const auto& vertices = controller_.GetVertices();
    EXPECT_NEAR(vertices[0].x, 3.0f, 0.001f);
    EXPECT_NEAR(vertices[0].y, -1.0f, 0.001f);
    EXPECT_NEAR(vertices[0].z, 3.0f, 0.001f);
}

TEST_F(IntegrationTest, ClearAndReload) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetVertexCount(), 8);

    controller_.ClearModel();
    EXPECT_EQ(controller_.GetVertexCount(), 0);

    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetVertexCount(), 8);
}

TEST_F(IntegrationTest, TransformationsOrder) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    controller_.ScaleModel(2.0f);
    controller_.RotateModel(0.0f, 90.0f, 0.0f);

    const auto& vertices = controller_.GetVertices();
    EXPECT_NEAR(vertices[0].x, 2.0f, 0.001f);
    EXPECT_NEAR(vertices[0].y, 2.0f, 0.001f);
    EXPECT_NEAR(vertices[0].z, -2.0f, 0.001f);
}

}  // namespace
}  // namespace viewer3d