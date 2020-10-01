#ifndef COLORPANEL_H
#define COLORPANEL_H

#include <QPushButton>
#include "../core/core_system.h"

class ColorPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPanel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setButtonColor(QColor);
    ~ColorPanel();
    void initUI();
protected slots:
    void colorChangedEvent();

signals:
    void getColor(const QColor&);

private:
    QPushButton *btnMainColor;
};

#endif
