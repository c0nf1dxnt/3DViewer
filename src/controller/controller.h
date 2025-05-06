#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "model/model.h"

namespace viewer3d {

class Controller {
 public:
  Controller(Model& model) : model_(model) {}
  ~Controller() = default;

  bool LoadModel(const std::string& filename) {
    return model_.LoadFromFile(filename);
  }
  void ClearModel() { model_.Clear(); }

  void TranslateModel(float dx, float dy, float dz) {
    model_.Translate(dx, dy, dz);
  }
  void RotateModel(float angleX, float angleY, float angleZ) {
    model_.Rotate(angleX, angleY, angleZ);
  }
  void ScaleModel(float factor) { model_.Scale(factor); }

  const std::vector<Vertex>& GetVertices() const {
    return model_.GetVertices();
  }
  const std::vector<Face>& GetFaces() const { return model_.GetFaces(); }

  std::string GetFilename() const { return model_.GetFilename(); }
  int GetVertexCount() const { return model_.GetVertexCount(); }
  int GetEdgeCount() const { return model_.GetEdgeCount(); }

 private:
  Model& model_;
};

}  // namespace viewer3d

#endif