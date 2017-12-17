#ifndef PAGE_GRABDESKTOPAREA_H
#define PAGE_GRABDESKTOPAREA_H

#include <QWidget>

namespace Ui {
class Page_GrabDesktopArea;
}

class Page_GrabDesktopArea : public QWidget
{
    Q_OBJECT

public:
    explicit Page_GrabDesktopArea(QWidget *parent = 0);
    ~Page_GrabDesktopArea();

private:
    Ui::Page_GrabDesktopArea *ui;
};

#endif // PAGE_GRABDESKTOPAREA_H
