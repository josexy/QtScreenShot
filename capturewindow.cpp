
#include "drawer/rectangleblurdrawer.h"
#include "drawer/imageblurdrawer.h"
#include "drawer/arrowdrawer.h"
#include "drawer/shapedrawer.h"
#include "drawer/erasedrawer.h"
#include "drawer/textdrawer.h"
#include "drawer/pendrawer.h"

#include "widget/customtextedit.h"
#include "widget/pixelwidget.h"
#include "capturewindow.h"
#include <QApplication>
#include <QPainterPath>
#include <QMouseEvent>
#include <QShortcut>
#include <QPainter>
#include <QDebug>


CaptureWindow::CaptureWindow(QWidget *parent) :
    QWidget(parent),
    d(0),
    pressed(false),
    completed(false),
    painting(false),
    fill(false),
    save(false),
    resizing(true),
    egeneral(true)
{
    initShortcuts();
    init();

    ActiveWindow aw;
    rects=aw.enum_window();
}


CaptureWindow::~CaptureWindow() {}

bool CaptureWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    Q_UNUSED(event);
    return QObject::eventFilter(obj, event);
}

void CaptureWindow::init()
{
    installEventFilter(this);
    setMouseTracking(true);

    QScreen *screen = qCore->getPrimaryScreen();
    screenSize=screen->size();
    __LOG__(QString("Screen Size:(%1,%2)").arg(screenSize.width()).arg(screenSize.height()),1);
    setFixedSize(screen->availableGeometry().size());

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);
    setWindowState(Qt::WindowActive|Qt::WindowFullScreen);

    QPalette palatte=palette();
    qCore->setBgPixmap(qCore->getPrimaryScreen()->grabWindow(0));

    palatte.setBrush(QPalette::Window,qCore->getbgPixmap());
    setPalette(palatte);

    inputTextEdit=new InputTextEdit(this);
    inputTextEdit->hide();

    shapeTool=new ShapeTool(this);
    shapeTool->hide();

    toolWidget=new ToolWidget(this);
    toolWidget->hide();

    pixelPanel=new PixelPanel(this);
    pixelPanel->hide();

    penTool=new PenTool(this);
    penTool->hide();

    textTool=new TextTool(this);
    textTool->hide();

    eraseTool=new EraseTool(this);
    eraseTool->hide();

    arrowTool=new ArrowTool(this);
    arrowTool->hide();

    eraseSize=qCore->getEraseSize();
    eraseRegion.setWidth(eraseSize);
    eraseRegion.setHeight(eraseSize);

    resetPaintInfo();

    paintEventNotify=PaintEventNotify::Unknown;
    shapeType=ShapeType::Unknown;
    setCursor(Qt::CrossCursor);
    
    setFont(QFont(qCore->getFontFamily()));
}

void CaptureWindow::initShortcuts()
{
    new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(cancel()));
    new QShortcut(QKeySequence(Qt::Key_C),this,SLOT(copyColorValue()));
    new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_A),this,SLOT(changeRgb2Hex()));

    new QShortcut(QKeySequence(Qt::Key_Up),this,SLOT(upMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Down),this,SLOT(downMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Left),this,SLOT(leftMovePixelPanel()));
    new QShortcut(QKeySequence(Qt::Key_Right),this,SLOT(rightMovePixelPanel()));

    new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Up),this,SLOT(upMove()));
    new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Down),this,SLOT(downMove()));
    new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Left),this,SLOT(leftMove()));
    new QShortcut(QKeySequence(Qt::SHIFT+Qt::Key_Right),this,SLOT(rightMove()));
}

void CaptureWindow::copyColorValue()
{
    __LOG__("Copy color value",1);
    pixelPanel->__copy=true;
    pixelPanel->update();
}

void CaptureWindow::cancel()
{
    __LOG__("Close capture window",1);
    kCapture_window_load=false;
    qCore->resetColor();
    close();
}

void CaptureWindow::upMovePixelPanel()
{
    if(!region.contains(QCursor::pos()))return;
    if(painting) return;
    cursorPos.setY(cursorPos.y()-1);
    movePixelPanel(cursorPos);
}
void CaptureWindow::downMovePixelPanel()
{
    if(!region.contains(QCursor::pos()))return;
    if(painting) return;
    cursorPos.setY(cursorPos.y()+1);
    movePixelPanel(cursorPos);
}
void CaptureWindow::leftMovePixelPanel()
{
    if(!region.contains(QCursor::pos()))return;
    if(painting) return;
    cursorPos.setX(cursorPos.x()-1);
    movePixelPanel(cursorPos);
}
void CaptureWindow::rightMovePixelPanel()
{
    if(!region.contains(QCursor::pos()))return;
    if(painting) return;
    cursorPos.setX(cursorPos.x()+1);
    movePixelPanel(cursorPos);
}

