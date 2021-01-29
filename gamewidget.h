#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    //背景音乐
    QMediaPlayer* bgGame;

private:
    Ui::GameWidget *ui;
};

#endif // GAMEWIDGET_H
