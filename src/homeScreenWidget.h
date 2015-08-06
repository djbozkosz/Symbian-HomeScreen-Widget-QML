//------------------------------------------------------------------------------
#ifndef HOMESCREENWIDGET_H
#define HOMESCREENWIDGET_H

// version 2015.07.28

#include <QObject>
#include <QString>
#include <QLibrary>
#include <QList>
#include <QImage>
#include <exception>
#include <stdexcept>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <aknsutils.h>
#include <aknsdrawutils.h>
#endif

//------------------------------------------------------------------------------
namespace NHomeScreenWidget
{
  static const uint IMAGE_SMALL_WIDTH = 70;
  static const uint IMAGE_SMALL_HEIGHT = 70;
  static const uint IMAGE_BIG_WIDTH = 312;
  static const uint IMAGE_BIG_HEIGHT = 82;

  static const uint TYPES_COUNT = 5;
  static const uint ITEM_TYPES_COUNT = 4;

  static const char STR_TYPE_WIDE_IMAGE[] = "wideimage";
  static const char STR_TYPE_ONE_ROW[] = "onerow";
  static const char STR_TYPE_TWO_ROWS[] = "tworows";
  static const char STR_TYPE_THREE_ROWS[] = "threerows";
  static const char STR_TYPE_THREE_TEXT_ROWS[] = "threetextrows";

  static const char STR_ITEM_TEXT1[] = "text1";
  static const char STR_ITEM_TEXT2[] = "text2";
  static const char STR_ITEM_TEXT3[] = "text3";
  static const char STR_ITEM_IMAGE1[] = "image1";

  static const char *const STR_TYPES[TYPES_COUNT] = { STR_TYPE_WIDE_IMAGE, STR_TYPE_ONE_ROW, STR_TYPE_TWO_ROWS, STR_TYPE_THREE_ROWS, STR_TYPE_THREE_TEXT_ROWS };
  static const char *const STR_ITEM_TYPES[ITEM_TYPES_COUNT] = { STR_ITEM_TEXT1, STR_ITEM_TEXT2, STR_ITEM_TEXT3, STR_ITEM_IMAGE1 };

  enum EType
  {
    TYPE_WIDE_IMAGE = 0,
    TYPE_ONE_ROW,
    TYPE_TWO_ROWS,
    TYPE_THREE_ROWS,
    TYPE_THREE_TEXT_ROWS
  };

  enum EItemType
  {
    ITEM_TEXT1 = 0,
    ITEM_TEXT2,
    ITEM_TEXT3,
    ITEM_IMAGE1
  };
}
//------------------------------------------------------------------------------
enum QHSEvent
{
  EUnknown = 0,
  EActivate,
  EDeactivate,
  ESuspend,
  EResume
};

enum QHSItemEvent
{
  EUnknownItemEvent = 0,
  ESelect
};
//------------------------------------------------------------------------------
/*struct SHomeScreenWidgetData
{
  QString text1;
  QString text2;
  QString text3;
  QString image1;
};*/
//------------------------------------------------------------------------------
class QHSWidget : public QObject
{
  private:
    Q_OBJECT

    inline QHSWidget(QString type, QString name, QString id, QObject *parent = NULL) : QObject(parent) { Q_UNUSED(type); Q_UNUSED(name); Q_UNUSED(id); }

  public:
    virtual inline ~QHSWidget() {}

    virtual void RegisterWidget() = 0;
    virtual void PublishWidget() = 0;
    virtual void SetItem(QString item, QString value) = 0;
    virtual void SetItem(QString item, int value) = 0;
    virtual void RemoveWidget() = 0;
    virtual QString WidgetName() = 0;

  signals:
    void handleEvent(QHSWidget *sender, QHSEvent event);
    void handleItemEvent(QHSWidget *sender, QString itemName, QHSItemEvent itemEvent);

  public:
    typedef QHSWidget *(*createHSWidget)(QString, QString, QString, QObject *);

    static QHSWidget *create(QString type, QString name, QString id, QObject *parent = NULL);
};
//------------------------------------------------------------------------------
class CHomeScreenWidget : public QObject
{
  private:
    Q_OBJECT

    QHSWidget *widget;
#ifdef Q_OS_SYMBIAN
    CFbsBitmap *image;
#else
    void *image;
#endif

  private slots:
    inline void handleEvent(QHSWidget *sender, QHSEvent event) { Q_UNUSED(sender); emit widgetEvent(event); }
    inline void handleItemEvent(QHSWidget *sender, QString itemName, QHSItemEvent itemEvent) { Q_UNUSED(sender); emit widgetItemEvent(itemName, itemEvent); }

  public:
    CHomeScreenWidget(NHomeScreenWidget::EType type, QString name, QString id, QObject *parent = NULL);
    virtual ~CHomeScreenWidget();

