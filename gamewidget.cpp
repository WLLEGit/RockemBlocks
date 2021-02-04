#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QFileInfo>
#include <QPainter>
#include <QBrush>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QTime>
#include<QPropertyAnimation>
#include <algorithm>

GameWidget::GameWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    //窗口基本设置
    setWindowFlag(Qt::Window);  //设置为独立窗口
    setWindowTitle("Rockem Blocks");
    setFixedSize(1024,768);
    setWindowIcon(QIcon(":/pic/Gem/Gold.png"));
    len = this->width()/100*6;

    //初始化场景、子控件及音效
    initBgm();
    initWidgets();
    initScene();
    initSound();
    settingWidget->raise();

    connect(resetButton, &QPushButton::clicked, this, &GameWidget::reset);
    connect(menuButton, &QPushButton::clicked, [=](){
        settingWidget->raise();
        settingWidget->show();
        progressTimer->stop();
    });
}


GameWidget::~GameWidget()
{
    delete ui;
}


void GameWidget::initBgm(){
    bgGame = new QMediaPlayer(this);
    bgGame->setVolume(90);
    //使用前务必修改为对应路径
    QFileInfo file(QApplication::applicationDirPath() + "/music/Classic.mp3");
    Q_ASSERT(file.exists());
    bgGame->setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/music/Classic.mp3"));
    connect(bgGame, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && !is_end)
            bgGame->play();
    });
}


void GameWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //背景图片
    painter.drawPixmap(0,0,this->width(), this->height(), QPixmap(":/pic/InGame/background.jpg"));

    //方格
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0,255));
    painter.setOpacity(0.6);
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if((i + j) % 2 == 0)
                painter.drawRect(this->width()/20*7 + len*j, (this->height()- 10*len)/2 + len*i, len, len);
    painter.setOpacity(0.5);
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if((i + j) % 2 == 1)
                painter.drawRect(this->width()/20*7 + len*j, (this->height()- 10*len)/2 + len*i, len, len);


    //棋盘上下装饰
    painter.setOpacity(1);
    painter.drawPixmap(this->width()/20*7, (this->height()- 10*len)/2 - len*3/8, 10*len, len*5/8, QPixmap(":/pic/InGame/top frame.png"));
    //painter.drawPixmap(this->width()/20*7, this->height() - len, 10*len, len*7/10, QPixmap(":/pic/InGame/bottom frame.png"));

    //记分器
    painter.drawPixmap(this->width()/20, this->height()/8, this->width()/5, (int)this->width() * 0.1157, QPixmap(":/pic/InGame/score.png"));

    //菜单栏
    painter.drawPixmap(this->width()/20, this->height()/20*11, this->width()/5, this->width()*14/50, QPixmap(":/pic/InGame/bottom widget classic.png"));

    QWidget::paintEvent(event);
}


