//------------------------------------------------------------------------------
#ifndef MAINQML_H
#define MAINQML_H

#include <QApplication>

#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include <QDeclarativeItem>

#include <QShowEvent>
#include <QThread>
#include <QPainter>
#include <QPaintDevice>
#include <QLabel>
#include <QColor>
#include <QFont>

#include <QDir>
#include <QFileInfo>
#include <QResizeEvent>
#include <QObjectList>
#include <QList>
#include <QTimer>
#include <QDateTime>
#include <QByteArray>
#include <QUrl>

#ifdef Q_OS_SYMBIAN
#include <apgtask.h>
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

#include "homeScreenWidget.h"

//------------------------------------------------------------------------------
class CQmlView : public QDeclarativeView
{
  private:
    Q_OBJECT

    bool runInBackground;

    CHomeScreenPublisher *widgetsPublisher;
    CHomeScreenWidget *widget;
    CHomeScreenWidget *widget2;

  protected:
    virtual void showEvent(QShowEvent *event);

  private slots:
    void timerInit();

  public:
    explicit CQmlView(const QString &source = QString(), QWidget *parent = NULL);
    virtual ~CQmlView();

    Q_INVOKABLE void quit(bool force);
    Q_INVOKABLE inline void setRunInBackground(bool enable) { runInBackground = enable; }

    QImage composeWidgetImage();

    Q_INVOKABLE inline bool isRunInBackground() const { return runInBackground; }

  public slots:
    void updateTime();
    void widgetEvent(QHSEvent event);
    void widgetItemEvent(QString itemName, QHSItemEvent itemEvent);
};
//------------------------------------------------------------------------------
#endif // MAINQML_H