void CaptureWindow::upMove()
{
    region.setTop(region.top()-1);
    moveToolWidget();
    update();
}
void CaptureWindow::downMove()
{
    region.setBottom(region.bottom()+1);
    moveToolWidget();
    update();
}
void CaptureWindow::leftMove()
{
    region.setLeft(region.left()-1);
    moveToolWidget();
    update();
}
void CaptureWindow::rightMove()
{
    region.setRight(region.right()+1);
    moveToolWidget();
    update();
}

void CaptureWindow::changeRgb2Hex()
{
    __LOG__("Convert RGB to HEX",1);
    pixelPanel->__convertRGBHEX=!pixelPanel->__convertRGBHEX;
    pixelPanel->update();
}

void CaptureWindow::drawDots(QPainter &p)
{
    if(region.width()<=0&&region.height()<=0) return;
    int x=region.x();
    int y=region.y();
    int w=region.width();
    int h=region.height();
    int size=qCore->getDotSize();
    auto c=qCore->getDotColor();

    p.fillRect(x-size/2,y-size/2,size,size,c);
    p.fillRect(x-size/2,y+h/2-size/2,size,size,c);
    p.fillRect(x-size/2,y+h-size/2,size,size,c);
    p.fillRect(x+w/2-size/2,y-size/2,size,size,c);
    p.fillRect(x+w/2-size/2,y+h-size/2,size,size,c);
    p.fillRect(x+w-size/2,y-size/2,size,size,c);
    p.fillRect(x+w-size/2,y+h/2-size/2,size,size,c);
    p.fillRect(x+w-size/2,y+h-size/2,size,size,c);
}

int CaptureWindow::checkInRectBorder(const QPoint&pos)
{
    int borderWeight=qCore->getBorderWeight();
    int x=region.x(),y=region.y();
    int w=region.width(),h=region.height();
    int nx=pos.x(),ny=pos.y();
    if(nx<=x&&abs(nx-x)<=borderWeight&&(ny<=y+h&&ny>=y))return LEFT;
    if(nx>=x+w&&abs(nx-(x+w))<=borderWeight&&(ny<=y+h&&ny>=y))return RIGHT;
    if(ny<=y&&abs(ny-y)<=borderWeight&&(nx<=x+w&&nx>=x))return TOP;
    if(ny>=y+h&&abs(ny-(y+h))<=borderWeight&&(nx<=x+w&&nx>=x))return BOTTOM;
    if(nx<=x&&abs(nx-x)<=borderWeight&&ny<=y&&abs(ny-y)<=borderWeight)return LEFTTOP;
    if(nx<=x&&abs(nx-x)<=borderWeight&&ny>y+h&&abs(ny-(y+h))<=borderWeight)return LEFTBOTTOM;
    if(nx>=x+w&&abs(nx-(x+w))<=borderWeight&&ny<=y&&abs(ny-y)<=borderWeight)return RIGHTTOP;
    if(nx>=x+w&&abs(nx-(x+w))<=borderWeight&&ny>=y+h&&abs(ny-(y+h))<=borderWeight)return RIGHTBOTTOM;
    if(checkInRectRegion(pos)) return REGION_IN;
    return 0;
}
bool CaptureWindow::checkInRectRegion(const QPoint&pos)
{
    return checkEraseInRectRegion(pos,0);
}
bool CaptureWindow::checkEraseInRectRegion(const QPoint&pos,int eraseSize){
    // pos is the center of erase
    int x=pos.x(),y=pos.y();
    int offset=qCore->getBorderWeight();
    if(x-eraseSize/2+offset>=region.left()&&
            y-eraseSize/2+offset>=region.top()&&
            x+eraseSize/2-offset<=region.right()&&
            y+eraseSize/2-offset<=region.bottom()) return true;
    return false;
}

void CaptureWindow::resetPaintInfo()
{
    freeLine.clear();
    paintRegion=QRect(-1,-1,0,0);
    paintPoint=QPoint(-1,-1);
    paintPoint2=QPoint(-1,-1);
    paintPoint3=QPoint(-1,-1);
    eraseRegion=QRect(-1,-1,0,0);
    erasePoint=QPoint(-1,-1);
}

