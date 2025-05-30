#include "view/mainwindow.h"

#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QPalette>
#include <QStyleFactory>

namespace viewer3d {

namespace {
const QString kAppTitle = "3DViewer";
const int kMinWindowWidth = 800;
const int kMinWindowHeight = 600;

QPalette createDarkPalette() {
  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
  return darkPalette;
}
}  // namespace

MainWindow::MainWindow(Controller& controller, QWidget* parent)
    : QMainWindow(parent), controller_(controller) {
  qApp->setStyle(QStyleFactory::create("Fusion"));
  qApp->setPalette(createDarkPalette());

  setupUI();
  setupConnections();
  updateStatusBar();
}

void MainWindow::setupUI() {
  createWidgets();
  createLayouts();

  setWindowTitle(kAppTitle);
  setMinimumSize(kMinWindowWidth, kMinWindowHeight);

  statusBar()->showMessage("Ready for work.");
}

void MainWindow::createWidgets() {
  glWidget_ = new GLWidget(controller_, this);

  openButton_ = new QPushButton("Open file", this);
  openButton_->setIcon(QIcon::fromTheme("document-open"));

  translateXSpin_ = new QDoubleSpinBox(this);
  translateYSpin_ = new QDoubleSpinBox(this);
  translateZSpin_ = new QDoubleSpinBox(this);
  translateButton_ = new QPushButton("Move", this);

  rotateXSpin_ = new QDoubleSpinBox(this);
  rotateYSpin_ = new QDoubleSpinBox(this);
  rotateZSpin_ = new QDoubleSpinBox(this);
  rotateButton_ = new QPushButton("Rotate", this);

  scaleSpin_ = new QDoubleSpinBox(this);
  scaleButton_ = new QPushButton("Scale", this);

  translateXSpin_->setRange(-100.0, 100.0);
  translateYSpin_->setRange(-100.0, 100.0);
  translateZSpin_->setRange(-100.0, 100.0);

  translateXSpin_->setSingleStep(0.1);
  translateYSpin_->setSingleStep(0.1);
  translateZSpin_->setSingleStep(0.1);

  rotateXSpin_->setRange(-360.0, 360.0);
  rotateYSpin_->setRange(-360.0, 360.0);
  rotateZSpin_->setRange(-360.0, 360.0);

  rotateXSpin_->setSingleStep(5.0);
  rotateYSpin_->setSingleStep(5.0);
  rotateZSpin_->setSingleStep(5.0);

  scaleSpin_->setRange(0.01, 100.0);
  scaleSpin_->setValue(1.0);
  scaleSpin_->setSingleStep(0.1);

  fileNameLabel_ = new QLabel(this);
  vertexCountLabel_ = new QLabel(this);
  edgeCountLabel_ = new QLabel(this);

  QString buttonStyle =
      "QPushButton { background-color: #3a3a3a; border-radius: 4px; padding: "
      "6px; }";
  buttonStyle += "QPushButton:hover { background-color: #4a4a4a; }";
  buttonStyle += "QPushButton:pressed { background-color: #2a2a2a; }";

  openButton_->setStyleSheet(buttonStyle);
  translateButton_->setStyleSheet(buttonStyle);
  rotateButton_->setStyleSheet(buttonStyle);
  scaleButton_->setStyleSheet(buttonStyle);
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

  QGroupBox* infoGroup = new QGroupBox("Information about model", this);
  QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
  infoLayout->addWidget(fileNameLabel_);
  infoLayout->addWidget(vertexCountLabel_);
  infoLayout->addWidget(edgeCountLabel_);
  fileLayout->addWidget(infoGroup);

  controlsLayout->addLayout(fileLayout);

  QGroupBox* translateGroup = new QGroupBox("Movement", this);
  QVBoxLayout* translateLayout = new QVBoxLayout(translateGroup);
  translateLayout->addWidget(new QLabel("X:", this));
  translateLayout->addWidget(translateXSpin_);
  translateLayout->addWidget(new QLabel("Y:", this));
  translateLayout->addWidget(translateYSpin_);
  translateLayout->addWidget(new QLabel("Z:", this));
  translateLayout->addWidget(translateZSpin_);
  translateLayout->addWidget(translateButton_);
  controlsLayout->addWidget(translateGroup);

  QGroupBox* rotateGroup = new QGroupBox("Rotation", this);
  QVBoxLayout* rotateLayout = new QVBoxLayout(rotateGroup);
  rotateLayout->addWidget(new QLabel("X:", this));
  rotateLayout->addWidget(rotateXSpin_);
  rotateLayout->addWidget(new QLabel("Y:", this));
  rotateLayout->addWidget(rotateYSpin_);
  rotateLayout->addWidget(new QLabel("Z:", this));
  rotateLayout->addWidget(rotateZSpin_);
  rotateLayout->addWidget(rotateButton_);
  controlsLayout->addWidget(rotateGroup);

  QGroupBox* scaleGroup = new QGroupBox("Scaling", this);
  QVBoxLayout* scaleLayout = new QVBoxLayout(scaleGroup);
  scaleLayout->addWidget(new QLabel("Level:", this));
  scaleLayout->addWidget(scaleSpin_);
  scaleLayout->addWidget(scaleButton_);
  controlsLayout->addWidget(scaleGroup);

  QString groupStyle =
      "QGroupBox { font-weight: bold; border: 1px solid #555; border-radius: "
      "5px; margin-top: 10px; }";
  groupStyle +=
      "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 "
      "5px 0 5px; }";

  infoGroup->setStyleSheet(groupStyle);
  translateGroup->setStyleSheet(groupStyle);
  rotateGroup->setStyleSheet(groupStyle);
  scaleGroup->setStyleSheet(groupStyle);
}

void MainWindow::setupConnections() {
  connect(openButton_, &QPushButton::clicked, this, &MainWindow::openFile);
  connect(translateButton_, &QPushButton::clicked, this,
          &MainWindow::translate);
  connect(rotateButton_, &QPushButton::clicked, this, &MainWindow::rotate);
  connect(scaleButton_, &QPushButton::clicked, this, &MainWindow::scale);
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open .obj file", "",
                                                  "OBJ Files (*.obj)");
  if (fileName.isEmpty()) {
    return;
  }

