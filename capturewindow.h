#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

#include "core/core.h"
#include "core/core_system.h"
#include "helper/activewindow.h"
#include "widget/toolwidget.h"
#include "widget/inputtextedit.h"
#include "widget/pixelpanel.h"
#include "tool/texttool.h"
#include "tool/shapetool.h"
#include "tool/pentool.h"
#include "tool/erasetool.h"
#include "tool/arrowtool.h"
class CaptureWindow : public QWidget
{
    Q_OBJECT
public:
    friend class ToolWidget;
    friend class ShapeTool;
    friend class PenTool;
    friend class PixelPanel;
    friend class InputTextEdit;
    friend class TextTool;
    friend class EraseTool;
    friend class ArrowTool;

    enum {
        LEFT=1,
        RIGHT,
        TOP,
        BOTTOM,
        LEFTTOP,
        RIGHTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        REGION_IN
    };

    explicit CaptureWindow(QWidget *parent = nullptr);
    ~CaptureWindow();

    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void resizeRegion(const QPoint&,const QPoint&,QRect &);

    void drawDots(QPainter &);
    void drawTopPosTip(QPainter&);

    int checkInRectBorder(const QPoint &);
    bool checkInRectRegion(const QPoint &);
    bool checkEraseInRectRegion(const QPoint &, int);
    bool checkValidPaintRegion();
    bool checkValidPoint();

    void movePixelPanel(const QPoint&);
    void moveToolWidget();
    void moveGrabWindow(const QPoint &);
    void moveWidgetTool(QWidget *);

    void resetPaintInfo();
    QRect grabSubRegion();

    void init();
    void initShortcuts();

protected slots:
    void cancel();
    void copyColorValue();
    void changeRgb2Hex();

    void upMove();
    void downMove();
    void leftMove();
    void rightMove();

    void upMovePixelPanel();
    void downMovePixelPanel();
    void leftMovePixelPanel();
    void rightMovePixelPanel();

private:
    QRect region;
    QPoint point;
    QPoint offset;

    int d,eraseSize;
    bool pressed;
    bool completed;
    bool painting;
    bool fill;
    bool save;
    bool resizing;
    bool egeneral;

    bool cutTopWindow=false;

    ToolWidget *toolWidget;
    ShapeTool *shapeTool;
    InputTextEdit *inputTextEdit;
    PixelPanel *pixelPanel;
    PenTool *penTool;
    TextTool *textTool;
    EraseTool *eraseTool;
    ArrowTool *arrowTool;

    PaintEventNotify paintEventNotify;
    ShapeType shapeType;

    QRect paintRegion;
    QRect eraseRegion;
    QPoint paintPoint,paintPoint2,paintPoint3,erasePoint;
    QPoint cursorPos;
    QSize screenSize;

    vector<shape_object>sos;
    vector<QPoint>freeLine;

    QPixmap subRegionPixmap;

    QVector<Rect> rects;
};

#endif
