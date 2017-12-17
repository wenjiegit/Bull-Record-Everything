#include "Scene_RecordVideo.h"
#include "ui_Scene_RecordVideo.h"

Scene_RecordVideo::Scene_RecordVideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene_RecordVideo)
{
    ui->setupUi(this);
}

Scene_RecordVideo::~Scene_RecordVideo()
{
    delete ui;
}
