#include "Page_GrabDesktopArea.h"
#include "ui_Page_GrabDesktopArea.h"

Page_GrabDesktopArea::Page_GrabDesktopArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_GrabDesktopArea)
{
    ui->setupUi(this);
}

Page_GrabDesktopArea::~Page_GrabDesktopArea()
{
    delete ui;
}
