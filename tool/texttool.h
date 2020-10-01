#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "basetool.h"

class CaptureWindow;
class BaseButton;
class BaseComboBox;

class QPushButton;

class TextTool : public BaseTool
{
    Q_OBJECT

public:
    explicit TextTool(QWidget *parent = nullptr);
    ~TextTool();
    void setButtonColor(QColor);
    void initUI();

protected slots:
    void setCurrentColor();

    void on_combFontFamily_currentTextChanged(const QString &);
    void on_combFontSize_currentTextChanged(const QString &);
    void on_btnTextBold_clicked();
    void on_btnTextUnderline_clicked();
    void on_btnTextItalic_clicked();
    void on_btnTextStrikeout_clicked();

private:
    CaptureWindow *__w;

    BaseButton *btnTextBold;
    BaseButton *btnTextItalic;
    BaseButton *btnTextUnderline;
    BaseButton *btnTextStrikeout;

    BaseComboBox *combFontFamily;
    BaseComboBox *combFontSize;
    QPushButton *btnColor;
};

#endif
