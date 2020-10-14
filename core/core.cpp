#include <QPixmap>
#include "core.h"
#include "core_system.h"
#include "core_fs_check.h"

core::core():__penSize(2),__borderWeight(4),__eraseSize(10),__dotSize(12),__ProxyType(false)
{
    __fileFormat="yyyy_MM_dd_hh_mm_ss";
    __saveDir=QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    // init UI color
    __borderColor=__widgetBgColor=QColor(51,255,255);
    __dotColor=QColor(51,0,255);
    __normalColor=Qt::white;
    __hoverColor=__widgetBgColor;

    __logoPixmap=QPixmap(svgImagePath()+"logo.svg");
    resetColor();
}

core::~core() { }

void core::resetColor()
{
    __bgColor=__penColor=Qt::red;
}

void core::setKeymap(const QStringList &keymap)
{
    __keymap=keymap;
}

const QStringList &core::getKeymap()
{
    return __keymap;
}

bool core::getStartBoot() const
{
    return __startOnBoot;
}

void core::setStartBoot(bool is)
{
    __startOnBoot=is;
}

bool core::getShadowEffect() const
{
    return __shadowEffect;
}

void core::setShadowEffect(bool is)
{
    __shadowEffect=is;
}

const QString &core::getFontFamily() const
{
    return __fontFamily;
}

void core::setFontFamily(const QString &fontFamily)
{
    __fontFamily=fontFamily;
}

bool core::getLanguage() const
{
    return __language;
}

void core::setLanguage(bool lang)
{
    __language=lang;
}

QSystemTrayIcon *core::getSysTray()
{
    return __sysTray;
}

void core::setSysTray(QSystemTrayIcon *systemTray)
{
    __sysTray=systemTray;
}

int core::getDotSize() const
{
    return __dotSize;
}

void core::setDotSize(int s)
{
    __dotSize=s;
}

QColor core::getDotColor() const
{
    return __dotColor;
}

void core::setDotColor(QColor c)
{
    __dotColor=c;
}

const QPixmap &core::logoPixmap() const
{
    return __logoPixmap;
}

void core::setEnableUpload(bool is)
{
    __enableUpload=is;
}

bool core::getEnableUpload()
{
    return __enableUpload;
}

void core::setEnableProxy(bool is)
{
    __enableProxy=is;
}

bool core::getEnableProxy()
{
    return __enableProxy;
}

void core::setProxyType(bool is)
{
    __ProxyType=is;
}

bool core::getProxyType()
{
    return __ProxyType;
}

void core::setProxyIP(const QString &ip)
{
    __IP=ip;
}

const QString &core::getProxyIP()
{
    return __IP;
}

void core::setProxyPort(int port)
{
    __Port=port;
}

int core::getProxyPort()
{
    return __Port;
}

void core::setSMMSAuthorization(const QString &auth)
{
    __Authorization=auth;
}

const QString &core::getSMMSAuthorization()
{
    return __Authorization;
}

void core::grabScreen(int x, int y, int w, int h)
{
    setPixMap(getPrimaryScreen()->grabWindow(0,x,y,w,h));
}

void core::grabScreen(const QRect &region)
{
    grabScreen(region.left(),region.top(),region.width(),region.height());
}

QString core::grabScreen2File(int x, int y, int w, int h)
{
    grabScreen(x,y,w,h);
    return PixMap2ImageFile(__pixmap);
}

void core::grabScreen2File(const QRect &region)
{
    grabScreen2File(region.left(),region.top(),region.width(),region.height());
}

void core::grabScreen2Clipboard(int x, int y, int w, int h)
{
    grabScreen(x,y,w,h);
    PixMap2ClipBoard(__pixmap);
}

void core::grabScreen2Clipboard(const QRect &region)
{
    grabScreen2Clipboard(region.left(),region.top(),region.width(),region.height());
}

void core::setBgPixmap(const QPixmap &bg)
{
    __bgPixmap=bg;
}

void core::setPixMap(const QPixmap &pixmap)
{
    __pixmap=pixmap;
}

QPixmap &core::getPixMap()
{
    return __pixmap;
}