void CaptureWindow::drawTopPosTip(QPainter& p)
{
    p.setPen(QPen(Qt::white,2));
    QString txt=QString("%1 x %2 px").arg(region.width()).arg(region.height());
    int fh=p.fontMetrics().height();
    int fw=p.fontMetrics().horizontalAdvance(txt);

    int padding=2;
    int x=region.left();
    int y=region.top()-(fh+padding*4+5);

    // up
    if(y<=0)x=region.right()+10,y=region.top();
    // leftbottom
    if(region.bottom()+10+toolWidget->height()>=screenSize.height()) x=region.right()+10,y=region.top();
    // righttop/rightbottom
    if(x+(fw+padding*4)>=screenSize.width())x=region.left()-(fw+padding*4)-10;

    p.drawRoundedRect(x,y,(fw+padding*4),fh+padding*2,3,3);
    p.drawText(x+padding*2,y+fh/2+padding*2+5,txt);
}
bool CaptureWindow::checkValidPaintRegion()
{
    return paintRegion.left()!=-1 && paintRegion.top()!=-1 && paintRegion.width() && paintRegion.height();
}
bool CaptureWindow::checkValidPoint()
{
    return paintPoint.x()!=-1&&paintPoint.y()!=-1;
}
void CaptureWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
    // background mask
    p.fillRect(rect(),qCore->maskColor());

    if(pressed||completed||cutTopWindow){
        p.eraseRect(region);

        // draw shape_object
        if(paintEventNotify==PaintEventNotify::Shape||paintEventNotify==PaintEventNotify::Pen
                ||paintEventNotify==PaintEventNotify::Erase||paintEventNotify==PaintEventNotify::Text
                ||paintEventNotify==PaintEventNotify::Arrow||paintEventNotify==PaintEventNotify::Blur
                ||paintEventNotify==PaintEventNotify::Brush){

            QPixmap tmpBgPixmap=qCore->getbgPixmap();
            QPainter pt(&tmpBgPixmap);
            int size=sos.size(),i=0;
            if(size){
                do{
                    // set pen for per shape_object
                    p.setPen(QPen(sos[i].color,sos[i].pensize));
                    pt.setPen(p.pen());

                    switch (sos[i].st) {
                    case ShapeType::Rectangle:
                    case ShapeType::Circle:
                    case ShapeType::Triangle:
                    case ShapeType::RTriangle:
                    case ShapeType::Brush: {
                        ShapeDrawer(sos[i],p,this);
                        ShapeDrawer(sos[i],pt,this);
                        break;
                    }
                    case ShapeType::Point:
                    case ShapeType::Line:
                    case ShapeType::DashLine:
                    case ShapeType::Curve:{
                        PenDrawer(sos[i],p,this);
                        PenDrawer(sos[i],pt,this);
                        break;
                    }
                    case ShapeType::TriArrow:
                    case ShapeType::LineArrow:{
                        ArrowDrawer(sos[i].ps.front(),sos[i].ps.back(),5*sos[i].pensize,sos[i].color,p,sos[i].st);
                        ArrowDrawer(sos[i].ps.front(),sos[i].ps.back(),5*sos[i].pensize,sos[i].color,pt,sos[i].st);
                        break;
                    }
                    case ShapeType::Text: {
                        TextDrawer(sos[i],p,this);
                        TextDrawer(sos[i],pt,this);
                        break;
                    }
                    case ShapeType::Blur:{
                        RectangleBlurDrawer(tmpBgPixmap,sos[i].rs.first(),p,qCore->blurRadius(),this);
                        RectangleBlurDrawer(tmpBgPixmap,sos[i].rs.first(),pt,qCore->blurRadius(),this);
                        break;
                    }
                    case ShapeType::Erase:{
                        EraseDrawer(sos[i],p,pt,this);
                        break;
                    }
                    default:break;
                    }
                    i++;
                }while(i<size);
            }
            // preview
            {
                QColor bgcolor=qCore->getBgColor();
                p.setPen(QPen(qCore->getPenColor(),qCore->getPenSize()));
                switch (shapeType) {
                case ShapeType::Rectangle:
                case ShapeType::Circle:
                case ShapeType::Brush: {
                    if(checkValidPaintRegion()) {
                        if(fill) {
                            p.setBrush(bgcolor);
                        }
                        if(qCore->getPenSize()){
                            if(shapeType==ShapeType::Rectangle){
                                p.drawRect(paintRegion);
                            }
                            else if(shapeType==ShapeType::Circle){
                                p.drawEllipse(paintRegion);
                            }else if(shapeType==ShapeType::Brush){
                                QColor c=qCore->getPenColor();
                                p.fillRect(paintRegion,QColor(c.red(),c.green(),c.blue(),qCore->brushOpacity()));
                            }
                        }
                        p.setBrush(Qt::NoBrush);
                    }
                    break;
                }
                case ShapeType::Blur:{
                    if(checkValidPaintRegion()){
                        RectangleBlurDrawer(tmpBgPixmap,paintRegion,p,qCore->blurRadius(),this);
                    }
                    break;
                }
                case ShapeType::Line:
                case ShapeType::DashLine:{
                    if(checkValidPoint() && paintPoint2.x()!=-1 &&paintPoint2.y()!=-1){
                        if(shapeType==ShapeType::DashLine){
                            p.setPen(QPen(qCore->getPenColor(),qCore->getPenSize(),Qt::DashLine));
                        }
                        p.drawLine(paintPoint,paintPoint2);
                    }
                    break;
                }
                case ShapeType::TriArrow:
                case ShapeType::LineArrow:{
                    ArrowDrawer(paintPoint,paintPoint2,5*qCore->getPenSize(),qCore->getPenColor(),p,shapeType);
                    break;
                }
                case ShapeType::Curve:{
                    if(freeLine.size()<=0) break;
                    QPainterPath pp;
                    for(std::size_t j=0; j+1<freeLine.size(); j++){
                        pp.moveTo(freeLine[j]);
                        pp.lineTo(freeLine[j+1]);
                    }
                    p.drawPath(pp);
                    break;
                }
                case ShapeType::Triangle:
                case ShapeType::RTriangle:{
                    if(checkValidPoint() && paintPoint2.x()!= -1 && paintPoint2.y()!=-1){
                        QVector<QPoint> x={paintPoint,paintPoint2,paintPoint3,paintPoint};
                        QPainterPath pp;
                        pp.addPolygon(QPolygonF(x));
                        if(fill) {
                            p.fillPath(pp,bgcolor);
                        }
                        p.drawPath(pp);
                        p.setBrush(Qt::NoBrush);
                    }
                    break;
                }
                case ShapeType::Point: {
                    if(checkValidPoint()) {
                        p.drawPoint(paintPoint);
                    }
                    break;
                }
                case ShapeType::Erase:{
                    if(egeneral){
                        for(auto && x:freeLine){
                            QRect r(x.x()-eraseSize/2,x.y()-eraseSize/2,eraseSize,eraseSize);
                            p.eraseRect(r);
                        }
                    }else{
                        if(erasePoint.x()!=-1&& erasePoint.y()!=-1&& eraseRegion.x()!=-1&&eraseRegion.y()!=-1)
                            p.eraseRect(eraseRegion);
                    }
                    break;
                }
                default:break;
                }
            }
        }
        // draw region border
        p.setPen(QPen(qCore->getBorderColor(),qCore->getBorderWeight()));
        p.drawRect(region);

        if(!save) drawDots(p);

        if(region.width()&&region.height()) drawTopPosTip(p);
    }
    QWidget::paintEvent(event);
}

