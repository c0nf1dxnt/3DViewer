#include "controller/controller.hpp"

namespace viewer3d {

Controller::Controller(Model& model) : model_(model) {}

bool Controller::LoadModel(const std::string& filename) {
    return model_.LoadFromFile(filename);
}

void Controller::ClearModel() { model_.Clear(); }

void Controller::TranslateModel(float dx, float dy, float dz) {
    model_.Translate(dx, dy, dz);
}

void Controller::RotateModel(float angleX, float angleY, float angleZ) {
    model_.Rotate(angleX, angleY, angleZ);
}

void Controller::ScaleModel(float factor) { model_.Scale(factor); }

const std::vector<Vertex>& Controller::GetVertices() const {
    return model_.GetVertices();
}

const std::vector<Face>& Controller::GetFaces() const {
    return model_.GetFaces();
}

std::string Controller::GetFilename() const { return model_.GetFilename(); }

int Controller::GetVertexCount() const { return model_.GetVertexCount(); }

int Controller::GetEdgeCount() const { return model_.GetEdgeCount(); }

}  // namespace viewer3d