void GameWidget::initWidgets(){
    menuButton = new HoverButton(this);
    resetButton = new HoverButton(this);
    scoreLabel = new QLabel("0", this);
    progressBar = new QProgressBar(this);
    decorateLabel = new QLabel(this);

    menuButton->setGeometry(this->width()/20+(int)this->width()/5 * 50.16/288, this->height()/20*11+this->width()*14/50 * 57.79/396, \
                            (int)this->width()*187.11/5/288, (int)this->width()*187.11/5/288);
    menuButton->setImage(":/pic/InGame/menu.png", ":/pic/InGame/menu_hover.png", menuButton->width(), menuButton->height());


    resetButton->setGeometry(this->width()/20 + (int)this->width()/5*93.18/288, this->height()/20*11 + this->width()*14/50*276.26/396, \
                             (int)this->width()/5*97.5/288, (int)this->width()/5*97.5/288);
    resetButton->setImage(":/pic/InGame/reset.png", ":/pic/InGame/reset_hover.png", resetButton->width(), resetButton->height());


    scoreLabel->setGeometry(this->width()/20 + (int)this->width()/5*22.68/280, this->height()/8 + (int)this->width()*0.1157*38.23/162, \
                            (int)this->width()/5*235.73/280, (int)this->width()*0.1157*57.43/162);
    scoreLabel->setFont(QFont("Microsoft YaHei", 12, 100));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("QLabel{color:white;}");

    decorateLabel->setStyleSheet("QLabel{border-image:url(:/pic/InGame/bottom frame.png);}");
    progressBar->setGeometry(this->width()/20*7 + 20, this->height() - len, 10*len - 40, len*7/10 - 15);
    decorateLabel->setGeometry(this->width()/20*7 - 20, this->height() - len - 10, 10*len + 30, len*7/10 + 5);
    progressBar->setRange(0,99);
    progressBar->setValue(0);
    progressBar->setFormat("%p%");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressBar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color:#24247e}");

    progressTimer = new QTimer(this);
    progressTimer->setInterval(1800);
    connect(progressTimer, &QTimer::timeout, [=](){
        if(progressBar->value() == 99)
            end();
        else
            progressBar->setValue(progressBar->value()+1);
    });

    settingWidget = new SettingWidget(2, DIFFICULITY-3, this);
    settingWidget->button()->setSound(":/sound/button_mouseover.wav", ":/sound/button_mouseleave.wav", ":/sound/button_press.wav", ":/sound/button_release.wav");
    settingWidget->setGeometry(0,0,1024,768);
    settingWidget->hide();
    connect(settingWidget->mask(), &QPushButton::clicked, [=](){
        settingWidget->hide();
        progressTimer->start();
    });
    connect(settingWidget->button(), &HoverButton::clicked, this, &GameWidget::returnToStart);

    settlementDialog = new Dialog(this);
    settlementDialog->hide();
    settlementDialog->setGeometry(0,0,1024,768);
    settlementDialog->descriptionLabel()->setText("您的得分为");
    settlementDialog->lineEdit()->setReadOnly(true);
    connect(settlementDialog->button(), &HoverButton::clicked, this, &GameWidget::returnToStart);
}

void GameWidget::initSound(){
    soundGo = new QSound(":/sound/voice_go.wav", this);
    soundGood = new QSound(":/sound/voice_good.wav", this);
    soundExcellent = new QSound(":/sound/voice_excellent.wav", this);
    soundAwesome = new QSound(":/sound/voice_awesome.wav", this);
    soundBadmove = new QSound(":/sound/badmove.wav", this);
    soundAct = new QSound(":/sound/combo_2.wav", this);
    soundFall = new QSound(":/sound/gem_hit.wav", this);
    soundGenerate = new QSound(":/sound/gem_hit.wav", this);
    soundUnbelievable = new QSound(":/sound/voice_unbelievable.wav", this);
    soundTimeUp = new QSound(":/sound/voice_timeup.wav", this);
    soundNoMoreMoves = new QSound(":/sound/voice_nomoremoves.wav", this);
    soundMagic = new QSound(":/sound/gem_shatters.wav", this);
}

int GameWidget::randomGem(bool allowMagic){
    if(allowMagic && QRandomGenerator::global()->bounded(0, 85) == 1)        //Magic方块1/85生成概率
        return 0;
    return QRandomGenerator::global()->bounded(1, DIFFICULITY+1);
}

void GameWidget::initScene(){
    boardWidget = new QWidget(this);
    boardWidget->show();
    boardWidget->setGeometry(1024/20*7, (768- 10*len)/2, 10*len, 10*len);
    QRandomGenerator::global()->fillRange(gemBoard[0], 100);
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j){
            gemBoard[i][j] = gemBoard[i][j] % DIFFICULITY + 1;
            gems[i][j] = new Gem(gemBoard[i][j], len, i, j, boardWidget);
            gems[i][j]->installEventFilter(this);
            connect(gems[i][j], &Gem::mouseClicked, this, &GameWidget::act);
        }
}

void GameWidget::start(){
    settingWidget->hide();
    settingWidget->raise();
    settlementDialog->hide();
    settlementDialog->raise();

    resetButton->setImage(":/pic/InGame/reset.png", ":/pic/InGame/reset_hover.png", resetButton->width(), resetButton->height());
    resetButton->setEnabled(true);
    progressBar->setValue(0);
    score = 0;
    scoreLabel->setNum(score);
    progressBar->setValue(0);
    soundGo->play();
    progressTimer->start();
}

