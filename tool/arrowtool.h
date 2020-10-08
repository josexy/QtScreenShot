#ifndef ARROWTOOL_H
#define ARROWTOOL_H

#include "basetool.h"

class CaptureWindow;
class BaseButton;


class ArrowTool : public BaseTool
{
    Q_OBJECT
public:

    ArrowTool(QWidget *parent=nullptr);
    void initUI();
    void singlePressed();
protected slots:
    void on_triArrow1_clicked();
    void on_lineArrow2_clicked();
private:
    CaptureWindow *__w;
    BaseButton * btnArrow1;
    BaseButton * btnArrow2;
};

#endif
