#include "settingwidget.h"
#include "ui_settingwidget.h"
#include <QDebug>

SettingWidget::SettingWidget(int type, int curDifficulty, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);
    this->hide();
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->mask->setStyleSheet("QPushButton{border-image:url(:/pic/Record/mask.png);}");
    ui->button->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", ui->button->width(), ui->button->height());

    if(type == 1){
        ui->difficultyLabel->setText("选择难度:");
        switchToDifficulty(curDifficulty);
        ui->button->setLabel("退出游戏");
    }
    else if(type == 2){
        setDifficulity(curDifficulty);
    }
    ui->easyDifficulty->setLabel("简单");             //label应在image后设置
    ui->defaultDifficulty->setLabel("默认");
    ui->hardDifficulty->setLabel("困难");
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::switchToDifficulty(int d){
    ui->easyDifficulty->setDisabled(false);
    ui->defaultDifficulty->setDisabled(false);
    ui->hardDifficulty->setDisabled(false);

    int w = 73, h = 27;
    ui->easyDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);
    ui->defaultDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);
    ui->hardDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);

    switch(d){
    case 1:
        ui->easyDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
        ui->easyDifficulty->setDisabled(true);
        break;
    case 2:
        ui->defaultDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
        ui->defaultDifficulty->setDisabled(true);
        break;
    case 3:
        ui->hardDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
        ui->hardDifficulty->setDisabled(true);
        break;
    }
}

QPushButton* SettingWidget::mask(){
    return ui->mask;
}

HoverButton* SettingWidget::button(){
    return ui->button;
}
HoverButton* SettingWidget::easyButton(){
    return ui->easyDifficulty;
}

HoverButton* SettingWidget::defaultButton(){
    return ui->defaultDifficulty;
}

HoverButton* SettingWidget::hardButton(){
    return ui->hardDifficulty;
}

void SettingWidget::setDifficulity(int curDifficulty){
    int w = 73, h = 27;
    ui->difficultyLabel->setText("当前难度:");
    ui->easyDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
    ui->defaultDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
    ui->hardDifficulty->setImage(":/pic/Setting/button_disabled.png", ":/pic/Setting/button_disabled.png", w, h);
    ui->button->setLabel("返回主菜单");
    ui->easyDifficulty->setDisabled(true);
    ui->defaultDifficulty->setDisabled(true);
    ui->hardDifficulty->setDisabled(true);

    switch(curDifficulty){
    case 1:
        ui->easyDifficulty->setEnabled(true);
        ui->easyDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);
        break;
    case 2:
        ui->defaultDifficulty->setEnabled(true);
        ui->defaultDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);
        break;
    case 3:
        ui->hardDifficulty->setEnabled(true);
        ui->hardDifficulty->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", w, h);
        break;
    }
}
