#include "colorpanel.h"

#include <QColorDialog>
#include <QGridLayout>
#include <QPainter>

QString styleSheetFmt = "QPushButton{"
                        "border:1px solid black;"
                        "background-color:%1;"
                        "}"
                        "QPushButton:hover{"
                        "border:2px solid black;"
                        "}"
                        "QPushButton:pressed{"
                        "border:3px solid black;"
                        "}";
QList<QString> keys={"white","red","green","blue","black","yellow","gray","orange"};
QMap<QString,QColor> mapColors;

ColorPanel::ColorPanel(QWidget *parent) :QWidget(parent)
{
    initUI();
}

void ColorPanel::initUI()
{
    setCursor(Qt::PointingHandCursor);
    setWindowFlag(Qt::FramelessWindowHint);
    setContextMenuPolicy(Qt::NoContextMenu);

    setFixedSize(140,50);

    mapColors[keys[0]]=Qt::white;
    mapColors[keys[1]]=Qt::red;
    mapColors[keys[2]]=Qt::green;
    mapColors[keys[3]]=Qt::blue;
    mapColors[keys[4]]=Qt::black;
    mapColors[keys[5]]=Qt::yellow;
    mapColors[keys[6]]=Qt::gray;
    mapColors[keys[7]]=QColor(231,101,26);


    btnMainColor=new QPushButton(this);
    setButtonColor(qCore->getPenColor());
    QPushButton *btn1=new QPushButton(this);
    QPushButton *btn2=new QPushButton(this);
    QPushButton *btn3=new QPushButton(this);
    QPushButton *btn4=new QPushButton(this);
    QPushButton *btn5=new QPushButton(this);
    QPushButton *btn6=new QPushButton(this);
    QPushButton *btn7=new QPushButton(this);
    QPushButton *btn8=new QPushButton(this);

    int nSize=16;
    QVector<QPushButton*>btns={btn1,btn2,btn3,btn4,btn5,btn6,btn7,btn8};
    for(int i=0; i<btns.size(); i++){
        btns[i]->setStyleSheet(styleSheetFmt.arg(keys[i]));
        btns[i]->setFixedSize(nSize,nSize);
    }
    QHBoxLayout *hboxLayout=new QHBoxLayout(this);
    hboxLayout->setSizeConstraint(QHBoxLayout::SetFixedSize);
    hboxLayout->setContentsMargins(1,1,1,1);
    hboxLayout->setSpacing(1);
    hboxLayout->setMargin(0);

    QGridLayout *colorGrid=new QGridLayout();

    btnMainColor->setFixedSize(nSize+21,nSize+21);
    colorGrid->addWidget(btnMainColor,0,0,2,2);
    colorGrid->addWidget(btn1,0,2,1,1);
    colorGrid->addWidget(btn2,0,3,1,1);
    colorGrid->addWidget(btn3,0,4,1,1);
    colorGrid->addWidget(btn4,0,5,1,1);
    colorGrid->addWidget(btn5,1,2,1,1);
    colorGrid->addWidget(btn6,1,3,1,1);
    colorGrid->addWidget(btn7,1,4,1,1);
    colorGrid->addWidget(btn8,1,5,1,1);

    hboxLayout->addLayout(colorGrid);

    connect(btnMainColor,&QPushButton::clicked,this,&ColorPanel::colorChangedEvent);
    connect(btn1,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[0]]);});
    connect(btn2,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[1]]);});
    connect(btn3,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[2]]);});
    connect(btn4,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[3]]);});
    connect(btn5,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[4]]);});
    connect(btn6,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[5]]);});
    connect(btn7,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[6]]);});
    connect(btn8,&QPushButton::clicked,this,[&](){setButtonColor(mapColors[keys[7]]);});

    btnMainColor->setCursor(Qt::PointingHandCursor);

    setLayout(hboxLayout);

}
ColorPanel::~ColorPanel()
{
}

void ColorPanel::colorChangedEvent()
{
    QColor color=QColorDialog::getColor(Qt::red,this,QString(),QColorDialog::ShowAlphaChannel);
    if(color.spec()!=QColor::Invalid) setButtonColor(color);
}

void ColorPanel::setButtonColor(QColor color)
{
    btnMainColor->setStyleSheet(styleSheetFmt.arg(color.name()));
    emit getColor(color);
}

void ColorPanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    p.setPen(QPen(qCore->getWidgetBgColor(),1));
    p.drawRect(rect());
    p.fillRect(rect(),Qt::white);
    p.setPen(Qt::NoPen);
    QWidget::paintEvent(event);
}


