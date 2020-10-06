#ifndef PENTOOL_H
#define PENTOOL_H

#include "basetool.h"

class CaptureWindow;
class QPushButton;
class ColorPanel;
class BaseButton;

class PenTool : public BaseTool
{
    Q_OBJECT

public:
    explicit PenTool(QWidget *parent = nullptr);
    ~PenTool();
    void singlePressed();
    void initUI();

protected slots:
    void on_btnPoint_clicked();
    void on_btnLine_clicked();
    void on_btnDashLine_clicked();
    void on_btnCurve_clicked();
    void on_combPenSize_currentTextChanged(const QString &);

private:
    CaptureWindow *__w;
    QPushButton *btnMainColor;
    ColorPanel *colorPanel;

    BaseButton *btnPoint;
    BaseButton *btnLine;
    BaseButton *btnCurve;
    BaseButton *btnDashLine;
};

#endif
