#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QFileInfo>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);  //设置为独立窗口
    bgGame = new QMediaPlayer(this);
    QFileInfo file("E:\\WinterCode2020\\Phase3\\RockemBlocks\\music\\Classic.mp3");
    Q_ASSERT(file.exists());
    bgGame->setMedia(QUrl::fromLocalFile("E:\\WinterCode2020\\Phase3\\RockemBlocks\\music\\Classic.mp3"));

}

GameWidget::~GameWidget()
{
    delete ui;
}
