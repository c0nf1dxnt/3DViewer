#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>

#include "controller/controller.h"
#include "view/glwidget.h"

namespace viewer3d {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(Controller& controller, QWidget* parent = nullptr);
  ~MainWindow() = default;

 private slots:
  void openFile();
  void translate();
  void rotate();
  void scale();
  void updateStatusBar();

 private:
  Controller& controller_;
  GLWidget* glWidget_;

  QPushButton* openButton_;

  QDoubleSpinBox* translateXSpin_;
  QDoubleSpinBox* translateYSpin_;
  QDoubleSpinBox* translateZSpin_;
  QPushButton* translateButton_;

  QDoubleSpinBox* rotateXSpin_;
  QDoubleSpinBox* rotateYSpin_;
  QDoubleSpinBox* rotateZSpin_;
  QPushButton* rotateButton_;

  QDoubleSpinBox* scaleSpin_;
  QPushButton* scaleButton_;

  QLabel* fileNameLabel_;
  QLabel* vertexCountLabel_;
  QLabel* edgeCountLabel_;

  void setupUI();
  void createWidgets();
  void createLayouts();
  void setupConnections();
};

}  // namespace viewer3d

#endif