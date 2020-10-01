#include "arrowtool.h"
#include "../capturewindow.h"
#include "../widget/basebutton.h"

ArrowTool::ArrowTool(QWidget *parent)
    : BaseTool(parent),
    __w((CaptureWindow*)parent)

{
    setFixedSize(50,50);
    initUI();
}

void ArrowTool::initUI()
{
    QHBoxLayout *hBoxlayout=new QHBoxLayout(this);
    hBoxlayout->setMargin(0);
    hBoxlayout->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hBoxlayout->setSpacing(0);

    btnArrow1=new BaseButton(qCore->svgImagePath()+"tri-arrow.svg",tr("tri arrow"),this);

    btnArrow1->setChecked(true);
    hBoxlayout->addWidget(btnArrow1);
    setLayout(hBoxlayout);

    connect(btnArrow1,&BaseButton::clicked,this,&ArrowTool::on_triArrow1_clicked);
}

void ArrowTool::singlePressed()
{
    btnArrow1->setChecked(false);
    switch (qCore->getSingleShape()){
    case ShapeType::TriArrow: btnArrow1->setChecked(true);break;
    default:break;
    }
}

void ArrowTool::on_triArrow1_clicked()
{
    __w->shapeType=ShapeType::TriArrow;
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
}

