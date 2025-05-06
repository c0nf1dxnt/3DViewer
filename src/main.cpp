#include <QtWidgets/QApplication>

#include "controller/controller.h"
#include "model/model.h"
#include "view/mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  viewer3d::Model model;
  viewer3d::Controller controller(model);
  viewer3d::MainWindow mainWindow(controller);

  mainWindow.show();

  return app.exec();
}
