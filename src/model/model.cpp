#include "model/model.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace viewer3d {

namespace {
constexpr float kDegToRad = M_PI / 180.0f;
constexpr float kMinScaleFactor = 0.1f;
}  // namespace

bool Model::LoadFromFile(const std::string& filename) {
    Clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open the file " << filename << std::endl;
        return false;
    }

    filename_ = filename;
    std::string line;
    int line_number = 0;

    try {
        while (std::getline(file, line)) {
            line_number++;
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "v") {
                Vertex vertex;
                if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
                    std::cerr << "Warning: incorrect vertex data in the row "
                              << line_number << std::endl;
                    continue;
                }
                vertices_.push_back(vertex);
            } else if (token == "f") {
                Face face;
                std::string vertex_index;
                while (iss >> vertex_index) {
                    std::istringstream vertex_stream(vertex_index);
                    int v_index;
                    if (!(vertex_stream >> v_index)) {
                        std::cerr
                            << "Warning: incorrect vertex index in the row "
                            << line_number << std::endl;
                        continue;
                    }

                    if (v_index < 0) {
                        v_index = vertices_.size() + v_index + 1;
                    }

                    if (v_index <= 0 ||
                        v_index > static_cast<int>(vertices_.size())) {
                        std::cerr << "Warning: the index of the vertex is out "
                                     "of range in "
                                     "the row "
                                  << line_number << std::endl;
                        continue;
                    }

                    face.vertexIndices.push_back(v_index - 1);
                }
                if (face.vertexIndices.size() >= 2) {
                    faces_.push_back(face);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading the file: " << e.what() << std::endl;
        Clear();
        return false;
    }

    if (vertices_.empty()) {
        std::cerr << "Warning: the file does not contain vertices" << std::endl;
        return false;
    }

    original_vertices_ = vertices_;

    current_translate_x_ = 0.0f;
    current_translate_y_ = 0.0f;
    current_translate_z_ = 0.0f;
    current_rotate_x_ = 0.0f;
    current_rotate_y_ = 0.0f;
    current_rotate_z_ = 0.0f;
    current_scale_ = 1.0f;

    CalculateEdgeCount();
    return true;
}

void Model::Clear() {
    vertices_.clear();
    original_vertices_.clear();
    faces_.clear();
    filename_.clear();
    edge_count_ = 0;

    current_translate_x_ = 0.0f;
    current_translate_y_ = 0.0f;
    current_translate_z_ = 0.0f;
    current_rotate_x_ = 0.0f;
    current_rotate_y_ = 0.0f;
    current_rotate_z_ = 0.0f;
    current_scale_ = 1.0f;
}

void Model::Translate(float dx, float dy, float dz) {
    current_translate_x_ = dx;
    current_translate_y_ = dy;
    current_translate_z_ = dz;

    ApplyAllTransformations();
}

void Model::Rotate(float angleX, float angleY, float angleZ) {
    current_rotate_x_ = angleX;
    current_rotate_y_ = angleY;
    current_rotate_z_ = angleZ;

    ApplyAllTransformations();
}

void Model::Scale(float factor) {
    if (factor <= kMinScaleFactor) {
        std::cerr << "Warning: The zoom level is too small" << std::endl;
        return;
    }

    current_scale_ = factor;

    ApplyAllTransformations();
}

void Model::ApplyAllTransformations() {
    vertices_ = original_vertices_;

    if (current_scale_ != 1.0f) {
        for (auto& vertex : vertices_) {
            vertex.x *= current_scale_;
            vertex.y *= current_scale_;
            vertex.z *= current_scale_;
        }
    }

    if (current_rotate_x_ != 0.0f || current_rotate_y_ != 0.0f ||
        current_rotate_z_ != 0.0f) {
        float radX = current_rotate_x_ * kDegToRad;
        float radY = current_rotate_y_ * kDegToRad;
        float radZ = current_rotate_z_ * kDegToRad;

        float sinX = sin(radX);
        float cosX = cos(radX);
        float sinY = sin(radY);
        float cosY = cos(radY);
        float sinZ = sin(radZ);
        float cosZ = cos(radZ);

        for (auto& vertex : vertices_) {
            float x = vertex.x;
            float y = vertex.y;
            float z = vertex.z;

            if (current_rotate_x_ != 0) {
                float y1 = y * cosX - z * sinX;
                float z1 = y * sinX + z * cosX;
                y = y1;
                z = z1;
            }

            if (current_rotate_y_ != 0) {
                float x1 = x * cosY + z * sinY;
                float z1 = -x * sinY + z * cosY;
                x = x1;
                z = z1;
            }

            if (current_rotate_z_ != 0) {
                float x1 = x * cosZ - y * sinZ;
                float y1 = x * sinZ + y * cosZ;
                x = x1;
                y = y1;
            }

            vertex.x = x;
            vertex.y = y;
            vertex.z = z;
        }
    }

    if (current_translate_x_ != 0.0f || current_translate_y_ != 0.0f ||
        current_translate_z_ != 0.0f) {
        for (auto& vertex : vertices_) {
            vertex.x += current_translate_x_;
            vertex.y += current_translate_y_;
            vertex.z += current_translate_z_;
        }
    }
}

void Model::CalculateEdgeCount() {
    edge_count_ = 0;
    for (const auto& face : faces_) {
        edge_count_ += face.vertexIndices.size();
    }
}

}  // namespace viewer3d