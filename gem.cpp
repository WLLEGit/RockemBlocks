#include "gem.h"
#include <QDebug>

Gem::Gem(int type, int len, int x, int y, QWidget *parent, int offset) : QPushButton(parent), _type(type), _x(x), _y(y)
{
    initGemPath();

    setGeometry(len*x, len*(y+offset), len, len);
    setVisible(true);
    if(type != 0){
        setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gemPath[type]));
        setIconSize(QSize(len, len));
    }
    else{
        setStyleSheet(QString("QPushButton{background-color:transparent;border:0px;}"));
        gifLabel = new QLabel(this);
        gifLabel->setGeometry(0,0,this->width(), this->height());
        gif = new QMovie(gemPath[0], QByteArray(), this);
        gif->setScaledSize(QSize(this->width(), this->height()));
        gifLabel->setMovie(gif);
        gifLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        gifLabel->show();
        gif->start();
    }

    connect(this, &Gem::clicked, [=](bool){
        this->mouseClicked(this);
    });
}


void Gem::initGemPath(){
    gemPath[0] = ":/pic/Gem/Magic.gif";
    gemPath[1] = ":/pic/Gem/Red.png";
    gemPath[2] = ":/pic/Gem/orange.png";
    gemPath[3] = ":/pic/Gem/Green.png";
    gemPath[4] = ":/pic/Gem/Blue.png";
    gemPath[5] = ":/pic/Gem/violet.png";
    gemPath[6] = ":/pic/Gem/Silver.png";
    gemPath[7] = ":/pic/Gem/Gold.png";
}

void Gem::bomb(){
    delete this;
}
