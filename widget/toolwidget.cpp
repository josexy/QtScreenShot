#include "toolwidget.h"
#include "capturewindow.h"
#include "basebutton.h"

#include <QHBoxLayout>
#include <QPainter>

ToolWidget::ToolWidget(QWidget *parent) :
    BaseTool(parent),captureW((CaptureWindow*)parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(450,50);

    initUI();
}

void ToolWidget::initUI()
{
    QHBoxLayout *hboxL=new QHBoxLayout(this);
    hboxL->setMargin(0);
    hboxL->setContentsMargins(qCore->borderPadding()/2,qCore->borderPadding()/2,qCore->borderPadding()/2,qCore->borderPadding()/2);
    hboxL->setSpacing(0);

    btnShape=new BaseButton(qCore->svgImagePath()+QStringLiteral("shape.svg"),tr("shape tool"),this);
    btnPen=new BaseButton(qCore->svgImagePath()+QStringLiteral("pen.svg"),tr("pen tool"),this);
    btnText=new BaseButton(qCore->svgImagePath()+QStringLiteral("text.svg"),tr("text tool"),this);
    btnArrow=new BaseButton(qCore->svgImagePath()+QStringLiteral("arrow.svg"),tr("arrow tool"),this);
    btnBlur=new BaseButton(qCore->svgImagePath()+QStringLiteral("blur.svg"),tr("blur"),this);
    btnErase=new BaseButton(qCore->svgImagePath()+QStringLiteral("erase.svg"),tr("erase tool"),this);
    btnUndo=new BaseButton(qCore->svgImagePath()+QStringLiteral("undo.svg"),tr("undo"),this,false);
    btnRedo=new BaseButton(qCore->svgImagePath()+QStringLiteral("redo.svg"),tr("redo"),this,false);
    btnCancel=new BaseButton(qCore->svgImagePath()+QStringLiteral("cancel.svg"),tr("close"),this,false);
    btnSave=new BaseButton(qCore->svgImagePath()+QStringLiteral("save.svg"),tr("save to file"),this,false);
    btnCopy=new BaseButton(qCore->svgImagePath()+QStringLiteral("copy.svg"),tr("copy to clipboard"),this,false);

    hboxL->addWidget(btnShape);
    hboxL->addWidget(btnPen);
    hboxL->addWidget(btnText);
    hboxL->addWidget(btnArrow);
    hboxL->addWidget(btnBlur);
    hboxL->addWidget(btnErase);
    hboxL->addWidget(btnUndo);
    hboxL->addWidget(btnRedo);
    hboxL->addWidget(btnCancel);
    hboxL->addWidget(btnSave);
    hboxL->addWidget(btnCopy);
    setLayout(hboxL);

    connect(btnShape,SIGNAL(clicked(bool)),this,SLOT(on_btnShape_clicked()));
    connect(btnPen,SIGNAL(clicked(bool)),this,SLOT(on_btnPen_clicked()));
    connect(btnText,SIGNAL(clicked(bool)),this,SLOT(on_btnText_clicked()));
    connect(btnArrow,SIGNAL(clicked(bool)),this,SLOT(on_btnArrow_clicked()));
    connect(btnErase,SIGNAL(clicked(bool)),this,SLOT(on_btnErase_clicked()));
    connect(btnUndo,SIGNAL(clicked(bool)),this,SLOT(on_btnUndo_clicked()));
    connect(btnRedo,SIGNAL(clicked(bool)),this,SLOT(on_btnRedo_clicked()));
    connect(btnCancel,SIGNAL(clicked(bool)),this,SLOT(on_btnCancel_clicked()));
    connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(on_btnSave2File_clicked()));
    connect(btnCopy,SIGNAL(clicked(bool)),this,SLOT(on_btnCopyClipboard_clicked()));
    connect(btnBlur,SIGNAL(clicked(bool)),this,SLOT(on_btnBlur_clicked()));
}

ToolWidget::~ToolWidget() {}

void ToolWidget::singlePressed()
{
    btnShape->setChecked(false);
    btnPen->setChecked(false);
    btnErase->setChecked(false);
    btnText->setChecked(false);
    btnArrow->setChecked(false);
    btnBlur->setChecked(false);
    switch (captureW->paintEventNotify) {
    case PaintEventNotify::Shape: btnShape->setChecked(true); break;
    case PaintEventNotify::Pen: btnPen->setChecked(true); break;
    case PaintEventNotify::Erase: btnErase->setChecked(true); break;
    case PaintEventNotify::Text: btnText->setChecked(true); break;
    case PaintEventNotify::Arrow: btnArrow->setChecked(true); break;
    case PaintEventNotify::Blur: btnBlur->setChecked(true);break;
    default:break;
    }
}

