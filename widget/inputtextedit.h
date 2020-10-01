#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QWidget>

class CaptureWindow;
class CustomTextEdit;
class InputTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit InputTextEdit(QWidget *parent = nullptr);
    ~InputTextEdit();
    void paintEvent(QPaintEvent*);
    void moveEvent(QMoveEvent*);

    QPair<QStringList,QVector<QPoint>> getMultilineText(const QString &);
    CustomTextEdit *customTextEdit;
public slots:
    void textUpdated(const QString &);
private:
    CaptureWindow* w;

    QString textValue;
    QPoint prevPos;
};

#endif
