#include "controller/controller.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "model/model.hpp"

namespace viewer3d {
namespace {

class TestController : public ::testing::Test {
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

TEST_F(TestController, LoadModel) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetFilename(), "test_cube.obj");
}

TEST_F(TestController, ClearModel) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_NE(controller_.GetVertexCount(), 0);

    controller_.ClearModel();
    EXPECT_EQ(controller_.GetVertexCount(), 0);
    EXPECT_EQ(controller_.GetEdgeCount(), 0);
    EXPECT_TRUE(controller_.GetFilename().empty());
}

TEST_F(TestController, TranslateModel) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    const auto& originalVertices = controller_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    controller_.TranslateModel(1.0f, 2.0f, 3.0f);

    const auto& translatedVertices = controller_.GetVertices();
    EXPECT_FLOAT_EQ(translatedVertices[0].x, firstVertex.x + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[0].y, firstVertex.y + 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[0].z, firstVertex.z + 3.0f);
}

TEST_F(TestController, RotateModel) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    controller_.RotateModel(90.0f, 0.0f, 0.0f);

    const auto& rotatedVertices = controller_.GetVertices();
    EXPECT_NEAR(rotatedVertices[0].y, -1.0f, 0.001f);
    EXPECT_NEAR(rotatedVertices[0].z, 1.0f, 0.001f);
}

TEST_F(TestController, ScaleModel) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    const auto& originalVertices = controller_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    controller_.ScaleModel(2.0f);

    const auto& scaledVertices = controller_.GetVertices();
    EXPECT_FLOAT_EQ(scaledVertices[0].x, firstVertex.x * 2.0f);
    EXPECT_FLOAT_EQ(scaledVertices[0].y, firstVertex.y * 2.0f);
    EXPECT_FLOAT_EQ(scaledVertices[0].z, firstVertex.z * 2.0f);
}

TEST_F(TestController, GetVertices) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetVertices().size(), 8);
}

TEST_F(TestController, GetFaces) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetFaces().size(), 6);
}

TEST_F(TestController, GetVertexCount) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetVertexCount(), 8);
}

TEST_F(TestController, GetEdgeCount) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));
    EXPECT_EQ(controller_.GetEdgeCount(), 12);
}

TEST_F(TestController, CombinedTransformations) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    controller_.ScaleModel(2.0f);
    controller_.RotateModel(90.0f, 0.0f, 0.0f);
    controller_.TranslateModel(1.0f, 1.0f, 1.0f);

    const auto& transformedVertices = controller_.GetVertices();
    EXPECT_NEAR(transformedVertices[0].x, 3.0f, 0.001f);
    EXPECT_NEAR(transformedVertices[0].y, -1.0f, 0.001f);
    EXPECT_NEAR(transformedVertices[0].z, 3.0f, 0.001f);
}

}  // namespace
}  // namespace viewer3d