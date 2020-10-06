#include "../widget/colorpanel.h"
#include "../widget/separator.h"
#include "../widget/basebutton.h"
#include "../widget/basecombobox.h"
#include "../capturewindow.h"
#include "pentool.h"

PenTool::PenTool(QWidget *parent) :
    BaseTool(parent),
    __w((CaptureWindow*)parent)
{
    setFixedSize(350,50);

    initUI();
}

PenTool::~PenTool() {}

void PenTool::initUI()
{
    // combobox only input number
    QValidator *validator=new QRegExpValidator(QRegExp("[0-9]\\d{0,1}",Qt::CaseInsensitive),this);
    BaseComboBox *combPenSize=new BaseComboBox(tr("Pen size"),this);
    combPenSize->setValidator(validator);
    combPenSize->setFixedWidth(50);
    for(int i=1; i<10; i+=1) combPenSize->addItem(QString::number(i));
    for(int i=10; i<30; i+=4) combPenSize->addItem(QString::number(i));
    for(int i=30; i<50; i+=5) combPenSize->addItem(QString::number(i));
    for(int i=50; i<=72; i+=6) combPenSize->addItem(QString::number(i));
    combPenSize->setCurrentIndex(1);
    combPenSize->setEditable(true);
    combPenSize->setMaxVisibleItems(10);

    connect(combPenSize,&QComboBox::currentTextChanged,this,&PenTool::on_combPenSize_currentTextChanged);

    colorPanel=new ColorPanel(this);
    connect(colorPanel,&ColorPanel::getColor,this,[&](const QColor &color){
        qCore->setPenColor(color);
    });

    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,0,qCore->borderPadding()/2,0);
    hboxL->setSpacing(0);

    QHBoxLayout *hL_Options=new QHBoxLayout();
    hboxL->addWidget(colorPanel);
    hboxL->addWidget(combPenSize);
    hboxL->addWidget(new Separator(this));

    hL_Options->setMargin(0);
    hL_Options->setContentsMargins(0,0,0,0);
    hL_Options->setSpacing(0);

    btnPoint=new BaseButton(qCore->svgImagePath()+QStringLiteral("dots.svg"),tr("Point"),this);
    btnLine=new BaseButton(qCore->svgImagePath()+QStringLiteral("line.svg"),tr("Line"),this);
    btnDashLine=new BaseButton(qCore->svgImagePath()+QStringLiteral("dashline.svg"),tr("Dash Line"),this);
    btnCurve=new BaseButton(qCore->svgImagePath()+QStringLiteral("curve.svg"),tr("Curve"),this);

    hL_Options->addWidget(btnPoint);
    hL_Options->addWidget(btnLine);
    hL_Options->addWidget(btnDashLine);
    hL_Options->addWidget(btnCurve);

    hboxL->addLayout(hL_Options);
    setLayout(hboxL);

    connect(btnPoint,&BaseButton::clicked,this,&PenTool::on_btnPoint_clicked);
    connect(btnLine,&BaseButton::clicked,this,&PenTool::on_btnLine_clicked);
    connect(btnDashLine,&BaseButton::clicked,this,&PenTool::on_btnDashLine_clicked);
    connect(btnCurve,&BaseButton::clicked,this,&PenTool::on_btnCurve_clicked);
}

void PenTool::singlePressed()
{
    btnPoint->setChecked(false);
    btnLine->setChecked(false);
    btnCurve->setChecked(false);
    btnDashLine->setChecked(false);
    if(qCore->getSingleShape()==ShapeType::Point)
        btnPoint->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Line)
        btnLine->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::Curve)
        btnCurve->setChecked(true);
    else if(qCore->getSingleShape()==ShapeType::DashLine)
        btnDashLine->setChecked(true);
}

void PenTool::on_btnDashLine_clicked()
{
    __w->shapeType=ShapeType::DashLine;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->shapeTool->singlePressed();
}

void PenTool::on_btnPoint_clicked()
{
    __w->shapeType=ShapeType::Point;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->shapeTool->singlePressed();
}

void PenTool::on_btnLine_clicked()
{
    __w->shapeType=ShapeType::Line;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->shapeTool->singlePressed();
}

void PenTool::on_btnCurve_clicked()
{
    __w->shapeType=ShapeType::Curve;
    __w->setCursor(Qt::CrossCursor);
    qCore->setSingleShape(__w->shapeType);
    singlePressed();
    __w->shapeTool->singlePressed();
}

void PenTool::on_combPenSize_currentTextChanged(const QString &text)
{
    if(text.isEmpty()) return;
    int penSize=text.toInt();
    if(penSize <= 0) qCore->setPenSize(0);
    else if(penSize >= 30) qCore->setPenSize(30);
    else qCore->setPenSize(penSize);
}
