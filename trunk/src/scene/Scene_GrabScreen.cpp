#include "Scene_GrabScreen.h"
#include "ui_Scene_GrabScreen.h"

Scene_GrabScreen::Scene_GrabScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene_GrabScreen)
{
    ui->setupUi(this);
}

Scene_GrabScreen::~Scene_GrabScreen()
{
    delete ui;
}