void CaptureWindow::resizeRegion(const QPoint& fromPos,const QPoint&toPos,QRect &r)
{
    int x1=fromPos.x();
    int y1=fromPos.y();
    int x2=toPos.x();
    int y2=toPos.y();
    if(x2>x1&&y2>y1){
        r.setRect(x1,y1,x2-x1,y2-y1);
    }else if(x2>x1&&y2<y1){
        r.setRect(x1,y2,x2-x1,y1-y2);
    }else if(x2<x1&&y2<y1){
        r.setRect(x2,y2,x1-x2,y1-y2);
    }else if(x2<x1&&y2>y1){
        r.setRect(x2,y1,x1-x2,y2-y1);
    }
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(completed){

        if(!pressed){
            switch (checkInRectBorder(event->pos())) {
            case LEFT:case RIGHT:setCursor(Qt::SizeHorCursor);break;
            case TOP:case BOTTOM:setCursor(Qt::SizeVerCursor);break;
            case LEFTTOP:case RIGHTBOTTOM:setCursor(Qt::SizeFDiagCursor);break;
            case RIGHTTOP:case LEFTBOTTOM:setCursor(Qt::SizeBDiagCursor);break;
            case REGION_IN: {
                if(!painting && resizing) {
                    // resize the region window
                    cursorPos=event->pos();
                    movePixelPanel(event->pos());
                    setCursor(Qt::OpenHandCursor);
                }else{
                    switch(shapeType){
                    case ShapeType::Text:setCursor(Qt::IBeamCursor);break;
                    case ShapeType::Line: case ShapeType::Point:case ShapeType::LineArrow:
                    case ShapeType::Curve: case ShapeType::Rectangle:case ShapeType::Brush:
                    case ShapeType::TriArrow: case ShapeType::DashLine:case ShapeType::RTriangle:
                    case ShapeType::Blur: case ShapeType::Circle:case ShapeType::Triangle: setCursor(Qt::CrossCursor);break;
                    case ShapeType::Erase: {
                        if(egeneral) setCursor(Qt::ForbiddenCursor);
                        else setCursor(Qt::CrossCursor);
                        break;
                    }
                    default:{setCursor(Qt::ArrowCursor);break;}
                    }
                }
                break;
            }
            default: {
                setCursor(Qt::ArrowCursor);
                pixelPanel->hide();
            }
            }
        }
        // left button pressed
        if((event->buttons()&Qt::LeftButton)==Qt::LeftButton){
            moveToolWidget();

            eraseTool->hide();
            pixelPanel->hide();
            shapeTool->hide();
            penTool->hide();
            textTool->hide();
            arrowTool->hide();

            if(d==LEFT) { if(event->pos().x()<=region.right())region.setX(event->pos().x());}
            else if(d==TOP){ if(event->pos().y()<=region.bottom())  region.setY(event->pos().y());}
            else if(d==RIGHT){ if(event->pos().x()>=region.left())region.setRight(event->pos().x());}
            else if(d==BOTTOM){ if(event->pos().y()>=region.top())region.setBottom(event->pos().y());}
            else if(d==LEFTTOP){
                if(event->pos().x()<=region.bottomRight().x()&&event->pos().y()<=region.bottomRight().y())
                    region.setTopLeft(event->pos());
            }
            else if(d==RIGHTTOP){
                if(event->pos().x()>=region.bottomLeft().x()&&event->pos().y()<=region.bottomLeft().y())
                    region.setTopRight(event->pos());
            }
            else if(d==LEFTBOTTOM){
                if(event->pos().x()<=region.topRight().x()&&event->pos().y()>=region.topRight().y())
                    region.setBottomLeft(event->pos());
            }
            else if(d==RIGHTBOTTOM){
                if(event->pos().x()>=region.topLeft().x()&&event->pos().y()>=region.topLeft().y())
                    region.setBottomRight(event->pos());
            }
            else if(d==REGION_IN) {

                // paint region
                if(paintEventNotify==PaintEventNotify::Shape||paintEventNotify==PaintEventNotify::Pen
                        ||paintEventNotify==PaintEventNotify::Erase||paintEventNotify==PaintEventNotify::Text
                        ||paintEventNotify==PaintEventNotify::Arrow||paintEventNotify==PaintEventNotify::Blur
                        ||paintEventNotify==PaintEventNotify::Brush){

                    if(shapeType==ShapeType::Brush||shapeType==ShapeType::Rectangle||shapeType==ShapeType::Circle||shapeType==ShapeType::Blur){
                        paintRegion.setWidth(event->x()-paintPoint.x());
                        paintRegion.setHeight(event->y()-paintPoint.y());
                    }else if(shapeType==ShapeType::DashLine || shapeType==ShapeType::Line
                             || shapeType==ShapeType::LineArrow || shapeType==ShapeType::TriArrow){
                        paintPoint2=event->pos();
                    }else if(shapeType==ShapeType::Curve){
                        freeLine.push_back(event->pos());
                    }else if(shapeType==ShapeType::RTriangle||shapeType==ShapeType::Triangle){
                        // paintPoint2 is left
                        // paintPoint3 is right
                        paintPoint2.setY(event->y());
                        paintPoint3.setY(event->y());
                        if(shapeType==ShapeType::RTriangle)
                            paintPoint2.setX(paintPoint.x());
                        else
                            paintPoint2.setX(paintPoint.x()-(event->x()-paintPoint.x()));
                        paintPoint3.setX(event->pos().x());
                    }else if(shapeType==ShapeType::Erase){
                        if(checkEraseInRectRegion(event->pos(),eraseSize)){
                            if(egeneral){
                                freeLine.push_back(event->pos());
                            }else{
                                // rectangle region erase
                                eraseRegion.setWidth(event->x()-erasePoint.x());
                                eraseRegion.setHeight(event->y()-erasePoint.y());
                            }
                        }
                    }
                }else {
                    moveGrabWindow(event->pos());
                }
            }
            update();
        }
        return;
    }
    if(!pressed) {
        for(auto x=rects.begin();x!=rects.end();x++){
            auto r=QRect(*x);
            if(r.contains(event->pos())){
                region=r;
                break;
            }
        }
        cutTopWindow=true;
        update();
        return;
    }

    setCursor(Qt::CrossCursor);

    movePixelPanel(event->pos());

    resizeRegion(point,event->pos(),region);
    update();
}

