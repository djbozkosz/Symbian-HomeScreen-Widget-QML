//------------------------------------------------------------------------------
#include "mainQml.h"

//------------------------------------------------------------------------------
CQmlView::CQmlView(const QString &source, QWidget *parent) : QDeclarativeView(parent), runInBackground(false)
{
  connect(engine(), SIGNAL(quit()), SLOT(close()));
  setResizeMode(QDeclarativeView::SizeRootObjectToView);

#if defined(Q_OS_SYMBIAN) || defined(QT_SIMULATOR)
  QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);
#endif

  widgetsPublisher = new CHomeScreenPublisher(this);

  widget = widgetsPublisher->createWidget(NHomeScreenWidget::TYPE_THREE_ROWS, "Qml Sis Template Widget");
  connect(widget, SIGNAL(widgetEvent(QHSEvent)), this, SLOT(widgetEvent(QHSEvent)));
  connect(widget, SIGNAL(widgetItemEvent(QString, QHSItemEvent)), this, SLOT(widgetItemEvent(QString, QHSItemEvent)));

  widget2 = widgetsPublisher->createWidget(NHomeScreenWidget::TYPE_WIDE_IMAGE, "Qml Sis Template Widget 2");
  connect(widget2, SIGNAL(widgetEvent(QHSEvent)), this, SLOT(widgetEvent(QHSEvent)));
  connect(widget2, SIGNAL(widgetItemEvent(QString, QHSItemEvent)), this, SLOT(widgetItemEvent(QString, QHSItemEvent)));

  QTimer::singleShot(100, this, SLOT(updateTime()));

  QDeclarativeView::setSource(source);
  rootContext()->setContextProperty("qmlView", this);

  QTimer::singleShot(100, this, SLOT(timerInit()));
}
//------------------------------------------------------------------------------
CQmlView::~CQmlView()
{
}
//------------------------------------------------------------------------------
void CQmlView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);

#ifdef Q_OS_SYMBIAN
  CAknAppUi *appUi = dynamic_cast<CAknAppUi*>(CEikonEnv::Static()->AppUi());
  if(appUi)
    appUi->HideApplicationFromFSW(false);
#endif
}
//------------------------------------------------------------------------------
void CQmlView::timerInit()
{
#if defined(Q_OS_SYMBIAN)
  const bool platformSymbian = true;
#elif defined(QT_SIMULATOR)
  const bool platformSymbian = false;
#endif
  if(!QMetaObject::invokeMethod(rootObject(), "setPlatformSymbian", Q_ARG(QVariant, QVariant(platformSymbian))))
    QTimer::singleShot(100, this, SLOT(timerInit()));

  QMetaObject::invokeMethod(rootObject(), "initPage");

/*#ifdef QT_SIMULATOR
  QImage i(composeWidgetImage());
  QLabel *lb = new QLabel(this);
  lb->setPixmap(QPixmap::fromImage(i));
  lb->setMaximumSize(i.width(), i.height());
  lb->setGeometry(0, 0, i.width(), i.height());
  lb->show();
#endif*/
}
//------------------------------------------------------------------------------
void CQmlView::quit(bool force)
{
#ifdef Q_OS_SYMBIAN
  CAknAppUi *appUi = dynamic_cast<CAknAppUi*>(CEikonEnv::Static()->AppUi());
  if(appUi)
    appUi->HideApplicationFromFSW((force) ? false : runInBackground);

  if((runInBackground) && (!force))
  {
    RWindowGroup *wg = &CCoeEnv::Static()->RootWin();
    wg->SetOrdinalPosition(-1);
  }
  else
#endif
    qApp->quit();
}
//------------------------------------------------------------------------------
QImage CQmlView::composeWidgetImage()
{
  QImage i(":/backgrounds/images/backgrounds/widget_belle00.png");
  QPainter p(&i);
  QFont f(p.font());
  QImage ii(":/icons/images/icons/qgn_menu_am_midlet.png");
  ii = ii.scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  p.drawImage(6, 6, ii);
  f.setPointSize(13);
  p.setFont(f);
  p.setPen(QColor(255, 255, 255, 255));
  p.drawText(85, 18, "Qml Sis Template Widget");
  p.setPen(QColor(255, 255, 255, 213));
  p.drawText(85, 37, "Custom painted, Qt 4.7.4, QML");
  p.setPen(QColor(255, 255, 255, 170));
  p.drawText(85, 56, "...");
  p.setPen(QColor(255, 255, 255, 128));
  p.drawText(85, 75, "www.djbozkosz.wz.cz");

  return i;
}
//------------------------------------------------------------------------------
void CQmlView::updateTime()
{
  QTimer::singleShot(1000, this, SLOT(updateTime()));

  widget->setItem(NHomeScreenWidget::ITEM_TEXT1, QDateTime::currentDateTime().toString("h:mm:ss d. M. yyyy"));
  widget->publish();
}
//------------------------------------------------------------------------------
void CQmlView::widgetEvent(QHSEvent event)
{
  //qDebug(QString("qml widgetEvent: %1").arg(event).toStdString().c_str());

  if((event == EActivate) || (event == EResume))
  {
    if(sender() == widget)
    {
      widget->setItem(NHomeScreenWidget::ITEM_TEXT2, "Qml Sis Template Widget");
      widget->setItem(NHomeScreenWidget::ITEM_TEXT3, "www.djbozkosz.wz.cz");
      widget->setItem(NHomeScreenWidget::ITEM_IMAGE1, QImage(":/icons/images/icons/qgn_menu_am_midlet.png"));
      widget->publish();
    }
    else if(sender() == widget2)
    {
      widget2->setItem(NHomeScreenWidget::ITEM_IMAGE1, composeWidgetImage());
      widget2->publish();
    }
  }
}
//------------------------------------------------------------------------------
void CQmlView::widgetItemEvent(QString itemName, QHSItemEvent itemEvent)
{
  //qDebug(QString("widgetEvent: %1 %2").arg(itemName).arg(itemEvent).toStdString().c_str());

  if(itemEvent == ESelect)
  {
    raise();
    emit showEvent(NULL);
  }
}
//------------------------------------------------------------------------------