void GameWidget::act(Gem* gem){
    is_acting = true;
    bool allowMagic = (gem->type()!=0);
    toBomb.clear();
    BombInfo bombInfo;
    bombInfo= (gem->type() != 0 ? gemBomb(gem, gem->type()) : magicBomb(gem));
    int cntStraight = bombInfo.num_straight;

    Q_ASSERT(bombInfo.cnt == (int)toBomb.size());
    if(gem->type()==0 && bombInfo.cnt < 3)
        bombInfo.cnt = 3;
    if(bombInfo.cnt < 3){
        QTimer::singleShot(290, this, [=](){
            is_acting=false;
        });
        toBomb.clear();
        badMove(gem);
        return;
    }

    soundAct->play();

    score += bombInfo.cnt * SCORE_PER_GEM;
    if(cntStraight >= 3 || (cntStraight == 2 && bombInfo.cnt >= 4))
        score += (BONUS_HAVE_STRAIGHT + BONUS_PRE_STRAIGHT * cntStraight);
    scoreLabel->setNum(score);

    if(cntStraight == 2 && bombInfo.cnt >= 4)
        soundGood->play();
    else if(cntStraight == 3)
        soundExcellent->play();
    else if(cntStraight >= 4)
        soundAwesome->play();
    else if(bombInfo.cnt == 6)
        soundGood->play();
    else if(bombInfo.cnt == 7)
        soundExcellent->play();
    else if(bombInfo.cnt == 8)
        soundAwesome->play();
    else if(bombInfo.cnt >= 9)
        soundUnbelievable->play();


    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            fallBoard[i][j]=0;

    for(Gem* gem : toBomb){
        gemBoard[gem->x()][gem->y()] = 100;
        fallBoard[gem->x()][gem->y()] = -1;
        for(int i = gem->y() - 1; i >= 0; --i)
            if(fallBoard[gem->x()][i] != -1)
                fallBoard[gem->x()][i]++;
    }
    for(Gem* gem : toBomb)
        gem->bomb();
    fall();
    QTimer::singleShot(200, this, [=](){
        fillBoard(allowMagic);
        QTimer::singleShot(450, this, [=](){
            is_acting=false;
            if(isFail()){
                if(resetButton->isEnabled())
                    reset();
                else
                    end();
            }
        });
    });
}

void GameWidget::fall(){
    QTimer::singleShot(500, this, [=](){
        soundFall->play();
    });

    for(int i = 0; i < 10; ++i)
        for(int j = 9; j >= 0; --j){
            if(fallBoard[i][j] != -1 && fallBoard[i][j] != 0 && gemBoard[i][j] != 100){
                gemBoard[i][j + fallBoard[i][j]] = gemBoard[i][j];
                gems[i][j]->setY(gems[i][j]->y() + fallBoard[i][j]);
                gems[i][j + fallBoard[i][j]] = gems[i][j];
                gemBoard[i][j] = 100;
                fallAnimation(gems[i][j], fallBoard[i][j]);
            }
        }
}

