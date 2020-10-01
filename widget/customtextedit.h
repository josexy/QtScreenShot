#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    CustomTextEdit(QWidget* parent=nullptr);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    void adjustSize();

signals:
    void textUpdated(const QString&);
    void TextEditSizeChanged(const QRect&);

private slots:
    void emitTextUpdated();
private:
    QSize __baseSize, __minSize;
};

#endif
