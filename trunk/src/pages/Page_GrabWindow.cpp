#include "Page_GrabWindow.h"
#include "ui_Page_GrabWindow.h"

Page_GrabWindow::Page_GrabWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_GrabWindow)
{
    ui->setupUi(this);
}

Page_GrabWindow::~Page_GrabWindow()
{
    delete ui;
}
