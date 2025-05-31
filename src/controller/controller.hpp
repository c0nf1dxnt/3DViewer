#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "model/model.hpp"

namespace viewer3d {

class Controller {
   public:
    Controller(Model& model);
    ~Controller() = default;

    bool LoadModel(const std::string& filename);
    void ClearModel();

    void TranslateModel(float dx, float dy, float dz);
    void RotateModel(float angleX, float angleY, float angleZ);
    void ScaleModel(float factor);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<Face>& GetFaces() const;

    std::string GetFilename() const;
    int GetVertexCount() const;
    int GetEdgeCount() const;

   private:
    Model& model_;
};

}  // namespace viewer3d

#endif