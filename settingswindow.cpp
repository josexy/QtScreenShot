#include "ui_settingswindow.h"
#include "settingswindow.h"
#include "widget/colorpicker.h"
#include "tmain.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTableWidget>
#include <QHeaderView>
#include <QSettings>


SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    initUI();
    initTableWidget();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initUI()
{
    setWindowIcon(QPixmap(qCore->svgImagePath()+QStringLiteral("logo.svg")));

    ui->combImageType->addItems(core::getSupportedImageFormats());
    ui->combImageType->setCurrentIndex(0);
    QSize size=__core.getScreenSize();
    move((size.width()-rect().width())/2,(size.height()-rect().height())/2);

    QValidator *validator=new QRegExpValidator(QRegExp("[0-9]\\d{0,2}",Qt::CaseInsensitive),this);
    ui->txtImageQ->setValidator(validator);
    ui->txtDotSize->setValidator(validator);

    ui->combLanguage->addItems({"English","中文"});
    ui->combLanguage->setCurrentIndex(0);

    ui->btnQtAbout->setIcon(QIcon::fromTheme("qtcreator"));
    ui->combFontFamily->addItems(core::getFontFamilies());
    ui->combFontFamily->insertItem(0,core::getAppFont().family());
    ui->combFontFamily->setCurrentIndex(0);
    ui->combSaveDir->addItems(core::getCurrentUserPaths());

    filefmts["dd_MM_yyyy"]="12_12_2019";
    filefmts["yyyy_MM_dd"]="2019_12_12";
    filefmts["hh_mm_ss"]="13_45_02";
    filefmts["yyyy_MM_dd_hh_mm_ss"]="2019_12_12_13_45_02";

    ui->combFileFormat->addItems(filefmts.keys());

    connect(ui->combFileFormat,&QComboBox::currentTextChanged,this,[&](const QString & fmt){
        ui->lbDisplayFmt->setText(qCore->prefix()+filefmts[fmt]+"."+ui->combImageType->currentText().toLower());
    });
    connect(ui->sliderImageQ,&QSlider::valueChanged,this,[&](int value){
        ui->txtImageQ->setText(QString::number(value));
    });
    ui->btnGetSaveDir->setFixedWidth(30);

    connect(ui->txtImageQ,&QLineEdit::textChanged,this,[&](const QString& text){
        ui->sliderImageQ->setValue(text.toInt());
    });

    connect(ui->btnDesktop,&QPushButton::clicked,this,[&](){
        __TMain->unregisterHotkey();
        ui->btnDesktop->setText(tr("Recording keystrokes..."));
        btnKeyMapType=ButtonKeymapType::Btn1;
        recording_keymap=!recording_keymap;
        change_hotkey=true;
    });
    connect(ui->btnDesktopCopy,&QPushButton::clicked,this,[&](){
        __TMain->unregisterHotkey();
        ui->btnDesktopCopy->setText(tr("Recording keystrokes..."));
        btnKeyMapType=ButtonKeymapType::Btn2;
        recording_keymap=!recording_keymap;
        change_hotkey=true;
    });
    connect(ui->btnFree,&QPushButton::clicked,this,[&](){
        __TMain->unregisterHotkey();
        ui->btnFree->setText(tr("Recording keystrokes..."));
        btnKeyMapType=ButtonKeymapType::Btn3;
        recording_keymap=!recording_keymap;
        change_hotkey=true;
    });

    connect(ui->btnSettings,&QPushButton::clicked,this,[&](){
        __TMain->unregisterHotkey();
        ui->btnSettings->setText(tr("Recording keystrokes..."));
        btnKeyMapType=ButtonKeymapType::Btn4;
        recording_keymap=!recording_keymap;
        change_hotkey=true;
    });
    connect(ui->btnQuit,&QPushButton::clicked,this,[&](){
        __TMain->unregisterHotkey();
        ui->btnQuit->setText(tr("Recording keystrokes..."));
        btnKeyMapType=ButtonKeymapType::Btn5;
        recording_keymap=!recording_keymap;
        change_hotkey=true;
    });

    colorDot=new ColorPicker(tr("Right click to select the dot color"),this);
    colorBorder=new ColorPicker(tr("Right click to select the border color"),this);
    colorMainUI=new ColorPicker(tr("Right click to select UI color"),this);

    ui->vLayout->addWidget(colorDot);
    ui->vLayout->addWidget(colorBorder);
    ui->vLayout->addWidget(colorMainUI);

    connect(colorDot,&ColorPicker::sendColor,this,[&](QColor c){ qCore->setDotColor(c); });
    connect(colorBorder,&ColorPicker::sendColor,this,[&](QColor c){ qCore->setBorderColor(c); });
    connect(colorMainUI,&ColorPicker::sendColor,this,[&](QColor c){ qCore->setHoverColor(c); });

    updateWidgetValue();

    installEventFilter(this);
}

