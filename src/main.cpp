#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "controller/controller.h"
#include "model/model.h"
#include "view/mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QApplication::setApplicationName("3DViewer");
  QApplication::setApplicationVersion("2.0");

  QCommandLineParser parser;
  parser.setApplicationDescription(
      "3D Viewer - программа для просмотра 3D моделей");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption fileOption(QStringList() << "f" << "file",
                                "Загрузить файл модели при запуске",
                                "filename");
  parser.addOption(fileOption);

  parser.process(app);

  viewer3d::Model model;
  viewer3d::Controller controller(model);
  viewer3d::MainWindow mainWindow(controller);

  if (parser.isSet(fileOption)) {
    QString filename = parser.value(fileOption);
    if (!controller.LoadModel(filename.toStdString())) {
      qWarning() << "Не удалось загрузить файл:" << filename;
    }
  }

  mainWindow.show();
  return app.exec();
}
