
#include "tmain.h"
#include "capturewindow.h"
#include "settingswindow.h"
#include "QHotkey/QHotkey/QHotkey"
#include "core/core_fs_check.h"

// global variable
bool kCapture_window_load;
bool kSetting_window_load;

TMain::TMain(QWidget* parent)
    : QWidget(parent),
      systemTray(nullptr),
      menu(nullptr),
      start_Capture(nullptr),
      show_Settings(nullptr),
      quit_App(nullptr),
      capture_AllScreen(nullptr),
      capture_WithCopy(nullptr),

      captureWindow(nullptr),
      settingsWindow(nullptr),

      hk_captureAllScreen(nullptr),
      hk_captureWithCopy(nullptr),
      hk_captureWindow(nullptr),
      hk_settingWindow(nullptr),
      hk_QuitApp(nullptr)
{    
    init();
}
void TMain::init()
{
    loadConfiguration();

    systemTray=new QSystemTrayIcon(this);
    systemTray->setIcon(qCore->logoPixmap());
    systemTray->setToolTip(tr("Qt screenshot tool"));
    connect(systemTray,&QSystemTrayIcon::activated,this,[&](QSystemTrayIcon::ActivationReason reason){
        if(reason==QSystemTrayIcon::Trigger){
            showCapatureWindow();
        }
    });

    menu=new QMenu(this);

    capture_AllScreen=new QAction(this);
    capture_AllScreen->setText(tr("Capture desktop"));
    connect(capture_AllScreen,&QAction::triggered,this,&TMain::captureDesktop);

    capture_WithCopy=new QAction(this);
    capture_WithCopy->setText(tr("Capture desktop and copy"));
    connect(capture_WithCopy,&QAction::triggered,this,&TMain::captureDesktopWithCopy);

    start_Capture=new QAction(this);
    start_Capture->setText(tr("General capture"));
    connect(start_Capture,&QAction::triggered,this,&TMain::captureGWindow);

    show_Settings=new QAction(this);
    show_Settings->setText(tr("Settings dialog"));
    connect(show_Settings,&QAction::triggered,this,&TMain::showSettingsDialog);

    quit_App=new QAction(this);
    quit_App->setText(tr("Quit"));
    connect(quit_App,&QAction::triggered,this,&TMain::quitApp);

    updateActionKey();

    menu->addAction(capture_AllScreen);
    menu->addAction(capture_WithCopy);
    menu->addSeparator();
    menu->addAction(start_Capture);
    menu->addAction(show_Settings);
    menu->addAction(quit_App);

    // register global hotkey
    registerHotkey();

    systemTray->setContextMenu(menu);
    qCore->setSysTray(systemTray);
    systemTray->show();
}

void TMain::loadConfiguration()
{
    // basic settings
    QString saveDirectory=QString::fromStdString(__core_settings.read("settings","save_directory").data1);
    QString fontFamliy=QString::fromStdString(__core_settings.read("settings","font_family").data1);
    bool language=__core_settings.read("settings","language").data3;
    bool start_on_boot=__core_settings.read("settings","start_on_boot").data3;
    bool shadow_effect=__core_settings.read("settings","shadow_effect").data3;
    QString image_type=QString::fromStdString(__core_settings.read("settings","image_type").data1);
    int image_quality=__core_settings.read("settings","image_quality").data2;
    int dot_size=__core_settings.read("settings","dot_size").data2;
    QString format_string=QString::fromStdString(__core_settings.read("settings","format_string").data1);
    QString border_color=QString::fromStdString(__core_settings.read("settings","border_color").data1);
    QString ui_color=QString::fromStdString(__core_settings.read("settings","ui_color").data1);
    QString dot_color=QString::fromStdString(__core_settings.read("settings","dot_color").data1);

    qCore->setDotColor(dot_color);
    qCore->setDotSize(dot_size);
    qCore->setLanguage(language);
    qCore->setSaveFileDir(saveDirectory);
    qCore->setFontFamily(fontFamliy);
    qCore->setStartBoot(start_on_boot);
    qCore->setImageQuality(image_quality);
    qCore->setImageType(image_type);
    qCore->setFilePrefixFormat(format_string);
    qCore->setShadowEffect(shadow_effect);
    qCore->setBorderColor(border_color);
    qCore->setHoverColor(ui_color);

    // keymap
    QString cap_desktop=QString::fromStdString(__core_settings.read("keymap","cap_desktop").data1);
    QString cap_desktop_copy=QString::fromStdString(__core_settings.read("keymap","cap_desktop_copy").data1);
    QString cap_usercontrol=QString::fromStdString(__core_settings.read("keymap","cap_usercontrol").data1);
    QString w_settings=QString::fromStdString(__core_settings.read("keymap","w_settings").data1);
    QString w_quit=QString::fromStdString(__core_settings.read("keymap","w_quit").data1);

    if(cap_desktop.trimmed().isEmpty()) cap_desktop="Ctrl+Shift+A";
    if(cap_desktop_copy.trimmed().isEmpty()) cap_desktop_copy="Ctrl+Shift+C";
    if(cap_usercontrol.trimmed().isEmpty()) cap_usercontrol="F1";
    if(w_settings.trimmed().isEmpty()) w_settings="Alt+Shift+S";
    if(w_quit.trimmed().isEmpty()) w_quit="Alt+Shift+Q";

    QStringList keymap={cap_desktop,cap_desktop_copy,cap_usercontrol,w_settings,w_quit};
    qCore->setKeymap(keymap);
}