void ToolWidget::on_btnSave2File_clicked()
{
    captureW->setCursor(Qt::ArrowCursor);

    if(captureW->paintEventNotify==PaintEventNotify::Text){
        captureW->inputTextEdit->hide();
    }
    captureW->grabSubRegion();
    qCore->PixMap2ImageFile(qCore->getPixMap());
    qCore->getSysTray()->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to a file"));
    on_btnCancel_clicked();
}

void ToolWidget::on_btnCopyClipboard_clicked()
{
    captureW->setCursor(Qt::ArrowCursor);

    if(captureW->paintEventNotify==PaintEventNotify::Text){
        captureW->inputTextEdit->hide();
    }
    captureW->grabSubRegion();
    qCore->PixMap2ClipBoard(qCore->getPixMap());
    qCore->getSysTray()->showMessage(tr("QtScreenShot notification"),tr("The image has been saved to the clipboard"));
    on_btnCancel_clicked();
}

void ToolWidget::on_btnCancel_clicked()
{
    kCapture_window_load=false;
    captureW->setCursor(Qt::ArrowCursor);
    captureW->close();
    close();
}

void ToolWidget::on_btnBlur_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Blur;
    captureW->shapeType=ShapeType::Blur;
    captureW->painting=true;
    captureW->resizing=false;
    qCore->setSingleShape(captureW->shapeType);

    captureW->eraseTool->hide();
    captureW->inputTextEdit->hide();
    captureW->penTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->shapeTool->hide();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}


void ToolWidget::on_btnShape_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Shape;
    captureW->painting=true;
    captureW->resizing=false;

    captureW->eraseTool->hide();
    captureW->inputTextEdit->hide();
    captureW->penTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->moveWidgetTool(captureW->shapeTool);
    captureW->shapeTool->show();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnPen_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Pen;
    captureW->painting=true;
    captureW->resizing=false;

    captureW->eraseTool->hide();
    captureW->inputTextEdit->hide();
    captureW->shapeTool->hide();
    captureW->textTool->hide();
    captureW->arrowTool->hide();
    captureW->moveWidgetTool(captureW->penTool);
    captureW->penTool->show();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnText_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Text;
    captureW->shapeType=ShapeType::Text;
    qCore->setSingleShape(captureW->shapeType);
    captureW->painting=true;
    captureW->resizing=false;

    captureW->eraseTool->hide();
    captureW->shapeTool->hide();
    captureW->penTool->hide();
    captureW->arrowTool->hide();
    captureW->moveWidgetTool(captureW->textTool);
    captureW->textTool->show();

    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnArrow_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Arrow;
    captureW->painting=true;
    captureW->resizing=false;

    captureW->eraseTool->hide();
    captureW->inputTextEdit->hide();
    captureW->shapeTool->hide();
    captureW->textTool->hide();
    captureW->penTool->hide();
    captureW->arrowTool->show();

    captureW->moveWidgetTool(captureW->arrowTool);
    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnErase_clicked()
{
    captureW->paintEventNotify=PaintEventNotify::Erase;
    captureW->painting=true;
    captureW->resizing=false;

    captureW->eraseTool->show();
    captureW->textTool->hide();
    captureW->shapeTool->hide();
    captureW->penTool->hide();
    captureW->inputTextEdit->hide();
    captureW->arrowTool->hide();

    captureW->moveWidgetTool(captureW->eraseTool);
    captureW->shapeTool->singlePressed();
    captureW->arrowTool->singlePressed();
    captureW->penTool->singlePressed();
    captureW->eraseTool->singlePressed();
    singlePressed();
}

void ToolWidget::on_btnUndo_clicked()
{
    captureW->inputTextEdit->hide();
    captureW->textTool->hide();
    captureW->resetPaintInfo();

    if(!captureW->sos.empty()){
        restore.push(captureW->sos.back());
        captureW->sos.pop_back();
        captureW->update();
    }
}

void ToolWidget::on_btnRedo_clicked()
{
    captureW->inputTextEdit->hide();
    captureW->textTool->hide();
    if(!restore.empty()){
        captureW->sos.push_back(restore.top());
        restore.pop();
        captureW->update();
    }
}
