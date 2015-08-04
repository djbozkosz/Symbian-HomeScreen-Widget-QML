//------------------------------------------------------------------------------
#include "mainQml.h"

//------------------------------------------------------------------------------
Q_DECL_EXPORT int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  CQmlView qmlView(QLatin1String("qml/Main.qml"));
  qmlView.showFullScreen();
  qmlView.raise();

  return app.exec();
}
//------------------------------------------------------------------------------
