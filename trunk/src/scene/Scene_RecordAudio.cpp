#include "Scene_RecordAudio.h"
#include "ui_Scene_RecordAudio.h"

Scene_RecordAudio::Scene_RecordAudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene_RecordAudio)
{
    ui->setupUi(this);
}

Scene_RecordAudio::~Scene_RecordAudio()
{
    delete ui;
}
