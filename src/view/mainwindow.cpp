#include "view/mainwindow.h"

#include <QFileInfo>
#include <QMessageBox>

namespace viewer3d {

MainWindow::MainWindow(Controller& controller, QWidget* parent)
    : QMainWindow(parent), controller_(controller) {
  setupUI();
  setupConnections();
  updateStatusBar();
}

void MainWindow::setupUI() {
  createWidgets();
  createLayouts();

  setWindowTitle("3DViewer v2.0");
  setMinimumSize(800, 600);
}

void MainWindow::createWidgets() {
  glWidget_ = new GLWidget(controller_, this);

  openButton_ = new QPushButton("Открыть файл", this);

  translateXSpin_ = new QDoubleSpinBox(this);
  translateYSpin_ = new QDoubleSpinBox(this);
  translateZSpin_ = new QDoubleSpinBox(this);
  translateButton_ = new QPushButton("Переместить", this);

  rotateXSpin_ = new QDoubleSpinBox(this);
  rotateYSpin_ = new QDoubleSpinBox(this);
  rotateZSpin_ = new QDoubleSpinBox(this);
  rotateButton_ = new QPushButton("Повернуть", this);

  scaleSpin_ = new QDoubleSpinBox(this);
  scaleButton_ = new QPushButton("Масштабировать", this);

  translateXSpin_->setRange(-100.0, 100.0);
  translateYSpin_->setRange(-100.0, 100.0);
  translateZSpin_->setRange(-100.0, 100.0);

  rotateXSpin_->setRange(-360.0, 360.0);
  rotateYSpin_->setRange(-360.0, 360.0);
  rotateZSpin_->setRange(-360.0, 360.0);

  scaleSpin_->setRange(0.01, 100.0);
  scaleSpin_->setValue(1.0);
  scaleSpin_->setSingleStep(0.1);

  fileNameLabel_ = new QLabel(this);
  vertexCountLabel_ = new QLabel(this);
  edgeCountLabel_ = new QLabel(this);
}

void MainWindow::createLayouts() {
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

  mainLayout->addWidget(glWidget_);

  QHBoxLayout* controlsLayout = new QHBoxLayout();
  mainLayout->addLayout(controlsLayout);

  QVBoxLayout* fileLayout = new QVBoxLayout();
  fileLayout->addWidget(openButton_);

  QGroupBox* infoGroup = new QGroupBox("Информация о модели", this);
  QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
  infoLayout->addWidget(fileNameLabel_);
  infoLayout->addWidget(vertexCountLabel_);
  infoLayout->addWidget(edgeCountLabel_);
  fileLayout->addWidget(infoGroup);

  controlsLayout->addLayout(fileLayout);

  QGroupBox* translateGroup = new QGroupBox("Перемещение", this);
  QVBoxLayout* translateLayout = new QVBoxLayout(translateGroup);
  translateLayout->addWidget(new QLabel("X:", this));
  translateLayout->addWidget(translateXSpin_);
  translateLayout->addWidget(new QLabel("Y:", this));
  translateLayout->addWidget(translateYSpin_);
  translateLayout->addWidget(new QLabel("Z:", this));
  translateLayout->addWidget(translateZSpin_);
  translateLayout->addWidget(translateButton_);
  controlsLayout->addWidget(translateGroup);

  QGroupBox* rotateGroup = new QGroupBox("Вращение", this);
  QVBoxLayout* rotateLayout = new QVBoxLayout(rotateGroup);
  rotateLayout->addWidget(new QLabel("X:", this));
  rotateLayout->addWidget(rotateXSpin_);
  rotateLayout->addWidget(new QLabel("Y:", this));
  rotateLayout->addWidget(rotateYSpin_);
  rotateLayout->addWidget(new QLabel("Z:", this));
  rotateLayout->addWidget(rotateZSpin_);
  rotateLayout->addWidget(rotateButton_);
  controlsLayout->addWidget(rotateGroup);

  QGroupBox* scaleGroup = new QGroupBox("Масштабирование", this);
  QVBoxLayout* scaleLayout = new QVBoxLayout(scaleGroup);
  scaleLayout->addWidget(new QLabel("Коэффициент:", this));
  scaleLayout->addWidget(scaleSpin_);
  scaleLayout->addWidget(scaleButton_);
  controlsLayout->addWidget(scaleGroup);
}

void MainWindow::setupConnections() {
  connect(openButton_, &QPushButton::clicked, this, &MainWindow::openFile);
  connect(translateButton_, &QPushButton::clicked, this,
          &MainWindow::translate);
  connect(rotateButton_, &QPushButton::clicked, this, &MainWindow::rotate);
  connect(scaleButton_, &QPushButton::clicked, this, &MainWindow::scale);
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Открыть OBJ файл", "",
                                                  "OBJ Files (*.obj)");
  if (fileName.isEmpty()) {
    return;
  }

  QFileInfo fileInfo(fileName);
  if (fileInfo.size() > 10 * 1024 * 1024) {
    QMessageBox::warning(this, "Предупреждение",
                         "Файл превышает 10 МБ. Загрузка может занять время.");
  }

  if (controller_.LoadModel(fileName.toStdString())) {
    updateStatusBar();
    glWidget_->updateModel();
  } else {
    QMessageBox::critical(this, "Ошибка", "Не удалось загрузить файл");
  }
}

void MainWindow::translate() {
  float dx = static_cast<float>(translateXSpin_->value());
  float dy = static_cast<float>(translateYSpin_->value());
  float dz = static_cast<float>(translateZSpin_->value());

  controller_.TranslateModel(dx, dy, dz);
  glWidget_->updateModel();

  translateXSpin_->setValue(0.0);
  translateYSpin_->setValue(0.0);
  translateZSpin_->setValue(0.0);
}

void MainWindow::rotate() {
  float angleX = static_cast<float>(rotateXSpin_->value());
  float angleY = static_cast<float>(rotateYSpin_->value());
  float angleZ = static_cast<float>(rotateZSpin_->value());

  controller_.RotateModel(angleX, angleY, angleZ);
  glWidget_->updateModel();

  rotateXSpin_->setValue(0.0);
  rotateYSpin_->setValue(0.0);
  rotateZSpin_->setValue(0.0);
}

void MainWindow::scale() {
  float factor = static_cast<float>(scaleSpin_->value());

  controller_.ScaleModel(factor);
  glWidget_->updateModel();

  scaleSpin_->setValue(1.0);
}

void MainWindow::updateStatusBar() {
  fileNameLabel_->setText("Файл: " +
                          QString::fromStdString(controller_.GetFilename()));
  vertexCountLabel_->setText("Вершин: " +
                             QString::number(controller_.GetVertexCount()));
  edgeCountLabel_->setText("Ребер: " +
                           QString::number(controller_.GetEdgeCount()));
}

}  // namespace viewer3d