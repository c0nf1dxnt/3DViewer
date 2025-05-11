#include "model/model.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace viewer3d {

namespace {
constexpr float kDegToRad = M_PI / 180.0f;
constexpr float kMinScaleFactor = 0.0001f;
}  // namespace

bool Model::LoadFromFile(const std::string& filename) {
  Clear();

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
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
          std::cerr << "Предупреждение: некорректные данные вершины в строке "
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
            std::cerr << "Предупреждение: некорректный индекс вершины в строке "
                      << line_number << std::endl;
            continue;
          }

          if (v_index < 0) {
            v_index = vertices_.size() + v_index + 1;
          }

          if (v_index <= 0 || v_index > static_cast<int>(vertices_.size())) {
            std::cerr
                << "Предупреждение: индекс вершины вне диапазона в строке "
                << line_number << std::endl;
            continue;
          }

          face.vertexIndices.push_back(v_index - 1);
        }
        if (face.vertexIndices.size() > 2) {
          faces_.push_back(face);
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Ошибка при чтении файла: " << e.what() << std::endl;
    Clear();
    return false;
  }

  if (vertices_.empty()) {
    std::cerr << "Предупреждение: файл не содержит вершин" << std::endl;
    return false;
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
  float radX = angleX * kDegToRad;
  float radY = angleY * kDegToRad;
  float radZ = angleZ * kDegToRad;

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
  if (factor <= kMinScaleFactor) {
    std::cerr << "Предупреждение: коэффициент масштабирования слишком мал"
              << std::endl;
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