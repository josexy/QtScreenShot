
#include "erasetool.h"
#include "../capturewindow.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"
#include "../widget/basecombobox.h"
#include <QHBoxLayout>
#include <QPainter>

EraseTool::EraseTool(QWidget *parent)
    : BaseTool(parent),
      __w((CaptureWindow*)parent),
      eraseType(EraseType::None)
{
    setFixedSize(150,50);

    initUI();
}

void EraseTool::initUI()
{
    QHBoxLayout* hBoxLayout=new QHBoxLayout(this);
    hBoxLayout->setMargin(0);
    hBoxLayout->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hBoxLayout->setSpacing(0);

    btnCircleErase=new BaseButton(qCore->svgImagePath()+"erase-rectangular0.svg",tr("General erase"),this);
    btnRegionCircle=new BaseButton(qCore->svgImagePath()+"erase-rectangular.svg",tr("Region erase"),this);
    combEraseSize=new BaseComboBox(tr("Erase size"),this);
    for(int i=10; i<=50; i+=10) combEraseSize->addItem(QString::number(i));

    hBoxLayout->addWidget(combEraseSize);
    hBoxLayout->addWidget(new Separator(this));
    hBoxLayout->addWidget(btnCircleErase);
    hBoxLayout->addWidget(btnRegionCircle);

    setLayout(hBoxLayout);

    connect(combEraseSize,&BaseComboBox::currentTextChanged,this,[&](){
        qCore->setEraseSize(combEraseSize->currentText().toInt());
        __w->eraseSize=qCore->getEraseSize();
    });
    connect(btnCircleErase,&BaseButton::clicked,this,[&](){
        eraseType=EraseType::General;
        __w->egeneral=true;
        __w->shapeType=ShapeType::Erase;
        singlePressed();
    });
    connect(btnRegionCircle,&BaseButton::clicked,this,[&](){
        eraseType=EraseType::Region;
        __w->egeneral=false;
        __w->shapeType=ShapeType::Erase;
        singlePressed();
    });
}

void EraseTool::singlePressed()
{
    btnCircleErase->setChecked(false);
    btnRegionCircle->setChecked(false);
    if(eraseType==EraseType::General)
        btnCircleErase->setChecked(true);
    else if(eraseType==EraseType::Region)
        btnRegionCircle->setChecked(true);
}
