#ifndef PAGE_GRABSCROLLAREA_H
#define PAGE_GRABSCROLLAREA_H

#include <QWidget>

namespace Ui {
class Page_GrabScrollArea;
}

class Page_GrabScrollArea : public QWidget
{
    Q_OBJECT

public:
    explicit Page_GrabScrollArea(QWidget *parent = 0);
    ~Page_GrabScrollArea();

private:
    Ui::Page_GrabScrollArea *ui;
};

#endif // PAGE_GRABSCROLLAREA_H
