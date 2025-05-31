#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

namespace viewer3d {

struct Vertex {
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};
};

struct Face {
  std::vector<int> vertexIndices;
};

class Model {
 public:
  Model() = default;
  ~Model() = default;

  bool LoadFromFile(const std::string& filename);
  void Clear();

  void Translate(float dx, float dy, float dz);
  void Rotate(float angleX, float angleY, float angleZ);
  void Scale(float factor);

  const std::vector<Vertex>& GetVertices() const { return vertices_; }
  const std::vector<Face>& GetFaces() const { return faces_; }

  std::string GetFilename() const { return filename_; }
  int GetVertexCount() const { return vertices_.size(); }
  int GetEdgeCount() const { return edge_count_; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Vertex> original_vertices_;
  std::vector<Face> faces_;
  std::string filename_;
  int edge_count_{0};

  float current_translate_x_{0.0f};
  float current_translate_y_{0.0f};
  float current_translate_z_{0.0f};
  float current_rotate_x_{0.0f};
  float current_rotate_y_{0.0f};
  float current_rotate_z_{0.0f};
  float current_scale_{1.0f};

  void CalculateEdgeCount();
  void ApplyAllTransformations();
};

}  // namespace viewer3d

#endif