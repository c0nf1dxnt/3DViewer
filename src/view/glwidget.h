#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "controller/controller.h"

namespace viewer3d {

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit GLWidget(Controller& controller, QWidget* parent = nullptr);
  ~GLWidget();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 public slots:
  void updateModel();

 private:
  Controller& controller_;
  QPoint lastPos_;

  float rotationX_ = 0.0f;
  float rotationY_ = 0.0f;
  float zoom_ = 1.0f;

  void drawModel();
};

}  // namespace viewer3d

#endif