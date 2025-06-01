#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "controller/controller.hpp"
#include "model/model.hpp"

namespace viewer3d {
namespace {

class ErrorHandlingTest : public ::testing::Test {
   protected:
    void SetUp() override {
        CreateInvalidObjFile("invalid.obj");
        CreateTestCubeFile("test_cube.obj");
    }

    void TearDown() override {
        std::remove("invalid.obj");
        std::remove("test_cube.obj");
    }

    void CreateInvalidObjFile(const std::string& filename) {
        std::ofstream file(filename);
        file << "invalid content\n";
        file << "v 1.0 not_a_number 1.0\n";
        file << "f 1 2 nonexistent\n";
        file.close();
    }

    void CreateTestCubeFile(const std::string& filename) {
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

TEST_F(ErrorHandlingTest, LoadInvalidFile) {
    EXPECT_FALSE(controller_.LoadModel("nonexistent_file.obj"));
}

TEST_F(ErrorHandlingTest, InvalidFileContent) {
    EXPECT_FALSE(controller_.LoadModel("invalid.obj"));
    EXPECT_EQ(controller_.GetVertexCount(), 0);
    EXPECT_EQ(controller_.GetEdgeCount(), 0);
}

TEST_F(ErrorHandlingTest, ScaleTooSmall) {
    EXPECT_TRUE(controller_.LoadModel("test_cube.obj"));

    const auto& originalVertices = controller_.GetVertices();
    Vertex firstVertex = originalVertices[0];

    controller_.ScaleModel(0.05f);

    const auto& scaledVertices = controller_.GetVertices();
    EXPECT_FLOAT_EQ(scaledVertices[0].x, firstVertex.x);
    EXPECT_FLOAT_EQ(scaledVertices[0].y, firstVertex.y);
    EXPECT_FLOAT_EQ(scaledVertices[0].z, firstVertex.z);
}

}  // namespace
}  // namespace viewer3d