QPixmap &core::getbgPixmap()
{
    return __bgPixmap;
}

QString core::PixMap2ImageFile(const QPixmap& pixmap)
{
    QString dts=QDateTime::currentDateTime().toString(__fileFormat);
    QString filename=__saveDir+"/"+prefix()+dts+"."+(__imageType.toLower());
    core_fs_check fsc(filename,getSaveFileDir(),this);

    filename=QDir::toNativeSeparators(filename);
    QImageWriter iw(filename);
    iw.setFormat(__imageType.toUtf8());
    
    if(__imageQuality<=0) __imageQuality=1;
    else if(__imageQuality>=100) __imageQuality=100;
    iw.setQuality(__imageQuality);
    iw.write(pixmap.toImage());

    return filename;
}

void core::PixMap2ClipBoard(const QPixmap& pixmap)
{
    QClipboard *clipBoard=QGuiApplication::clipboard();
    clipBoard->setPixmap(pixmap);
}

void core::setFilePrefixFormat(const QString &format)
{
    __fileFormat=format;
}

const QString &core::getFilePrefixFormat() const
{
    return __fileFormat;
}

QScreen *core::getPrimaryScreen()
{
    return QGuiApplication::primaryScreen();
}

QSize core::getScreenSize()
{
    return getPrimaryScreen()->size();
}

QStringList core::getCurrentUserPaths()
{
    QStringList sl;
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());
    sl.push_back(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).first());
    return sl;
}

QStringList core::getSupportedImageFormats()
{
    QStringList sl;
    for(auto x : QImageWriter::supportedImageFormats())
        sl.push_back(x.toUpper());
    return sl;
}

void core::setSaveFileDir(const QString &dir)
{
    __saveDir=dir;
}

const QString &core::getSaveFileDir()const
{
    return __saveDir;
}

int core::getPenSize() const
{
    return __penSize;
}

void core::setPenSize(int penSize)
{
    __penSize=penSize;
}

int core::getBorderWeight() const
{
    return __borderWeight;
}

void core::setBorderWeight(int borderWeight)
{
    __borderWeight=borderWeight;
}

int core::getEraseSize() const
{
    return __eraseSize;
}

void core::setEraseSize(int eraseSize)
{
    __eraseSize=eraseSize;
}

int core::getImageQuality() const
{
    return __imageQuality;
}

void core::setImageQuality(int imageQuality)
{
    __imageQuality=imageQuality;
}

const QString &core::getImageType() const
{
    return __imageType;
}

void core::setImageType(const QString &imageType)
{
    __imageType=imageType;
}

QFont& core::getFont()
{
    return __font;
}

void core::setFont(const QFont &font)
{
    __font=font;
}

QColor core::getBorderColor() const
{
    return __borderColor;
}

void core::setBorderColor(const QColor &color)
{
    __borderColor=color;
}

QColor core::getPenColor() const
{
    return __penColor;
}

void core::setPenColor(const QColor &color)
{
    __penColor=color;
}

QColor core::getBgColor() const
{
    return __bgColor;
}

void core::setBgColor(const QColor & bgcolor)
{
    __bgColor=bgcolor;
}

QColor core::getWidgetBgColor() const
{
    return __widgetBgColor;
}

void core::setWidgetBgColor(const QColor &color)
{
    __widgetBgColor=__hoverColor=color;
}

QColor core::getNormalColor() const
{
    return __normalColor;
}

void core::setNormalColor(const QColor &c)
{
    __normalColor=c;
}

QColor core::getHoverColor() const
{
    return __hoverColor;
}

void core::setHoverColor(const QColor & c)
{
    __widgetBgColor=__hoverColor=c;
}

QColor core::getPressedColor() const
{
    return __hoverColor.darker(120);
}

QColor core::getCheckedColor() const
{
    return __hoverColor.darker(140);
}

ShapeType core::getSingleShape() const
{
    return __shapeType;
}

void core::setSingleShape(ShapeType st)
{
    __shapeType=st;
}

QStringList core::getFontFamilies()
{
    return QFontDatabase().families();
}

QFont core::getAppFont()
{
    return QApplication::font();
}