    inline void publish() { if(widget) widget->PublishWidget(); }
    inline void setItem(NHomeScreenWidget::EItemType item, QString value) { if(widget) widget->SetItem(NHomeScreenWidget::STR_ITEM_TYPES[item], value); }
    inline void setItem(NHomeScreenWidget::EItemType item, int value) { if(widget) widget->SetItem(NHomeScreenWidget::STR_ITEM_TYPES[item], value); }
    void setItem(NHomeScreenWidget::EItemType item, const QImage &value);
    inline void remove() { if(widget) widget->RemoveWidget(); }
    inline QString name() { if(widget) return widget->WidgetName(); return QString(); }
    inline QHSWidget *hsWidget() { return widget; }

  signals:
    void widgetEvent(QHSEvent event);
    void widgetItemEvent(QString itemName, QHSItemEvent itemEvent);
};
//------------------------------------------------------------------------------
class CHomeScreenPublisher : public QObject
{
  private:
    Q_OBJECT

    QList<CHomeScreenWidget *> widgets;

  public:
    inline CHomeScreenPublisher(QObject *parent = NULL) : QObject(parent) {}
    virtual inline ~CHomeScreenPublisher() {}

    CHomeScreenWidget *createWidget(NHomeScreenWidget::EType type, QString name);
};
//------------------------------------------------------------------------------
inline CHomeScreenWidget *CHomeScreenPublisher::createWidget(NHomeScreenWidget::EType type, QString name)
{
  CHomeScreenWidget *w = new CHomeScreenWidget(type, name, QString("%1_%2").arg(TARGETUID3).arg(widgets.size()), this);
  widgets.push_back(w);

  return *(&widgets.back());
}
//------------------------------------------------------------------------------
inline QHSWidget *QHSWidget::create(QString type, QString name, QString id, QObject *parent)
{
#ifdef Q_OS_SYMBIAN
  QHSWidget *widget = NULL;
  createHSWidget c;
  QLibrary lib(HSWIDGETLIB);

  if(!lib.load())
    throw std::runtime_error("Unable to load library!");

  if(!(c = reinterpret_cast<createHSWidget>(lib.resolve("5"))))
  {
    lib.unload();
    throw std::runtime_error("Unable to load entry!");
  }

  if(!(widget = c(type, name, id, parent)))
  {
    lib.unload();
    throw std::runtime_error("Unable to create widget!");
  }

  lib.setParent(widget);

  return widget;
#else
  return NULL;
#endif
}
//------------------------------------------------------------------------------
inline CHomeScreenWidget::CHomeScreenWidget(NHomeScreenWidget::EType type, QString name, QString id, QObject *parent) : QObject(parent), widget(NULL), image(NULL)
{
  try
  {
    widget = QHSWidget::create(NHomeScreenWidget::STR_TYPES[type], name, id, this);
    if(!widget)
      throw std::runtime_error("Unable to create widget!");
    widget->RegisterWidget();

#ifdef Q_OS_SYMBIAN
    image = new (ELeave) CFbsBitmap;
    if(!image)
      throw std::runtime_error("Unable to create widget image!");
    if(image->Create(
      (type == NHomeScreenWidget::TYPE_WIDE_IMAGE) ?
        TSize(NHomeScreenWidget::IMAGE_BIG_WIDTH, NHomeScreenWidget::IMAGE_BIG_HEIGHT) :
        TSize(NHomeScreenWidget::IMAGE_SMALL_WIDTH, NHomeScreenWidget::IMAGE_SMALL_HEIGHT),
      CEikonEnv::Static()->DefaultDisplayMode()) != KErrNone)
      throw std::runtime_error("Unable to create widget bitmap!");
#endif

    connect(widget, SIGNAL(handleEvent(QHSWidget *, QHSEvent)), this, SLOT(handleEvent(QHSWidget *, QHSEvent)));
    connect(widget, SIGNAL(handleItemEvent(QHSWidget *, QString, QHSItemEvent)), this, SLOT(handleItemEvent(QHSWidget *, QString, QHSItemEvent)));
  }
  catch(std::exception &e)
  {
    qDebug(e.what());
  }
}
//-----------------------------------------------------------------------------
inline CHomeScreenWidget::~CHomeScreenWidget()
{
#ifdef Q_OS_SYMBIAN
  if(image)
    delete image;
#endif
}
//-----------------------------------------------------------------------------
inline void CHomeScreenWidget::setItem(NHomeScreenWidget::EItemType item, const QImage &value)
{
#ifdef Q_OS_SYMBIAN
  if(!image)
    return;

  QImage i = value.scaled(image->SizeInPixels().iWidth, image->SizeInPixels().iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  i = i.convertToFormat(QImage::Format_ARGB32);
  image->BeginDataAccess();
  memcpy(image->DataAddress(), i.constBits(), image->DataSize());
  image->EndDataAccess();
  setItem(item, image->Handle());
#endif
}
//-----------------------------------------------------------------------------
#endif // HOMESCREENWIDGET_H
