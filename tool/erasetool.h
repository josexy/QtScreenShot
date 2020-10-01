#ifndef ERASETOOL_H
#define ERASETOOL_H

#include "../core/core_system.h"
#include "basetool.h"

class CaptureWindow;
class BaseButton;
class BaseComboBox;
class EraseTool : public BaseTool
{
    Q_OBJECT
public:

    enum class EraseType{
        None,
        General,
        Region
    };

    explicit EraseTool(QWidget *parent = nullptr);

    void singlePressed();
    void initUI();
private:
    CaptureWindow *__w;
    BaseButton * btnCircleErase;
    BaseButton * btnRegionCircle;
    BaseComboBox * combEraseSize;

    EraseType eraseType;
};

#endif