void CaptureWindow::moveGrabWindow(const QPoint& pos)
{
    int w=region.width(),h=region.height();
    QRect r;
    r.setX(pos.x()-offset.x());
    r.setY(pos.y()-offset.y());
    r.setWidth(w);
    r.setHeight(h);
    if(r.left()<=0 && r.top() <=0) region=QRect(0,0,r.width(),r.height());
    else if(r.left()<=0 && r.bottom() >=screenSize.height()) region=QRect(0,screenSize.height()-r.height(),r.width(),r.height());
    else if(r.right()>=screenSize.width() && r.bottom() >= screenSize.height()) region=QRect(screenSize.width()-r.width(),screenSize.height()-r.height(),r.width(),r.height());
    else if(r.right()>=screenSize.width() && r.top() <=0)region=QRect(screenSize.width()-r.width(),0,r.width(),r.height());
    else if(r.left()<=0)region=QRect(0,r.y(),r.width(),r.height());
    else if(r.top()<=0)region=QRect(r.x(),0,r.width(),r.height());
    else if(r.bottom()>=screenSize.height())region=QRect(r.x(),screenSize.height()-r.height(),r.width(),r.height());
    else if(r.right()>=screenSize.width())region=QRect(screenSize.width()-r.width(),r.y(),r.width(),r.height());
    else region=r;
}
void CaptureWindow::moveWidgetTool(QWidget *widget)
{
    // shapeTool is top of toolWidget
    if(mapToParent(toolWidget->pos()).y()<region.top()){
        //  shapeTool is left or right of toolWidget
        if(region.left()+10+toolWidget->width()+shapeTool->width()>=screenSize.width()){
            widget->move(QPoint(mapToParent(toolWidget->pos()).x()-widget->width()-10,region.top()-toolWidget->height()-10));
        }else {
            widget->move(QPoint(region.left(),region.top()-toolWidget->height()-widget->height()-10));
        }
    }
    // shapeTool is left or right of toolWidget
    else if(region.bottom()+10+widget->height()+toolWidget->height()>=screenSize.height()){
        if(region.left()+10+toolWidget->width()+widget->width()>=screenSize.width()){
            widget->move(QPoint(region.left()-widget->width()-10,region.bottom()+10));
        }else {
            widget->move(QPoint(region.left()+10+toolWidget->width(),region.bottom()+10));
        }
        // shapeTool is bottom of toolWidget
    }else if(region.bottom()+10+toolWidget->height()<=screenSize.height()){
        widget->move(QPoint(region.left(),region.bottom()+10+toolWidget->height()));
    }
}

