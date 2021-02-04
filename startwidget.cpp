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
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QTextCharFormat>

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);

    setFixedSize(1024, 768);
    setWindowTitle("Rockem Blocks");
    setWindowIcon(QIcon(":/pic/Gem/Gold.png"));
    gameWidget = new GameWidget(parent);
    bgLoading= new QMediaPlayer(this);
    bgMainmenu = new QMediaPlayer(this);
    InitWidget();

    // 大文件不好放入资源文件中，只能使用相对路径，编译前务必修改
    Q_ASSERT(QFileInfo(QApplication::applicationDirPath() + "/music/Loading.mp3").exists());
    bgLoading->setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath() + "./music/Loading.mp3"));
    bgMainmenu->setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath() + "./music/Mainmenu.mp3"));

    connect(bgLoading, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia)
            bgMainmenu->play();
    });     //切换到主背景音乐
    connect(bgMainmenu, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && !is_end)
            bgMainmenu->play();
    });     //循环播放
    connect(gameWidget, &GameWidget::returnToMenu, this, &StartWidget::ChildFrameClose);
    bgLoading->play();
    ShowBackground();
    QTimer::singleShot(1000, this, [=](){
        ShowButton();
        InitUser();
        recordWidget->raise();
        settingWidget->raise();

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
        if(gameWidget->userName == QStringLiteral("")){
            setNameDialog->show();
            return;
        }
        TransitionAnimation();
        gameWidget->setGeometry(this->geometry());
        is_end = true;
        gameWidget->is_end=false;
        bgLoading->stop();
        bgMainmenu->stop();
        gameWidget->bgGame->play();
        this->hide();
        Restore();
        gameWidget->show();
        QTimer::singleShot(500, this, [=](){
            gameWidget->start();
        });

    }) ;
    connect(settingButton, &MenuButton::clicked, settingWidget, &SettingWidget::show);
    connect(settingWidget->mask(), &QPushButton::clicked, settingWidget, &SettingWidget::hide);
    connect(recordButton, &MenuButton::clicked, this, &StartWidget::ShowRecordBoard);
}