  QFileInfo fileInfo(fileName);
  if (fileInfo.size() > 10 * 1024 * 1024) {
    QMessageBox::warning(
        this, "Warning!",
        "The file exceeds 10 MB. It may take a while to download.");
  }

  statusBar()->showMessage("Uploading the model...");

  if (controller_.LoadModel(fileName.toStdString())) {
    updateStatusBar();
    glWidget_->updateModel();
    statusBar()->showMessage("The model was uploaded successfully", 3000);
  } else {
    QMessageBox::critical(this, "Error", "Failed to upload file");
    statusBar()->showMessage("Model loading error", 3000);
  }
}

void MainWindow::translate() {
  float dx = static_cast<float>(translateXSpin_->value());
  float dy = static_cast<float>(translateYSpin_->value());
  float dz = static_cast<float>(translateZSpin_->value());

  statusBar()->showMessage("Moving the model...");
  controller_.TranslateModel(dx, dy, dz);
  glWidget_->updateModel();
  statusBar()->showMessage("The model has been moved", 3000);

  translateXSpin_->setValue(0.0);
  translateYSpin_->setValue(0.0);
  translateZSpin_->setValue(0.0);
}

void MainWindow::rotate() {
  float angleX = static_cast<float>(rotateXSpin_->value());
  float angleY = static_cast<float>(rotateYSpin_->value());
  float angleZ = static_cast<float>(rotateZSpin_->value());

  statusBar()->showMessage("Rotating the model...");
  controller_.RotateModel(angleX, angleY, angleZ);
  glWidget_->updateModel();
  statusBar()->showMessage("The model has been rotated", 3000);

  rotateXSpin_->setValue(0.0);
  rotateYSpin_->setValue(0.0);
  rotateZSpin_->setValue(0.0);
}

void MainWindow::scale() {
  float factor = static_cast<float>(scaleSpin_->value());

  statusBar()->showMessage("Scaling the model...");
  controller_.ScaleModel(factor);
  glWidget_->updateModel();
  statusBar()->showMessage("The model has been scaled", 3000);

  scaleSpin_->setValue(1.0);
}

void MainWindow::updateStatusBar() {
  fileNameLabel_->setText("File: " +
                          QString::fromStdString(controller_.GetFilename()));
  vertexCountLabel_->setText("Vertexes: " +
                             QString::number(controller_.GetVertexCount()));
  edgeCountLabel_->setText("Edges: " +
                           QString::number(controller_.GetEdgeCount()));
}

}  // namespace viewer3d