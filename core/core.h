#ifndef CORE_H
#define CORE_H

#include <QGraphicsEffect>
#include <QSystemTrayIcon>
#include <QStandardPaths>
#include <QFontDatabase>
#include <QImageWriter>
#include <QApplication>
#include <QFontMetrics>
#include <QStringList>
#include <QClipboard>
#include <QDateTime>
#include <QPixmap>
#include <QScreen>
#include <QFont>
#include <QDir>

using std::vector;

// global declare variable
extern bool kCapture_window_load;
extern bool kSetting_window_load;

enum class PaintEventNotify{
    Unknown,
    Pen,
    Shape,
    Text,
    Arrow,
    Erase,
    Blur,
    Brush
};

enum class ShapeType{
    Unknown,
    // line
    Point,
    Curve,
    Line,
    DashLine,
    // geometry
    Rectangle,
    Circle,
    Triangle,
    RTriangle,
    // text
    Text,
    // erase
    Erase,
    // arrow
    TriArrow,
    LineArrow,
    // Blur
    Blur,
    Brush
};

struct shape_object{
    QVector<QRect>rs;
    QVector<QPoint>ps;
    QStringList ts;

    ShapeType st;
    QFont font;
    QColor color;
    QColor bgcolor;
    int pensize;
    int erasesize;

    bool egeneral;
    bool fill;
    bool isblur;
};

class core : public QObject
{
public:
    core();
    ~core();

    // grab screen image
    void grabScreen(int=0,int=0,int=-1,int=-1);
    void grabScreen(const QRect&);
    QString grabScreen2File(int=0,int=0,int=-1,int=-1);
    void grabScreen2File(const QRect&);
    void grabScreen2Clipboard(int=0,int=0,int=-1,int=-1);
    void grabScreen2Clipboard(const QRect&);

    // background image
    QPixmap& getbgPixmap();
    void setBgPixmap(const QPixmap&);

    QPixmap& getPixMap();
    void setPixMap(const QPixmap&);

    // pixmap convert to image file or copy to clipboard
    QString PixMap2ImageFile(const QPixmap&);
    void PixMap2ClipBoard(const QPixmap&);

    // set save image file format name
    void setFilePrefixFormat(const QString&);
    const QString& getFilePrefixFormat() const;

    // save directory
    void setSaveFileDir(const QString&);
    const QString& getSaveFileDir() const;

    // painter pen size
    int getPenSize()const;
    void setPenSize(int=1);

    // painter region border size
    int getBorderWeight()const;
    void setBorderWeight(int);

    // painter erase size
    int getEraseSize()const;
    void setEraseSize(int);

    // image quality
    int getImageQuality()const;
    void setImageQuality(int);

    // image type: PNG/JPEG
    const QString& getImageType()const;
    void setImageType(const QString&);

    // painter font
    QFont &getFont();
    void setFont(const QFont&);

    // painter region border color
    QColor getBorderColor()const;
    void setBorderColor(const QColor&);

    // painter pen color
    QColor getPenColor()const;
    void setPenColor(const QColor&);

    // painter background color
    QColor getBgColor()const;
    void setBgColor(const QColor&);

    // tool widget background color
    QColor getWidgetBgColor() const;
    void setWidgetBgColor(const QColor&);

    // normal color
    QColor getNormalColor() const;
    void setNormalColor(const QColor &);

    // hover color
    QColor getHoverColor() const;
    void setHoverColor(const QColor&);

    // pressed color
    QColor getPressedColor() const;

    // checked color
    QColor getCheckedColor() const;

    ShapeType getSingleShape()const;
    void setSingleShape(ShapeType);

    void resetColor();

    // the keymap of QSystemTrayIcon
    void setKeymap(const QStringList &);
    const QStringList& getKeymap();

    // start on boot
    bool getStartBoot()const;
    void setStartBoot(bool);

    // enable shadow effect
    bool getShadowEffect()const;
    void setShadowEffect(bool);

    // font family
    const QString& getFontFamily()const;
    void setFontFamily(const QString &);

    // language: English/Chinese
    bool getLanguage()const;
    void setLanguage(bool);

    QSystemTrayIcon * getSysTray();
    void setSysTray(QSystemTrayIcon *);

    int getDotSize()const;
    void setDotSize(int);

    QColor getDotColor() const;
    void setDotColor(QColor);

    int brushOpacity() const {return 60;}
    int blurRadius() const {return 10;}
    int borderPadding() const {return 12;}
    QString svgImagePath() const {return QStringLiteral(":/svg/res/images/");}
    QString prefix() const {return QStringLiteral("ScreenShot_");}
    QColor maskColor() const {return QColor(0,0,0,170);}
    const QPixmap &logoPixmap() const;

    // enable upload image
    void setEnableUpload(bool);
    bool getEnableUpload();

    // enable proxy
    void setEnableProxy(bool);
    bool getEnableProxy();

    // false: HTTP/ true: Socks5
    void setProxyType(bool);
    bool getProxyType();

    // Proxy IP
    void setProxyIP(const QString &);
    const QString &getProxyIP();

    // Proxy Port
    void setProxyPort(int);
    int getProxyPort();

    void setSMMSAuthorization(const QString &);
    const QString &getSMMSAuthorization();

    static QStringList getFontFamilies();
    static QFont getAppFont();
    static QScreen *getPrimaryScreen();
    static QSize getScreenSize();
    static QStringList getCurrentUserPaths();
    static QStringList getSupportedImageFormats();

private:
    QSystemTrayIcon * __sysTray;
    QPixmap __pixmap,__bgPixmap,__cutPixmap,__logoPixmap;
    QStringList __keymap;
    QString __fileFormat,__saveDir,__imageType,__fontFamily;
    QString __Authorization,__IP;
    int __Port;

    QColor __borderColor,__penColor,__bgColor,__dotColor;
    QColor __widgetBgColor,__normalColor,__hoverColor;
    QFont __font;

    ShapeType __shapeType;

    int __imageQuality,__penSize,__borderWeight,__eraseSize,__dotSize;
    bool __startOnBoot,__shadowEffect,__language,__ProxyType,__enableUpload,__enableProxy;
};


#endif
