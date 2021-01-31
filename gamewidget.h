#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QMediaPlayer>
#include <hoverbutton.h>
#include <QLabel>
#include <QProgressBar>
#include "gem.h"
#include <QPropertyAnimation>
#include <QTimer>
#include <QSound>
#include <vector>

constexpr int SCORE_PER_GEM = 5;        //每颗宝石分数
constexpr int DIFFICULITY = 5;          //宝石种类数
constexpr int RECT_BONUS = 5;           //田字形加分
constexpr int BONUS_HAVE_STRAIGHT = 5;  //有直边基础加分
constexpr int BONUS_PRE_STRAIGHT = 5;   //每一条直边加分（L型，T型，十字形）


struct BombInfo{
    bool is_straight;
    int num_straight;
};

enum Direction{Center, Left, Up, Down,  Right};

namespace Ui {
class GameWidget;
}

class GameWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    //背景音乐
    QMediaPlayer* bgGame;
    bool is_end= false;
    void start();

private:
    Ui::GameWidget *ui;
    HoverButton* menuButton, *resetButton;
    QLabel* scoreLabel, *decorateLabel;
    QProgressBar* progressBar;
    QWidget* boardWidget;
    int len;
    QTimer *progressTimer;
    int score=0;
    unsigned int gemBoard[10][10];          //第一个值为x，第二个值为y
    int fallBoard[10][10];
    Gem* gems[10][10];
    QSound* soundGo, *soundGood, *soundExcellent, *soundAwesome, *soundBadmove;
    bool is_acting=false;
    std::vector<Gem*> toBomb;

    void initBgm();
    void paintEvent(QPaintEvent *event) override;
    void initWidgets();
    void initScene();
    void initSound();
    bool is_lose();
    void act(Gem* gem);
    BombInfo gemBomb(Gem* gem, int type, Direction dir=Center);
    void fallAnimation(Gem* target, int h);
    void fall();
    void fillBoard();
    void end();
    int randomGem();
    void badMove(Gem* gem);
    void gemShack(Gem* gem);

    void showMenu();
    void reset();
    bool event(QEvent *event) override;

};

#endif // GAMEWIDGET_H