void TMain::updateActionKey()
{
    capture_AllScreen->setShortcut(QKeySequence(qCore->getKeymap()[0]));
    capture_WithCopy->setShortcut(QKeySequence(qCore->getKeymap()[1]));
    start_Capture->setShortcut(QKeySequence(qCore->getKeymap()[2]));
    show_Settings->setShortcut(QKeySequence(qCore->getKeymap()[3]));
    quit_App->setShortcut(QKeySequence(qCore->getKeymap()[4]));
}

void TMain::unregisterHotkey()
{
    __LOG__("Unregister hotkey",1);

    hk_captureAllScreen->resetShortcut();
    hk_captureWithCopy->resetShortcut();
    hk_captureWindow->resetShortcut();
    hk_settingWindow->resetShortcut();
    hk_QuitApp->resetShortcut();
}

void TMain::reRegisterHotkey()
{
    __LOG__("Re-register hotkey",1);

    QStringList keymap=qCore->getKeymap();
    hk_captureAllScreen->setShortcut(QKeySequence(keymap[0]),true);
    hk_captureWithCopy->setShortcut(QKeySequence(keymap[1]),true);
    hk_captureWindow->setShortcut(QKeySequence(keymap[2]),true);
    hk_settingWindow->setShortcut(QKeySequence(keymap[3]),true);
    hk_QuitApp->setShortcut(QKeySequence(keymap[4]),true);
}

void TMain::registerHotkey()
{
    __LOG__("Register hotkey",1);

    QLoggingCategory::setFilterRules(QStringLiteral("QHotkey.warning=false"));

    // get new keymap from qCore->Object
    QStringList keymap=qCore->getKeymap();

    // capture desktop
    hk_captureAllScreen=new QHotkey(this);
    hk_captureAllScreen->setShortcut(QKeySequence(keymap[0]),true);
    connect(hk_captureAllScreen,&QHotkey::activated,this,&TMain::captureDesktop);
    // capture desktop and copy
    hk_captureWithCopy=new QHotkey(this);
    hk_captureWithCopy->setShortcut(QKeySequence(keymap[1]),true);
    connect(hk_captureWithCopy,&QHotkey::activated,this,&TMain::captureDesktopWithCopy);
    // general capture
    hk_captureWindow=new QHotkey(this);
    hk_captureWindow->setShortcut(QKeySequence(keymap[2]),true);
    connect(hk_captureWindow,&QHotkey::activated,this,&TMain::captureGWindow);
    // show settings dialog
    hk_settingWindow=new QHotkey(this);
    hk_settingWindow->setShortcut(QKeySequence(keymap[3]),true);
    connect(hk_settingWindow,&QHotkey::activated,this,&TMain::showSettingsDialog);
    // quit application
    hk_QuitApp=new QHotkey(this);
    hk_QuitApp->setShortcut(QKeySequence(keymap[4]),true);
    connect(hk_QuitApp,&QHotkey::activated,this,&TMain::quitApp);

}

void TMain::captureDesktop()
{
    qCore->grabScreen2File();
    systemTray->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to a file"));
    __LOG__("Capture all desktop",1);
}
void TMain::captureDesktopWithCopy()
{

    qCore->grabScreen2Clipboard();
    systemTray->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to the clipboard"));
    __LOG__("Capture all desktop and copy to clipboard",1);
}

void TMain::captureGWindow()
{
    if(!kCapture_window_load){
        kCapture_window_load=true;
        __LOG__("Start to capture desktop...",1);
        showCapatureWindow();
    }
}

void TMain::showSettingsDialog()
{
    if(!kCapture_window_load && !kSetting_window_load){
        kSetting_window_load=true;
        __LOG__("Show settings dialog",1);
        showSettingsWindow();
    }
}

void TMain::showCapatureWindow()
{
    captureWindow=new CaptureWindow();
    captureWindow->setAttribute(Qt::WA_DeleteOnClose);
    captureWindow->show();
}

void TMain::showSettingsWindow()
{
    settingsWindow=new SettingsWindow();
    settingsWindow->setTMain(this);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}

TMain::~TMain()
{
}

void TMain::quitApp()
{
    __LOG__("QtScreenShot exited!",1);
    qApp->exit();
}


