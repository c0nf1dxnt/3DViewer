#include "model/model.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace viewer3d {

bool Model::LoadFromFile(const std::string& filename) {
  Clear();

  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  filename_ = filename;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "v") {
      Vertex vertex;
      iss >> vertex.x >> vertex.y >> vertex.z;
      vertices_.push_back(vertex);
    } else if (token == "f") {
      Face face;
      std::string vertex_index;
      while (iss >> vertex_index) {
        std::istringstream vertex_stream(vertex_index);
        int v_index;
        vertex_stream >> v_index;

        if (v_index < 0) {
          v_index = vertices_.size() + v_index + 1;
        }

        face.vertexIndices.push_back(v_index - 1);
      }
      if (face.vertexIndices.size() > 2) {
        faces_.push_back(face);
      }
    }
  }

  CalculateEdgeCount();
  return true;
}

void Model::Clear() {
  vertices_.clear();
  faces_.clear();
  filename_.clear();
  edge_count_ = 0;
}

void Model::Translate(float dx, float dy, float dz) {
  for (auto& vertex : vertices_) {
    vertex.x += dx;
    vertex.y += dy;
    vertex.z += dz;
  }
}

void Model::Rotate(float angleX, float angleY, float angleZ) {
  float radX = angleX * M_PI / 180.0f;
  float radY = angleY * M_PI / 180.0f;
  float radZ = angleZ * M_PI / 180.0f;

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

    if (angleX != 0) {
      float y1 = y * cosX - z * sinX;
      float z1 = y * sinX + z * cosX;
      y = y1;
      z = z1;
    }

    if (angleY != 0) {
      float x1 = x * cosY + z * sinY;
      float z1 = -x * sinY + z * cosY;
      x = x1;
      z = z1;
    }

    if (angleZ != 0) {
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

void Model::Scale(float factor) {
  if (factor <= 0) {
    return;
  }

  for (auto& vertex : vertices_) {
    vertex.x *= factor;
    vertex.y *= factor;
    vertex.z *= factor;
  }
}

void Model::CalculateEdgeCount() {
  edge_count_ = 0;
  for (const auto& face : faces_) {
    edge_count_ += face.vertexIndices.size();
  }
}

}  // namespace viewer3d