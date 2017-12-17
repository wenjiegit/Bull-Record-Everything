#include "Page_GrabScrollArea.h"
#include "ui_Page_GrabScrollArea.h"

Page_GrabScrollArea::Page_GrabScrollArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_GrabScrollArea)
{
    ui->setupUi(this);
}

Page_GrabScrollArea::~Page_GrabScrollArea()
{
    delete ui;
}