BombInfo GameWidget::gemBomb(Gem* gem, int type, Direction dir){
    bool flag=true;
    BombInfo bombInfo{1, true, 0}, tmp;
    toBomb.push_back(gem);
    int cntStraight=0, cntHand=0;

    for(auto _gem = toBomb.begin(); _gem!=toBomb.end(); ++_gem)                     //防止绕环
        if(*_gem == gems[gem->x()-1][gem->y()])
            flag=false;
    if(flag && dir!=Right && gem->x()>0 && gems[gem->x()-1][gem->y()]->type()==type){
        cntHand++;
        tmp = gemBomb(gems[gem->x()-1][gem->y()], type, Left);
        bombInfo.cnt += tmp.cnt;
        if(dir != Left && dir!=Center)
            bombInfo.is_straight = false;
        else if(!tmp.is_straight)
            bombInfo.is_straight = false;
        if(dir==Center && tmp.is_straight)
            cntStraight++;
    }

    flag = true;
    for(auto _gem = toBomb.begin(); _gem!=toBomb.end(); ++_gem)
        if(*_gem == gems[gem->x()+1][gem->y()])
            flag=false;
    if(flag && dir!=Left && gem->x()<9 && gems[gem->x()+1][gem->y()]->type()==type){
        cntHand++;
        tmp = gemBomb(gems[gem->x()+1][gem->y()], type, Right);
        bombInfo.cnt += tmp.cnt;
        if(dir != Right && dir!=Center)
            bombInfo.is_straight = false;
        else if(!tmp.is_straight)
            bombInfo.is_straight = false;
        if(dir==Center && tmp.is_straight)
            cntStraight++;

    }

    flag = true;
    for(auto _gem = toBomb.begin(); _gem!=toBomb.end(); ++_gem)
        if(*_gem == gems[gem->x()][gem->y()+1])
            flag = false;
    if(flag && dir!=Up && gem->y()<9 && gems[gem->x()][gem->y()+1]->type() == type){
        cntHand++;
        tmp = gemBomb(gems[gem->x()][gem->y()+1], type, Down);
        bombInfo.cnt += tmp.cnt;
        if(dir != Down && dir!=Center)
            bombInfo.is_straight = false;
        else if(!tmp.is_straight)
            bombInfo.is_straight = false;
        if(dir==Center && tmp.is_straight)
            cntStraight++;

    }

    flag = true;
    for(auto _gem = toBomb.begin(); _gem!=toBomb.end(); ++_gem)
        if(*_gem == gems[gem->x()][gem->y()-1])
            flag = false;
    if(flag && dir!=Down && gem->y()>0 && gems[gem->x()][gem->y()-1]->type() == type){
        cntHand++;
        tmp = gemBomb(gems[gem->x()][gem->y()-1], type, Up);
        bombInfo.cnt += tmp.cnt;
        if(dir != Up && dir!=Center)
            bombInfo.is_straight = false;
        else if(!tmp.is_straight)
            bombInfo.is_straight = false;
        if(dir==Center && tmp.is_straight)
            cntStraight++;

    }
    if(cntHand > cntStraight)
        bombInfo.num_straight = 0;
    else
        bombInfo.num_straight = cntStraight;
    return bombInfo;
}


BombInfo GameWidget::magicBomb(Gem *gem){
    soundMagic->play();
    BombInfo bombInfo{1, true, 0};
    int type;

    toBomb.push_back(gem);
    if(gem->y()!=0)
        type=gems[gem->x()][gem->y()-1]->type();
    else
        type=gems[gem->x()][gem->y()+1]->type();

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if(gems[i][j]->type() == type && gems[i][j] != gem){
                toBomb.push_back(gems[i][j]);
                bombInfo.cnt++;
            }
    return bombInfo;
}

void GameWidget::badMove(Gem *gem){
    gemShack(gem);
    soundBadmove->play();
}

void GameWidget::end(){
    progressTimer->stop();
    if(progressBar->value() == 99){
        settlementDialog->title()->setText("时间到");
        soundTimeUp->play();
    }
    else{
        settlementDialog->title()->setText("无法消去了");
        soundNoMoreMoves->play();
    }

    settlementDialog->lineEdit()->setText(QString::number(score));
    settlementDialog->raise();
    settlementDialog->show();

    QFile recordFile(QApplication::applicationDirPath() + "/record");
    recordFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&recordFile);
    out.setCodec("utf-8");
    out << userName << " " << score << "\n";
    recordFile.close();
    sort();
}

void GameWidget::fallAnimation(Gem *gem, int h){
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this);
    animation->setDuration(500);
    animation->setStartValue(gem->geometry());
    animation->setEndValue(QRect(gem->geometry().x(), gem->geometry().y() + len*h, gem->width(), gem->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start();
    QTimer::singleShot(1000, this, [=](){
        delete animation;
    });
}

void GameWidget::gemShack(Gem *gem){
    QPropertyAnimation* animation = new QPropertyAnimation(gem, "geometry", this);
    animation->setDuration(300);
    animation->setStartValue(gem->geometry());
    animation->setKeyValueAt(0.25, QRect(gem->geometry().x() + 10, gem->geometry().y(), gem->width(), gem->height()));
    animation->setKeyValueAt(0.75, QRect(gem->geometry().x() - 10, gem->geometry().y(), gem->width(), gem->height()));
    animation->setEndValue(gem->geometry());
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start();
    QTimer::singleShot(1000, this, [=](){
        delete animation;
    });
}

void GameWidget::fillBoard(bool allowMagic){
    int lack[10] = {0};

    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 10; ++j)
            if(fallBoard[i][j] == -1)
                lack[i]++;
            else if(fallBoard[i][j] != 0){
                lack[i] += fallBoard[i][j];
                break;
            }
    }

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < lack[i]; ++j){
            gems[i][lack[i]-j-1] = new Gem(randomGem(allowMagic), len, i, lack[i]-j-1, boardWidget, -lack[i]);
            gemBoard[i][lack[i]-j-1] = gems[i][lack[i]-j-1]->type();
            gems[i][lack[i]-j-1] -> installEventFilter(this);
            connect(gems[i][lack[i]-j-1], &Gem::mouseClicked, this, &GameWidget::act);
        }

    QTimer::singleShot(500, this, [=](){
        soundGenerate->play();
    });

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < lack[i]; ++j){
            fallAnimation(gems[i][lack[i]-j-1], lack[i]);
        }
}