void CaptureWindow::moveToolWidget()
{
    // rigth bottom
    if(region.bottom()+10+toolWidget->height()>=screenSize.height() && region.left()+toolWidget->width()>=screenSize.width()){
        toolWidget->move(QPoint(toolWidget->x(),region.top()-toolWidget->height()-10));
        // bottom
    }else if(region.bottom()+10+toolWidget->height()>=screenSize.height()){
        toolWidget->move(QPoint(region.left(),region.top()-toolWidget->height()-10));
        // right
    }else if(region.left()+toolWidget->width()>=screenSize.width()){
        toolWidget->move(QPoint(toolWidget->x(),region.bottom()+10));
        // normal
    }else{
        toolWidget->move(QPoint(region.left(),region.bottom()+10));
    }
}

void CaptureWindow::movePixelPanel(const QPoint& pos)
{
    int pw=pixelPanel->width();
    int ph=pixelPanel->height();
    int offset=20;
    int x=pos.x();
    int y=pos.y();
    pixelPanel->move(x+offset,y+offset);

    if(ph+y+offset>=screenSize.height()&&pw+x+offset>=screenSize.width()){
        pixelPanel->move(x-pw-offset,y-offset-ph);
    }else if(ph+y+offset>=screenSize.height()){
        pixelPanel->move(x+offset,y-offset-ph);
    }else if(pw+x+offset>=screenSize.width()){
        pixelPanel->move(x-pw-offset,y+offset);
    }else{
        pixelPanel->move(x+offset,y+offset);
    }
    pixelPanel->setPos(pos);
    pixelPanel->show();
    pixelPanel->update();
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        cutTopWindow=false;
        if(!completed){
            setCursor(Qt::PointingHandCursor);
            point=event->pos();
            pixelPanel->move(event->pos());
        }
        d=checkInRectBorder(event->pos());

        switch (d) {
        case LEFT:case RIGHT:setCursor(Qt::SizeHorCursor);painting=false;break;
        case TOP:case BOTTOM:setCursor(Qt::SizeVerCursor);painting=false;break;
        case LEFTTOP:case RIGHTBOTTOM:setCursor(Qt::SizeFDiagCursor);painting=false;break;
        case RIGHTTOP:case LEFTBOTTOM:setCursor(Qt::SizeBDiagCursor);painting=false;break;
        case REGION_IN:{
            freeLine.clear();

            if(paintEventNotify==PaintEventNotify::Shape||paintEventNotify==PaintEventNotify::Pen
                    ||paintEventNotify==PaintEventNotify::Erase||paintEventNotify==PaintEventNotify::Text
                    ||paintEventNotify==PaintEventNotify::Arrow||paintEventNotify==PaintEventNotify::Blur
                    ||paintEventNotify==PaintEventNotify::Brush){

                painting=true;
                bool erase_ok=true;
                switch(shapeType){
                case ShapeType::Rectangle:
                case ShapeType::Circle:
                case ShapeType::Triangle:
                case ShapeType::RTriangle:
                case ShapeType::Line:
                case ShapeType::DashLine:
                case ShapeType::Curve:
                case ShapeType::Point:
                case ShapeType::TriArrow:
                case ShapeType::LineArrow:
                case ShapeType::Blur:
                case ShapeType::Brush:setCursor(Qt::CrossCursor);break;

                case ShapeType::Text:{
                    setCursor(Qt::IBeamCursor);
                    QSize size=inputTextEdit->customTextEdit->size();
                    inputTextEdit->customTextEdit->setFocus();
                    inputTextEdit->move(QPoint(event->pos().x()-10,event->y()-size.height()/2));
                    inputTextEdit->show();
                    // clear textedit
                    inputTextEdit->customTextEdit->clear();
                    break;
                }
                case ShapeType::Erase:{
                    // check erase is in region
                    if(checkEraseInRectRegion(event->pos(),eraseSize)){
                        if(egeneral){
                            setCursor(Qt::ForbiddenCursor);
                            eraseRegion.setRect(event->pos().x()/eraseSize/2,event->pos().y()+eraseSize/2,eraseSize,eraseSize);
                        }else{
                            setCursor(Qt::CrossCursor);
                            erasePoint=event->pos();
                            eraseRegion.setX(erasePoint.x());
                            eraseRegion.setY(erasePoint.y());
                        }
                    }else erase_ok=false;
                    break;
                }
                default:setCursor(Qt::CrossCursor);break;
                }
                if(erase_ok){
                    // modify paint region
                    paintPoint=event->pos();
                    paintRegion=QRect(paintPoint.x(),paintPoint.y(),0,0);
                    freeLine.push_back(paintPoint);
                }
            }
            else{
                setCursor(Qt::ClosedHandCursor);
                offset.setX(event->pos().x()-region.left());
                offset.setY(event->pos().y()-region.top());
            }
            break;
        }
        default: {
            setCursor(Qt::ArrowCursor);
        }
        }
        pressed=true;
    }
}

