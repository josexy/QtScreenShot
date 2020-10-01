#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget* parent): QTextEdit(parent)
{
    setStyleSheet(QStringLiteral("CustomTextEdit { border: none;background: transparent; }"));
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContextMenuPolicy(Qt::NoContextMenu);

    connect(this,&CustomTextEdit::textChanged,this,&CustomTextEdit::adjustSize);
    connect(this,&CustomTextEdit::textChanged,this,&CustomTextEdit::emitTextUpdated);
}


void CustomTextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);
}

void CustomTextEdit::showEvent(QShowEvent *e)
{
    QFont font;
    QFontMetrics fm(font);
    setFixedWidth(fm.lineSpacing() * 6);
    setFixedHeight(fm.lineSpacing());
    __baseSize = size();
    __minSize = __baseSize;
    adjustSize();
    QTextEdit::showEvent(e);
}

void CustomTextEdit::resizeEvent(QResizeEvent *e)
{
    __minSize.setHeight(qMin(__baseSize.height(), height()));
    __minSize.setWidth(qMin(__baseSize.width(), width()));
    emit TextEditSizeChanged(rect());
    QTextEdit::resizeEvent(e);
}


void CustomTextEdit::adjustSize()
{
    QString&& text = toPlainText();

    QFontMetrics fm(font());
    QRect bounds = fm.boundingRect(QRect(), 0, text);
    int pixelsWide = bounds.width() + fm.lineSpacing();
    int pixelsHigh = bounds.height() + fm.lineSpacing();

    if (pixelsWide < __minSize.width()) {
        pixelsWide = __minSize.width();
    }
    if (pixelsHigh < __minSize.height()) {
        pixelsHigh = __minSize.height();
    }

    setFixedSize(pixelsWide, pixelsHigh);
}

void CustomTextEdit::emitTextUpdated()
{
    emit textUpdated(toPlainText());
}

