#include "Page_GrabCamera.h"
#include "ui_Page_GrabCamera.h"

Page_GrabCamera::Page_GrabCamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_GrabCamera)
{
    ui->setupUi(this);
}

Page_GrabCamera::~Page_GrabCamera()
{
    delete ui;
}
