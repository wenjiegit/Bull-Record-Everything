#ifndef PAGE_GRABWINDOW_H
#define PAGE_GRABWINDOW_H

#include <QWidget>

namespace Ui {
class Page_GrabWindow;
}

class Page_GrabWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Page_GrabWindow(QWidget *parent = 0);
    ~Page_GrabWindow();

private:
    Ui::Page_GrabWindow *ui;
};

#endif // PAGE_GRABWINDOW_H
