#include "startwidget.h"
#include "ui_startwidget.h"
#include <QMainWindow>
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLabel>
#include <QTimer>
#include <QFileInfo>
#include <QTime>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    setFixedSize(1024, 768);
    setWindowTitle("Rockem Blocks");
    gameWidget = new GameWidget(parent);
    bgLoading= new QMediaPlayer(this);
    bgMainmenu = new QMediaPlayer(this);

    // 大文件不好放入资源文件中，只能使用绝对路径，编译前务必修改
    Q_ASSERT(QFileInfo("E:\\WinterCode2020\\Phase3\\RockemBlocks\\music\\Loading.mp3").exists());
    bgLoading->setMedia(QUrl::fromLocalFile("E:\\WinterCode2020\\Phase3\\RockemBlocks\\music\\Loading.mp3"));
    bgMainmenu->setMedia(QUrl::fromLocalFile("E:\\WinterCode2020\\Phase3\\RockemBlocks\\music\\mainmenu.mp3"));

    connect(bgLoading, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia)
            bgMainmenu->play();
    });     //切换到主背景音乐
    connect(bgMainmenu, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && !is_end)
            bgMainmenu->play();
    });     //循环播放

    bgLoading->play();
    ShowBackground();
    QTimer::singleShot(1000, this, [=](){
        ShowButton();
    });

}

StartWidget::~StartWidget()
{
    delete ui;
}

void StartWidget::ShowBackground(){
    QPixmap pix;
    QLabel *pic = new QLabel(this);
    QPropertyAnimation *animation = new QPropertyAnimation(pic, "geometry",this);

    if(!pix.load(":/pic/background.png"))
        qDebug() << "导入背景图片错误";
    pic->setPixmap(pix);
    pic->setGeometry(0,0,pix.width(), pix.height());

    animation->setDuration(3000);
    animation->setStartValue(QRect(pic->x(), pic->y(), pic->width(), pic->height()));
    animation->setEndValue(QRect(pic->x(), this->height() - pic->height(), pic->width(), pic->height()));
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start();
}

void StartWidget::ShowButton(){
    startButton = new MenuButton(this->width()/10, this->width()/2, this->height()/2, this->width(), this->height(),\
                                 ":/pic/Menu/start.png", ":/pic/Menu/start_hover.png", this);
    recordButton = new MenuButton(this->width()/100*7, this->width()/6, this->height()/2, this->width(), this->height(),\
                                  ":/pic/Menu/record.png", ":/pic/Menu/record_hover.png", this);
    settingButton = new MenuButton(this->width()/100*7, this->width()/6*5, this->height()/2, this->width(), this->height(),\
                                   ":/pic/Menu/settings.png", ":/pic/Menu/setting_hover.png", this);


    exitButton = new HoverButton(this);
    exitButton->setGeometry(this->width()-70, this->height()-35, 60, 30);
    exitButton->setFlat(true);
    exitButton->setImage(":/pic/Menu/exit.png", ":/pic/Menu/exit_hover.png", 60, 30);
    exitButton->setSound(":/sound/button_mouseover.wav", ":/sound/button_mouseleave.wav", ":/sound/button_press.wav", ":/sound/button_release.wav");


    //显示按钮
    startButton->show();
    recordButton->show();
    settingButton->show();
    QTimer::singleShot(2000, this, [=](){
        exitButton->setVisible(true);
    });
    //链接按钮功能
    connect(exitButton, &QPushButton::clicked,[=](){
        QTimer::singleShot(250, this, [=](){
            QApplication::exit();
        });
    });
    connect(startButton, &MenuButton::clicked, [=](){
        gameWidget->setGeometry(this->geometry());
        TransitionAnimation();
        is_end = true;
        bgLoading->stop();
        bgMainmenu->stop();
        gameWidget->bgGame->play();
        this->hide();
        setWindowOpacity(1);
        gameWidget->show();
        QTimer::singleShot(500, this, [=](){
            gameWidget->start();
        });

    }) ;
    connect(settingButton, &MenuButton::clicked, [=](){

    }) ;
    connect(recordButton, &MenuButton::clicked, [=](){

    }) ;
}

void StartWidget::TransitionAnimation(){
    startButton->setVisible(false);
    recordButton->setVisible(false);
    settingButton->setVisible(false);
    exitButton->setVisible(false);


    QLabel* label = new QLabel(this);
    label->setVisible(true);
    QPropertyAnimation animation(label, "geometry", this);
    animation.setDuration(2000);
    label->setGeometry(startButton->x(), startButton->y(), startButton->width(), startButton->height());
    label->setStyleSheet(QString("QLabel{border-image:url(:/pic/Gem/crystalball.png);}"));
    animation.setStartValue(QRect(startButton->x(), startButton->y(), startButton->width(), startButton->height()));
    animation.setEndValue(QRect(-this->width()/4, -this->height()/4, this->width()/2*3, this->height()/2*3));
    animation.setEasingCurve(QEasingCurve::InOutCubic);
    animation.start();

    for(int i = 20; i > 0; i -= 1){
        QTime _time = QTime::currentTime().addMSecs(100);
        setWindowOpacity(i / 20.0);
        while(_time >= QTime::currentTime())
            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
}
