#include "capturewindow.h"
#include "customtextedit.h"
#include "inputtextedit.h"

#include <QPainter>

InputTextEdit::InputTextEdit(QWidget *parent) :
    QWidget(parent),
    w((CaptureWindow*)parent)
{

    setWindowFlag(Qt::FramelessWindowHint);
    customTextEdit=new CustomTextEdit(this);

    connect(customTextEdit,&CustomTextEdit::textUpdated,this,&InputTextEdit::textUpdated);
    connect(customTextEdit,&CustomTextEdit::TextEditSizeChanged,this,[&](const QRect&r){
        this->setFixedSize(r.width(),r.height());
    });
}


QPair<QStringList, QVector<QPoint>> InputTextEdit::getMultilineText(const QString &s)
{
    QStringList sl=s.split('\n');
    auto fm=customTextEdit->fontMetrics();
    int h=fm.height();

    QPoint np(prevPos);
    QVector<QPoint> lps;

    for(int i=0; i<sl.size(); i++){
        QPoint pos(np.x(),np.y()+h*(i+1));
        lps.push_back(pos);
    }
    return qMakePair(sl,lps);
}

void InputTextEdit::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);

    if(w->shapeType==ShapeType::Text && !textValue.trimmed().isEmpty()){
        shape_object so;
        so.st=w->shapeType;
        so.color=customTextEdit->textColor();
        so.font=qCore->getFont();
        auto x=getMultilineText(textValue);
        so.ts=x.first;
        so.ps=x.second;
        w->sos.push_back(so);
        w->update();
    }
    QWidget::moveEvent(event);
}


void InputTextEdit::textUpdated(const QString& text)
{
    textValue=text;
    prevPos=mapToParent(customTextEdit->pos());
}

InputTextEdit::~InputTextEdit()
{
}

void InputTextEdit::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QPen(qCore->getBorderColor(),2));
    p.drawRect(rect());
    p.fillRect(rect(),QColor(0,0,0,100));
    QWidget::paintEvent(event);
}

