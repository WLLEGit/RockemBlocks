#include "gem.h"

Gem::Gem(int type, int len, int x, int y, QWidget *parent, int offset) : QPushButton(parent), _type(type), _x(x), _y(y), parent(parent)
{
    initGemPath();

    setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(gemPath[type]));
    setIconSize(QSize(len, len));
    setGeometry(len*x, len*(y+offset), len, len);
    setVisible(true);

    connect(this, &Gem::clicked, [=](bool){
        this->mouseClicked(this);
    });
}

void Gem::initGemPath(){
    gemPath[0] = ":/pic/Gem/Magic.png";
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