void SettingsWindow::initTableWidget()
{
    QStringList header;
    header<<tr("Shortcut")<<tr("Description");
    twShortcut=new QTableWidget(8,2,this);
    twShortcut->setHorizontalHeaderLabels(header);
    twShortcut->setEditTriggers(QTableWidget::NoEditTriggers);
    twShortcut->setSelectionBehavior(QAbstractItemView::SelectRows);

    twShortcut->setItem(0,0,new QTableWidgetItem("Shift+U/D/L/R"));
    twShortcut->setItem(0,1,new QTableWidgetItem(tr("Move capture region")));

    twShortcut->setItem(1,0,new QTableWidgetItem("U/D/L/R"));
    twShortcut->setItem(1,1,new QTableWidgetItem(tr("Move pixel panel")));

    twShortcut->setItem(2,0,new QTableWidgetItem("Esc"));
    twShortcut->setItem(2,1,new QTableWidgetItem(tr("Close capture window")));

    twShortcut->setItem(3,0,new QTableWidgetItem(qCore->getKeymap()[0]));
    twShortcut->setItem(3,1,new QTableWidgetItem(tr("Capture Desktop")));

    twShortcut->setItem(4,0,new QTableWidgetItem(qCore->getKeymap()[1]));
    twShortcut->setItem(4,1,new QTableWidgetItem(tr("Capture Desktop and copy")));

    twShortcut->setItem(5,0,new QTableWidgetItem(qCore->getKeymap()[2]));
    twShortcut->setItem(5,1,new QTableWidgetItem(tr("General capture")));

    twShortcut->setItem(6,0,new QTableWidgetItem(qCore->getKeymap()[3]));
    twShortcut->setItem(6,1,new QTableWidgetItem(tr("Settings dialog")));

    twShortcut->setItem(7,0,new QTableWidgetItem(qCore->getKeymap()[4]));
    twShortcut->setItem(7,1,new QTableWidgetItem(tr("Quit")));

    twShortcut->horizontalHeader()->setStretchLastSection(true);
    twShortcut->verticalHeader()->hide();
    ui->hLayout->addWidget(twShortcut);
}

void SettingsWindow::setTMain(TMain *tmian)
{
    __TMain=tmian;
}

void SettingsWindow::qCorerestore()
{
    qCore->setSMMSAuthorization(ui->txtAuth->text());
    qCore->setFilePrefixFormat(ui->combFileFormat->currentText());
    qCore->setSaveFileDir(ui->combSaveDir->currentText());
    qCore->setLanguage(ui->combLanguage->currentIndex());
    qCore->setImageType(ui->combImageType->currentText());
    qCore->setImageQuality(ui->txtImageQ->text().toInt());
    qCore->setShadowEffect(ui->cbShadowEffect->isChecked());
    qCore->setBorderColor(colorBorder->getColor());
    qCore->setHoverColor(colorMainUI->getColor());
    qCore->setDotSize(ui->txtDotSize->text().toInt());
    qCore->setDotColor(colorDot->getColor());
    qCore->setProxyIP(ui->txtProxyIP->text());
    qCore->setProxyPort(ui->txtProxyPort->text().toUInt());
    qCore->setProxyType(ui->combType->currentIndex());
    qCore->setEnableProxy(ui->cbProxy->isChecked());
    qCore->setEnableUpload(ui->cbUpload->isChecked());
}