void StartWidget::TransitionAnimation(){
    startButton->setVisible(false);
    recordButton->setVisible(false);
    settingButton->setVisible(false);
    exitButton->setVisible(false);
    userText->setVisible(false);
    maskUserText->setVisible(false);


    transitionLabel = new QLabel(this);
    transitionLabel->setVisible(true);
    QPropertyAnimation animation(transitionLabel, "geometry", this);
    animation.setDuration(2000);
    transitionLabel->setGeometry(startButton->x(), startButton->y(), startButton->width(), startButton->height());
    transitionLabel->setStyleSheet(QString("QLabel{border-image:url(:/pic/Gem/crystalball.png);}"));
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

void StartWidget::InitWidget(){
    //记录窗口
    recordWidget = new QWidget(this);
    mask = new QPushButton(recordWidget);
    recordLabel = new QLabel(recordWidget);
    recordTable = new QTableWidget(recordWidget);
    topRankLabel = new QLabel(recordWidget);

    recordWidget->setGeometry(0,0,this->width(), this->height());
    recordWidget->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    recordWidget->hide();

    mask->setStyleSheet("QPushButton{border-image:url(:/pic/Record/mask.png);}");
    mask->setGeometry(recordWidget->geometry());
    mask->setVisible(true);
    connect(mask, &QPushButton::clicked, recordWidget, &QWidget::hide);

    recordLabel->setStyleSheet("QLabel{border-image:url(:/pic/Record/record_board.png);}");
    recordLabel->setGeometry(recordWidget->width()/10*3, (recordWidget->height()-recordWidget->width()/5*3)/2 , recordWidget->width()/5*2, recordWidget->width()/5*3);
    recordLabel->setVisible(true);

    topRankLabel->setFont(QFont("MicroSoft YaHei", 15, 100));
    topRankLabel->setStyleSheet("QLabel{color:#f0f0f0;}");
    topRankLabel->setGeometry(recordLabel->x()+110, recordLabel->y()+110, 250, 30);
    topRankLabel->setAlignment(Qt::AlignCenter);

    recordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recordTable->setColumnCount(2);
    recordTable->setRowCount(7);
    recordTable->setGeometry(recordLabel->x() + 50, recordLabel->y()+170, recordLabel->width()-100, 300);
    recordTable->setFont(QFont("Microsoft YaHei", 10, 700));
    recordTable->setStyleSheet("QTableWidget{color:#f0f0f0;}");
    recordTable->horizontalHeader()->setVisible(false);
    recordTable->verticalHeader()->setVisible(false);
    recordTable->setShowGrid(false);
    recordTable->setStyleSheet("QTableWidget{background-color:transparent;border:0px;}");
    recordTable->setColumnWidth(0, recordTable->width()/2-1);
    recordTable->setColumnWidth(1, recordTable->width()/2-1);
    for(int i  = 0; i < 7; ++i)
        recordTable->setRowHeight(i, 40);

    //设置窗口
    settingWidget = new SettingWidget(1, 2, this);
    settingWidget->button()->setSound(":/sound/button_mouseover.wav", ":/sound/button_mouseleave.wav", ":/sound/button_press.wav", ":/sound/button_release.wav");
    connect(settingWidget->easyButton(), &HoverButton::clicked, [=](){
        gameWidget->setDifficulity(1);
        settingWidget->switchToDifficulty(1);

    });
    connect(settingWidget->defaultButton(), &HoverButton::clicked, [=](){
        gameWidget->setDifficulity(2);
        settingWidget->switchToDifficulty(2);
    });
    connect(settingWidget->hardButton(), &HoverButton::clicked, [=](){
        gameWidget->setDifficulity(3);
        settingWidget->switchToDifficulty(3);
    });
    connect(settingWidget->button(), &HoverButton::clicked, [=](){
        QTimer::singleShot(250, this, [=](){
            QApplication::exit();
        });
    });
}

void StartWidget::ShowRecordBoard(){
    QFile recordFile(QApplication::applicationDirPath() + "/record");
    recordFile.open(QIODevice::ReadOnly);
    QTextStream in(&recordFile);
    in.setCodec("utf-8");
    QString word;

    in >> word;
    for(int i = 0; i < 7 && !in.atEnd(); ++i){
        for(int j = 0; j <= 1; ++j){
            in >> word;
            if(i==0 && j==1)
                topRankLabel->setText(word);
            recordTable->setItem(i,j,new QTableWidgetItem(word));
        }
    }
    recordFile.close();
    recordWidget->show();
}

void StartWidget::InitUser(){
    QFile recordFile(QApplication::applicationDirPath() + "/record");
    if(recordFile.exists()){
        recordFile.open(QIODevice::ReadOnly);
        QTextStream in(&recordFile);
        in.setCodec("utf-8");
        QString defaultName;
        in >> defaultName;
        gameWidget->userName = defaultName;
    }
    else
        gameWidget->userName = "";
    recordFile.close();

    QTextCharFormat format;
    format.setTextOutline(QPen(Qt::black, 1.5,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    userText = new QTextEdit(this);
    userText->setReadOnly(true);
    userText->setFont(QFont("Microsoft YaHei", 15, 700));
    userText->setStyleSheet("border:none;background-color:transparent;color:white;font-weight:bold;");
    userText->mergeCurrentCharFormat(format);
    userText->setGeometry(20, this->height()-50, 300, 60);
    userText->setText(QString("%1欢迎回来").arg(gameWidget->userName));

    maskUserText = new QPushButton(this);
    maskUserText->setStyleSheet("background-color:transparent;border:0px;");
    maskUserText->setGeometry(userText->geometry());
    QTimer::singleShot(2000, userText, [=](){
        userText->show();
        maskUserText->show();
    });

    setNameDialog = new Dialog(this);
    setNameDialog->title()->setText("新用户");
    setNameDialog->descriptionLabel()->setText("输入用户名:");
    setNameDialog->setGeometry(0,0,1024,768);


    connect(maskUserText, &QPushButton::clicked, [=](){
        setNameDialog->show();
    });
    connect(setNameDialog->button(), &HoverButton::clicked, [=](){
        SetUser();
    });
    connect(setNameDialog->mask(), &QPushButton::clicked, setNameDialog, &Dialog::hide);
}

void StartWidget::SetUser(){
    gameWidget->userName = setNameDialog->lineEdit()->text();
    userText->setText(QString("%1欢迎回来").arg(gameWidget->userName));
    setNameDialog->hide();
    if(gameWidget->userName == QStringLiteral(""))
        return;

    QFile recordFile(QApplication::applicationDirPath() + "/record");
    if(recordFile.exists()){
        recordFile.open(QIODevice::ReadOnly);
        QTextStream text(&recordFile);
        text.setCodec("utf-8");
        QString firstLine;
        QStringList list;
        firstLine = text.readLine();
        while(!text.atEnd())
            list << text.readLine();
        recordFile.close();

        recordFile.setFileName(QApplication::applicationDirPath() + "/record");
        recordFile.open(QIODevice::Truncate | QIODevice::WriteOnly);
        text.setDevice(&recordFile);
        text.setCodec("utf-8");
        text << gameWidget->userName + '\n';
        for(QString line : list)
            text << line << '\n';
        recordFile.close();
    }
    else{
        recordFile.open(QIODevice::WriteOnly);
        QTextStream text(&recordFile);
        text.setCodec("utf-8");
        text << gameWidget->userName + '\n';
        recordFile.close();
    }
}

void StartWidget::ChildFrameClose(){
    this->setGeometry(gameWidget->geometry());
    is_end = false;
    gameWidget->is_end = true;
    gameWidget->bgGame->stop();
    bgMainmenu->play();
    gameWidget->hide();
    this->show();
}

void StartWidget::Restore(){
    setWindowOpacity(1);
    delete transitionLabel;
    startButton->setVisible(true);
    recordButton->setVisible(true);
    settingButton->setVisible(true);
    exitButton->setVisible(true);
    userText->setVisible(true);
    maskUserText->setVisible(true);
}
