#ifndef PAGE_GRABCAMERA_H
#define PAGE_GRABCAMERA_H

#include <QWidget>

namespace Ui {
class Page_GrabCamera;
}

class Page_GrabCamera : public QWidget
{
    Q_OBJECT

public:
    explicit Page_GrabCamera(QWidget *parent = 0);
    ~Page_GrabCamera();

private:
    Ui::Page_GrabCamera *ui;
};

#endif // PAGE_GRABCAMERA_H
