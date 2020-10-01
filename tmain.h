#ifndef TMAIN_H
#define TMAIN_H

#include "core/core.h"
#include "core/core_settings.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class QHotkey;
class SettingsWindow;
class CaptureWindow;

class TMain : public QWidget
{
    Q_OBJECT
public:
    explicit TMain(QWidget *parent=nullptr);
    ~TMain();

    void unregisterHotkey();
    void registerHotkey();
    void reRegisterHotkey();

    void updateActionKey();
    void loadConfiguration();
    void init();

protected slots:
    void showCapatureWindow();
    void showSettingsWindow();
    void quitApp();

    void showSettingsDialog();
    void captureDesktopWithCopy();
    void captureGWindow();
    void captureDesktop();
private:
    core_settings __core_settings;

    QSystemTrayIcon *systemTray;
    QMenu *menu;
    QAction *start_Capture;
    QAction *show_Settings;
    QAction *quit_App;
    QAction *capture_AllScreen;
    QAction *capture_WithCopy;

    CaptureWindow *captureWindow;
    SettingsWindow *settingsWindow;

    QHotkey *hk_captureAllScreen;
    QHotkey *hk_captureWithCopy;
    QHotkey *hk_captureWindow;
    QHotkey *hk_settingWindow;
    QHotkey *hk_QuitApp;

};

#endif
