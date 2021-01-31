#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "hoverbutton.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QPropertyAnimation>

class MenuButton : public QLabel
{
    Q_OBJECT
public:
    explicit MenuButton(int r, int x, int y, int width, int height, QString path, QString path2, QWidget *parent = nullptr);

    void setSound(QString pathHover, QString pathLeave, QString pathPress, QString pathRelease);

    void show();
    void setVisible(bool);
    int x(){return pushLabel->x();}
    int y(){return pushLabel->y();}
    int width(){return pushLabel->width();}
    int height(){return pushLabel->height();}

private:
    QLabel* pushLabel = new QLabel;
    HoverButton *textButton = new HoverButton;
    QMovie *buttonGif = new QMovie(":/pic/Gem/crystalball.gif");
    QPropertyAnimation* animation = new QPropertyAnimation;
signals:
    void clicked();
};

#endif // MENUBUTTON_H