bool SettingsWindow::eventFilter(QObject * obj, QEvent * event)
{
    if(obj==this){
        if(event->type()==QEvent::KeyPress){
            QKeyEvent *ke=(QKeyEvent*)(event);
            QString keyName;
            if(recording_keymap){
                if(ke->modifiers()&Qt::AltModifier) keyName+="Alt+";
                if(ke->modifiers()&Qt::ControlModifier) keyName+="Ctrl+";
                if(ke->modifiers()&Qt::ShiftModifier) keyName+="Shift+";
                if(ke->modifiers()&Qt::MetaModifier) keyName+="Meta+";
                if(ke->key()>=Qt::Key_A&&ke->key()<=Qt::Key_Z)
                    keyName+=QKeySequence(ke->key()).toString();
                if(ke->key()>=Qt::Key_0&&ke->key()<=Qt::Key_9)
                    keyName+=QKeySequence(ke->key()).toString();
                if(ke->key()>=Qt::Key_F1&&ke->key()<=Qt::Key_F12)
                    keyName+=QKeySequence(ke->key()).toString();

                switch (btnKeyMapType) {
                case ButtonKeymapType::Btn1: {
                    ui->btnDesktop->setText(keyName);
                    break;
                }
                case ButtonKeymapType::Btn2: {
                    ui->btnDesktopCopy->setText(keyName);
                    break;
                }
                case ButtonKeymapType::Btn3: {
                    ui->btnFree->setText(keyName);
                    break;
                }
                case ButtonKeymapType::Btn4: {
                    ui->btnSettings->setText(keyName);
                    break;
                }
                case ButtonKeymapType::Btn5: {
                    ui->btnQuit->setText(keyName);
                    break;
                }
                default:break;
                }
                keymap[static_cast<int>(btnKeyMapType)]=keyName;
            }
        }
        if(event->type()==QEvent::KeyRelease){
            recording_keymap=false;
        }
    }
    return QWidget::eventFilter(obj,event);
}

void SettingsWindow::saveConfigurationFile()
{
    try{
        __core_settings.write("settings","save_directory",Data(ui->combSaveDir->currentText().toStdString()));
        __core_settings.write("settings","font_family",Data(ui->combFontFamily->currentText().toStdString()));
        __core_settings.write("settings","language",Data("",0,ui->combLanguage->currentIndex()==1));
        __core_settings.write("settings","image_type",Data(ui->combImageType->currentText().toStdString()));
        __core_settings.write("settings","start_on_boot",Data("",0,ui->cbAutoStartup->isChecked()));
        __core_settings.write("settings","shadow_effect",Data("",0,ui->cbShadowEffect->isChecked()));
        __core_settings.write("settings","image_quality",Data("",ui->txtImageQ->text().toInt()));
        __core_settings.write("settings","dot_size",Data("",ui->txtDotSize->text().toInt()));
        __core_settings.write("settings","format_string",Data(ui->combFileFormat->currentText().toStdString()));
        __core_settings.write("settings","border_color",Data(colorBorder->getColor().name().toStdString()));
        __core_settings.write("settings","ui_color",Data(colorMainUI->getColor().name().toStdString()));
        __core_settings.write("settings","dot_color",Data(colorDot->getColor().name().toStdString()));

        __core_settings.write("settings","smms_authorization",Data(ui->txtAuth->text().toStdString()));

        __core_settings.write("settings","enable_upload",Data("",0.0,ui->cbUpload->isChecked()));
        __core_settings.write("settings","enable_proxy",Data("",0.0,ui->cbProxy->isChecked()));
        // proxy
        __core_settings.write("proxies","type",Data(ui->combType->currentIndex()==0?"http":"socks5"));
        __core_settings.write("proxies","ip",Data(ui->txtProxyIP->text().toStdString()));
        __core_settings.write("proxies","port",Data("",ui->txtProxyPort->text().toInt()));


        if(keymap[0].size() && keymap[1].size() && keymap[2].size() && keymap[3].size() && keymap[4].size()){
            __core_settings.write("keymap","cap_desktop",Data(keymap[0].toStdString()));
            __core_settings.write("keymap","cap_desktop_copy",Data(keymap[1].toStdString()));
            __core_settings.write("keymap","cap_usercontrol",Data(keymap[2].toStdString()));
            __core_settings.write("keymap","w_settings",Data(keymap[3].toStdString()));
            __core_settings.write("keymap","w_quit",Data(keymap[4].toStdString()));
        }
        __core_settings.flush();
    }catch(JsonException &err){
        __LOG__(err.what(),3);
    }

    qCorerestore();
}

