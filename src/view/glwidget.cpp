#include "view/glwidget.h"

#include <GL/glu.h>

#include <QMouseEvent>
#include <QWheelEvent>

namespace viewer3d {

namespace {
constexpr float kBackgroundR = 0.2f;
constexpr float kBackgroundG = 0.2f;
constexpr float kBackgroundB = 0.2f;
constexpr float kBackgroundA = 1.0f;

constexpr float kModelR = 0.9f;
constexpr float kModelG = 0.9f;
constexpr float kModelB = 0.9f;

constexpr float kCameraDistance = -5.0f;
constexpr float kRotationSpeed = 0.5f;
constexpr float kZoomSpeed = 0.1f;
constexpr float kMinZoom = 0.1f;
constexpr float kDefaultFOV = 45.0f;
constexpr float kNearPlane = 0.1f;
constexpr float kFarPlane = 100.0f;
}  // namespace

GLWidget::GLWidget(Controller& controller, QWidget* parent)
    : QOpenGLWidget(parent), controller_(controller) {}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(kBackgroundR, kBackgroundG, kBackgroundB, kBackgroundA);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, kCameraDistance);
  glScalef(zoom_, zoom_, zoom_);
  glRotatef(rotationX_, 1.0f, 0.0f, 0.0f);
  glRotatef(rotationY_, 0.0f, 1.0f, 0.0f);

  drawModel();
}

void GLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float aspect = static_cast<float>(width) / (height ? height : 1);
  gluPerspective(kDefaultFOV, aspect, kNearPlane, kFarPlane);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GLWidget::mousePressEvent(QMouseEvent* event) { lastPos_ = event->pos(); }

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
  int dx = event->x() - lastPos_.x();
  int dy = event->y() - lastPos_.y();

  if (event->buttons() & Qt::LeftButton) {
    rotationX_ += kRotationSpeed * dy;
    rotationY_ += kRotationSpeed * dx;
    update();
  }

  lastPos_ = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event) {
  float delta = event->angleDelta().y() / 120.0f;
  zoom_ += zoom_ * kZoomSpeed * delta;
  zoom_ = std::max(kMinZoom, zoom_);
  update();
}

void GLWidget::updateModel() { update(); }

void GLWidget::drawModel() {
  const auto& vertices = controller_.GetVertices();
  const auto& faces = controller_.GetFaces();

  if (vertices.empty() || faces.empty()) {
    return;
  }

  glColor3f(kModelR, kModelG, kModelB);

  for (const auto& face : faces) {
    const auto& indices = face.vertexIndices;
    if (indices.size() < 3) continue;

    glBegin(GL_LINE_LOOP);
    for (int index : indices) {
      if (index >= 0 && index < static_cast<int>(vertices.size())) {
        const auto& vertex = vertices[index];
        glVertex3f(vertex.x, vertex.y, vertex.z);
      }
    }
    glEnd();
  }
}

}  // namespace viewer3d