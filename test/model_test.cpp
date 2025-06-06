#include "model/model.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <string>

namespace viewer3d {
namespace {

class ModelTest : public ::testing::Test {
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
};

TEST_F(ModelTest, LoadFromFile) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));
    EXPECT_EQ(model_.GetFilename(), "test_cube.obj");
    EXPECT_EQ(model_.GetVertexCount(), 8);
    EXPECT_EQ(model_.GetFaces().size(), 6);
    EXPECT_EQ(model_.GetEdgeCount(), 12);
}

TEST_F(ModelTest, LoadFromNonExistentFile) {
    EXPECT_FALSE(model_.LoadFromFile("non_existent_file.obj"));
    EXPECT_EQ(model_.GetVertexCount(), 0);
}

TEST_F(ModelTest, Clear) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));
    EXPECT_EQ(model_.GetVertexCount(), 8);

    model_.Clear();
    EXPECT_EQ(model_.GetVertexCount(), 0);
    EXPECT_EQ(model_.GetFaces().size(), 0);
    EXPECT_EQ(model_.GetEdgeCount(), 0);
    EXPECT_TRUE(model_.GetFilename().empty());
}

TEST_F(ModelTest, Translate) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));

    const auto& originalVertices = model_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    model_.Translate(1.0f, 2.0f, 3.0f);

    const auto& translatedVertices = model_.GetVertices();
    EXPECT_FLOAT_EQ(translatedVertices[0].x, firstVertex.x + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[0].y, firstVertex.y + 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[0].z, firstVertex.z + 3.0f);
}

TEST_F(ModelTest, Scale) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));

    const auto& originalVertices = model_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    model_.Scale(2.0f);

    const auto& scaledVertices = model_.GetVertices();
    EXPECT_FLOAT_EQ(scaledVertices[0].x, firstVertex.x * 2.0f);
    EXPECT_FLOAT_EQ(scaledVertices[0].y, firstVertex.y * 2.0f);
    EXPECT_FLOAT_EQ(scaledVertices[0].z, firstVertex.z * 2.0f);
}

TEST_F(ModelTest, ScaleTooSmall) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));

    const auto& originalVertices = model_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    model_.Scale(0.05f);

    const auto& scaledVertices = model_.GetVertices();
    EXPECT_FLOAT_EQ(scaledVertices[0].x, firstVertex.x);
    EXPECT_FLOAT_EQ(scaledVertices[0].y, firstVertex.y);
    EXPECT_FLOAT_EQ(scaledVertices[0].z, firstVertex.z);
}

TEST_F(ModelTest, Rotate) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));

    model_.Rotate(90.0f, 0.0f, 0.0f);

    const auto& rotatedVertices = model_.GetVertices();
    EXPECT_NEAR(rotatedVertices[0].y, -1.0f, 0.001f);
    EXPECT_NEAR(rotatedVertices[0].z, 1.0f, 0.001f);
}

TEST_F(ModelTest, CombinedTransformations) {
    EXPECT_TRUE(model_.LoadFromFile("test_cube.obj"));

    model_.Scale(2.0f);
    model_.Rotate(90.0f, 0.0f, 0.0f);
    model_.Translate(1.0f, 1.0f, 1.0f);

    const auto& transformedVertices = model_.GetVertices();
    EXPECT_NEAR(transformedVertices[0].x, 3.0f, 0.001f);
    EXPECT_NEAR(transformedVertices[0].y, -1.0f, 0.001f);
    EXPECT_NEAR(transformedVertices[0].z, 3.0f, 0.001f);
}

}  // namespace
}  // namespace viewer3d