void SettingsWindow::updateWidgetValue()
{    
    // update basic settings
    ui->combSaveDir->setCurrentText(qCore->getSaveFileDir());
    ui->combFontFamily->setCurrentText(qCore->getFontFamily());
    ui->combLanguage->setCurrentIndex(qCore->getLanguage());
    ui->cbAutoStartup->setChecked(qCore->getStartBoot());
    ui->cbShadowEffect->setChecked(qCore->getShadowEffect());

    ui->txtImageQ->setText(QString::number(qCore->getImageQuality()));
    ui->txtDotSize->setText(QString::number(qCore->getDotSize()));
    ui->combFileFormat->setCurrentText(qCore->getFilePrefixFormat());
    ui->sliderImageQ->setValue(qCore->getImageQuality());
    ui->lbDisplayFmt->setText(qCore->prefix()+filefmts[qCore->getFilePrefixFormat()]+"."+qCore->getImageType().toLower());

    ui->combImageType->setCurrentText(qCore->getImageType());

    colorDot->setColor(qCore->getDotColor());
    colorBorder->setColor(qCore->getBorderColor());
    colorMainUI->setColor(qCore->getHoverColor());

    ui->cbUpload->setChecked(qCore->getEnableUpload());
    ui->cbProxy->setChecked(qCore->getEnableProxy());

    ui->txtAuth->setText(qCore->getSMMSAuthorization());
    // proxy
    ui->txtProxyIP->setText(qCore->getProxyIP());
    ui->txtProxyPort->setText(QString::number(qCore->getProxyPort()));
    ui->combType->setCurrentIndex(qCore->getProxyType());

    // update keymap
    keymap=qCore->getKeymap();
    ui->btnDesktop->setText(keymap[0]);
    ui->btnDesktopCopy->setText(keymap[1]);
    ui->btnFree->setText(keymap[2]);
    ui->btnSettings->setText(keymap[3]);
    ui->btnQuit->setText(keymap[4]);

    qCorerestore();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    kSetting_window_load=false;
    __LOG__("Close settings window",1);
}

void SettingsWindow::on_btnQtAbout_clicked()
{
    __LOG__("Show About Qt dialog",1);
    QMessageBox::aboutQt(this);
}

void SettingsWindow::on_combFontFamily_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    setFont(QFont(ui->combFontFamily->currentText()));
}

void SettingsWindow::on_btnSaveConfig_clicked()
{

    if( ui->cbUpload->isChecked() && ui->txtAuth->text().trimmed().isEmpty()){
        QMessageBox::critical(this,tr("Error"),tr("SMMS Authorization not empty!"));
        return;
    }

    recording_keymap=false;
    saveConfigurationFile();
    qCore->setKeymap(keymap);

    __TMain->updateActionKey();
    if(change_hotkey){
        __TMain->reRegisterHotkey();
        change_hotkey=false;
    }

#ifdef Q_OS_WIN
    // auto start on windows
    __LOG__("Windows: Start on boot",1);
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QApplication::applicationName();
    if (ui->cbAutoStartup->isChecked()){
        QString strAppPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue(appName,strAppPath);
    } else {
        reg.remove(appName);
    }
#endif
    __LOG__("Save configuration to json file",1);
    QMessageBox::information(this,tr("Information"),tr("Settings saved successfully!"));
}

void SettingsWindow::on_btnGetSaveDir_clicked()
{
    __LOG__("Get current saving directory by user",1);
    QString Dir=QFileDialog::getExistingDirectory(this);
    if(Dir.size()) ui->combSaveDir->setCurrentText(Dir);
}