bool GameWidget::eventFilter(QObject *watched, QEvent *event){              //动画进行中禁用点击事件
    if(watched->metaObject()->className() == QStringLiteral("Gem"))
        if(is_acting && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick))
            return true;
    return QMainWindow::eventFilter(watched, event);
}

void GameWidget::reset(){
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            gems[i][j]->bomb();
    delete boardWidget;
    initScene();
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j){
            gemShack(gems[i][j]);
        }

    resetButton->setImage(":/pic/InGame/reset_disabled.png", ":/pic/InGame/reset_disabled.png", resetButton->width(), resetButton->height());
    resetButton->setDisabled(true);
}

void GameWidget::setDifficulity(int d){
    switch (d) {
    case 1:
        DIFFICULITY = 4;
        break;
    case 2:
        DIFFICULITY = 5;
        break;
    case 3:
        DIFFICULITY = 6;
        break;
    }

    settingWidget->setDifficulity(DIFFICULITY-3);

    reset();
    resetButton->setImage(":/pic/InGame/reset.png", ":/pic/InGame/reset_hover.png", resetButton->width(), resetButton->height());
    resetButton->setEnabled(true);
}

void GameWidget::returnToStart(){
    settlementDialog->hide();
    settingWidget->hide();
    emit returnToMenu();
    QTimer::singleShot(500, this, [=](){
        for(int i = 0; i < 10; ++i)
            for(int j = 0; j < 10; ++j)
                gems[i][j]->bomb();
        delete boardWidget;
        initScene();
        this->hide();
    });
}

bool GameWidget::isFail(){
    bool flag=true;
    bool result=true;
    for(int i = 0; i < 10 && flag; ++i)
        for(int j = 0; j < 10 && flag; ++j)
            if(isEliminable(gems[i][j])){
                result=false;
                flag=false;
            }
    return result;

}

bool GameWidget::isEliminable(Gem *gem){                    //只判断是否有三个相邻，不记录具体个数
    int cnt=1;
    if(gem->x()>0 && gems[gem->x()-1][gem->y()]->type() == gem->type())
        cnt++;
    if(gem->x()<9 && gems[gem->x()+1][gem->y()]->type() == gem->type())
        cnt++;
    if(gem->y()>0 && gems[gem->x()][gem->y()-1]->type() == gem->type())
        cnt++;
    if(gem->y()<9 && gems[gem->x()][gem->y()+1]->type() == gem->type())
        cnt++;
    return cnt>=3;
}

void GameWidget::sort(){
    QFile recordFile(QApplication::applicationDirPath() + "/record");
    recordFile.open(QIODevice::ReadOnly);
    QTextStream file(&recordFile);
    file.setCodec("utf-8");
    std::vector<NameScorePair> list;
    NameScorePair pair;
    QString userName;

    list.clear();
    file >> userName;
    while (!file.atEnd()) {
        file >> pair.name;
        if(pair.name == QStringLiteral(""))
            break;
        file >> pair.score;
        list.push_back(pair);
    }
    recordFile.close();

    std::sort(list.begin(), list.end(), [](NameScorePair p1, NameScorePair p2){return p1.score > p2.score;}); //降序排列

    recordFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.setDevice(&recordFile);
    file.setCodec("utf-8");

    file << userName + '\n';
    for(NameScorePair p : list)
        file << p.name << " " << p.score << '\n';
    recordFile.close();
}
