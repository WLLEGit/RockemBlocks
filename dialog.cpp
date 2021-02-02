#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->mask->setStyleSheet("QPushButton{border-image:url(:/pic/Record/mask.png);}");
    ui->titleLabel->setAlignment(Qt::AlignCenter);
    ui->titleLabel->setStyleSheet("color:white;");
    ui->titleLabel->setFont(QFont("Microsoft YaHei", 8, 600));
    ui->descriptionLabel->setFont(QFont("Microsoft YaHei", 8, 600));
    ui->pushButton->setImage(":/pic/Setting/button.png", ":/pic/Setting/button_hover.png", ui->pushButton->width(), ui->pushButton->height());
    ui->pushButton->setSound(":/sound/button_mouseover.wav", ":/sound/button_mouseleave.wav", ":/sound/button_press.wav", ":/sound/button_release.wav");
    ui->pushButton->setLabel("确定");
    ui->lineEdit->setStyleSheet("color:white;background-color:transparent;border:0px;");
}

Dialog::~Dialog()
{
    delete ui;
}

QLabel* Dialog::title(){
    return ui->titleLabel;
}
QLineEdit* Dialog::lineEdit(){
    return ui->lineEdit;
}
HoverButton* Dialog::button(){
    return ui->pushButton;
}
QLabel* Dialog::descriptionLabel(){
    return ui->descriptionLabel;
}
QPushButton* Dialog::mask(){
    return ui->mask;
}
