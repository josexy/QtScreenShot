#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "core/core.h"
#include "core/core_settings.h"
#include "core/core_system.h"
#include <QWidget>

namespace Ui {
class SettingsWindow;
}

class TMain;
class ColorPicker;
class QTableWidget;

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    enum class ButtonKeymapType {
        Btn1,
        Btn2,
        Btn3,
        Btn4,
        Btn5
    };

    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
    void setTMain(TMain *);
    void closeEvent(QCloseEvent *);
    void saveConfigurationFile();
    void qCorerestore();
    bool eventFilter(QObject*,QEvent*);
    void updateWidgetValue();
    void initUI();
    void initTableWidget();

protected slots:
    void on_btnQtAbout_clicked();
    void on_combFontFamily_currentIndexChanged(int);
    void on_btnSaveConfig_clicked();
    void on_btnGetSaveDir_clicked();

private:
    Ui::SettingsWindow *ui;
    core __core;
    core_settings __core_settings;

    TMain *__TMain;
    QMap<QString,QString>filefmts;
    QStringList keymap;
    ButtonKeymapType btnKeyMapType;

    ColorPicker *colorDot;
    ColorPicker *colorBorder;
    ColorPicker *colorMainUI;
    QTableWidget *twShortcut;
    bool recording_keymap=false,change_hotkey=false;

};

#endif