QRect CaptureWindow::grabSubRegion()
{
    save=true;
    update();
    int offset=qCore->getBorderWeight();
    QRect r;
    r.setLeft(region.left()+offset);
    r.setTop(region.top()+offset);
    r.setRight(region.right()-offset);
    r.setBottom(region.bottom()-offset);

    qCore->setPixMap(grab(r));
    save=false;
    return r;
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){

        if(region.width() ==0 ||region.height()==0)
            return;
        pixelPanel->hide();

        // grab window pixmap to pixmap object
        grabSubRegion();

        // resize region window
        if(!painting && resizing) setCursor(Qt::OpenHandCursor);

        pressed=false;
        completed=true;

        if(paintEventNotify==PaintEventNotify::Shape||paintEventNotify==PaintEventNotify::Pen
                ||paintEventNotify==PaintEventNotify::Erase||paintEventNotify==PaintEventNotify::Text
                ||paintEventNotify==PaintEventNotify::Arrow||paintEventNotify==PaintEventNotify::Blur
                ||paintEventNotify==PaintEventNotify::Brush){
            if(!painting) return;

            resizeRegion(paintPoint,event->pos(),paintRegion);
            resizeRegion(erasePoint,event->pos(),eraseRegion);
            shape_object so;
            so.color=qCore->getPenColor();
            so.pensize=qCore->getPenSize();
            so.bgcolor=qCore->getBgColor();
            so.fill=fill;
            so.isblur=false;

            if(!checkValidPoint())
                goto invalid;

            if(shapeType!=ShapeType::Point && shapeType!=ShapeType::Line
               && shapeType!=ShapeType::TriArrow && shapeType!=ShapeType::DashLine
               && shapeType!=ShapeType::Point && shapeType!=ShapeType::LineArrow && !checkValidPaintRegion()){
                goto invalid;
            }

            if(shapeType==ShapeType::Brush||shapeType==ShapeType::Blur||shapeType==ShapeType::Rectangle||shapeType==ShapeType::Circle){
                // check is a rectangle
                if(paintRegion.width() == 0 || paintRegion.height() == 0) return;
                so.st=shapeType;
                if(shapeType==ShapeType::Blur){
                    if(paintRegion.x()<=region.x()) paintRegion.setX(region.x());
                    if(paintRegion.y()<=region.y()) paintRegion.setY(region.y());
                    if(paintRegion.right()>=region.right()) paintRegion.setRight(region.right());
                    if(paintRegion.bottom()>=region.bottom()) paintRegion.setBottom(region.bottom());
                }
                so.rs.push_back(paintRegion);
                sos.push_back(so);
            }else if(shapeType==ShapeType::DashLine || shapeType==ShapeType::Line
                     || shapeType==ShapeType::TriArrow || shapeType==ShapeType::LineArrow){
                if(checkValidPoint() && paintPoint2.x()!=-1 && paintPoint2.y()!=-1){
                    so.st=shapeType;
                    so.ps.push_back(paintPoint);
                    so.ps.push_back(paintPoint2);
                    sos.push_back(so);
                }
            }else if(shapeType==ShapeType::Point){
                so.st=shapeType;
                so.ps.push_back(paintPoint);
                sos.push_back(so);
            }else if(shapeType==ShapeType::Curve){
                so.st=shapeType;
                for(auto pos:freeLine) so.ps.push_back(pos);
                sos.push_back(so);

            }else if(shapeType==ShapeType::RTriangle||shapeType==ShapeType::Triangle){
                if(checkValidPoint()&&paintPoint2.x()!=-1 && paintPoint2.y()!=-1){
                    so.st=shapeType;
                    so.ps.push_back(paintPoint);
                    so.ps.push_back(paintPoint2);
                    so.ps.push_back(paintPoint3);
                    so.ps.push_back(paintPoint);
                    sos.push_back(so);
                }
            }else if(shapeType==ShapeType::Erase){
                // not empty shape_objects
                if(sos.size()>0){
                    so.st=shapeType;
                    so.egeneral=egeneral;
                    if(egeneral){
                        so.erasesize=eraseSize;
                        for(auto &&x : freeLine) so.ps.push_back(x);
                        sos.push_back(so);
                    }else{
                        if(eraseRegion.x()<=region.x()) eraseRegion.setX(region.x());
                        if(eraseRegion.y()<=region.y()) eraseRegion.setY(region.y());
                        if(eraseRegion.right()>=region.right()) eraseRegion.setRight(region.right());
                        if(eraseRegion.bottom()>=region.bottom()) eraseRegion.setBottom(region.bottom());

                        so.rs.push_back(eraseRegion);
                        if(eraseRegion.x()!=-1&&eraseRegion.y()!=-1) sos.push_back(so);
                    }
                }
            }
        }

        moveToolWidget();
        toolWidget->show();

invalid:
        resetPaintInfo();
    }
}
