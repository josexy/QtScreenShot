#include "../widget/colorpanel.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"

#include "../capturewindow.h"
#include "shapetool.h"
#include <QHBoxLayout>
#include <QPainter>

ShapeTool::ShapeTool(QWidget *parent) :
    BaseTool(parent),
    __w((CaptureWindow*)parent)
{
    setFixedSize(390,50);

    initUI();
}

void ShapeTool::initUI()
{
    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hboxL->setSpacing(0);

    QHBoxLayout *hL_Options=new QHBoxLayout();
    hL_Options->setMargin(0);
    hL_Options->setContentsMargins(0,0,0,0);
    hL_Options->setSpacing(0);

    Separator *separator=new Separator(this);
    ColorPanel *colorPanel=new ColorPanel(this);
    connect(colorPanel,&ColorPanel::getColor,this,[&](const QColor &color){
        prevColor=color;
        if(__w->fill)
            qCore->setBgColor(color);
        else
            qCore->setPenColor(color);
    });
    hboxL->insertWidget(0,colorPanel);
    btnBorder=new BaseButton(qCore->svgImagePath()+QStringLiteral("border-rect.svg"),tr("Border"),this);
    btnFill=new BaseButton(qCore->svgImagePath()+QStringLiteral("filled-circle.svg"),tr("Fill"),this);
    btnRectangle=new BaseButton(qCore->svgImagePath()+QStringLiteral("rectangular.svg"),tr("Rectangle"),this);
    btnCircle=new BaseButton(qCore->svgImagePath()+QStringLiteral("circle.svg"),tr("Circle"),this);
    btnTriangle=new BaseButton(qCore->svgImagePath()+QStringLiteral("triangle.svg"),tr("Triangle"),this);
    btnRTriangle=new BaseButton(qCore->svgImagePath()+QStringLiteral("rtriangle.svg"),tr("Vertical triangle"),this);
    hL_Options->addWidget(btnBorder);
    hL_Options->addWidget(btnFill);
    hL_Options->addWidget(btnRectangle);
    hL_Options->addWidget(btnCircle);
    hL_Options->addWidget(btnTriangle);
    hL_Options->addWidget(btnRTriangle);
    hL_Options->insertWidget(2,separator);

    hboxL->addLayout(hL_Options);
    setLayout(hboxL);

    connect(btnBorder,SIGNAL(clicked(bool)),this,SLOT(on_btnBorder_clicked()));
    connect(btnFill,SIGNAL(clicked(bool)),this,SLOT(on_btnFill_clicked()));
    connect(btnRectangle,SIGNAL(clicked(bool)),this,SLOT(on_btnRectangle_clicked()));
    connect(btnCircle,SIGNAL(clicked(bool)),this,SLOT(on_btnCircle_clicked()));
    connect(btnTriangle,SIGNAL(clicked(bool)),this,SLOT(on_btnTriangle_clicked()));
    connect(btnRTriangle,SIGNAL(clicked(bool)),this,SLOT(on_btnRTriangle_clicked()));
    btnBorder->setChecked(true);
}

ShapeTool::~ShapeTool() {}

void ShapeTool::singlePressed()
{
    btnRectangle->setChecked(false);
    btnCircle->setChecked(false);
    btnTriangle->setChecked(false);
    btnRTriangle->setChecked(false);
    if(qCore->getSingleShape()==ShapeType::Rectangle)
        btnRectangle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Circle)
        btnCircle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Triangle)
        btnTriangle->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::RTriangle)
        btnRTriangle->setChecked(true);
}

void ShapeTool::on_btnRectangle_clicked()
{
    __w->shapeType=ShapeType::Rectangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnCircle_clicked()
{
    __w->shapeType=ShapeType::Circle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnTriangle_clicked()
{
    __w->shapeType=ShapeType::Triangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnRTriangle_clicked()
{
    __w->shapeType=ShapeType::RTriangle;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->penTool->singlePressed();
}

void ShapeTool::on_btnBorder_clicked()
{
    qCore->setPenColor(prevColor);
    __w->fill=false;
    btnBorder->setChecked(true);
    btnFill->setChecked(false);
}

void ShapeTool::on_btnFill_clicked()
{
    qCore->setBgColor(prevColor);
    __w->fill=true;
    btnBorder->setChecked(false);
    btnFill->setChecked(true);
}
