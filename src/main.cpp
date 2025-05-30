#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "controller/controller.h"
#include "model/model.h"
#include "view/mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QApplication::setApplicationName("3DViewer");
  QApplication::setApplicationVersion("2.1");

  QCommandLineParser parser;
  parser.setApplicationDescription(
      "3DViewer - a program for viewing 3D models");
  QCommandLineOption helpOption(QStringList() << "h" << "help", "Shows help.");
  parser.addOption(helpOption);
  parser.addVersionOption();

  QCommandLineOption fileOption(QStringList() << "f" << "file",
                                "Uploads the model file at startup.",
                                "filename");
  parser.addOption(fileOption);

  parser.process(app);

  if (parser.isSet(helpOption)) {
    parser.showHelp();
    return 0;
  }

  viewer3d::Model model;
  viewer3d::Controller controller(model);
  viewer3d::MainWindow mainWindow(controller);

  if (parser.isSet(fileOption)) {
    QString filename = parser.value(fileOption);
    if (!controller.LoadModel(filename.toStdString())) {
      qWarning() << "Failed to upload file:" << filename;
    }
  }

  mainWindow.show();
  return app.exec();
}
