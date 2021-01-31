#include "hoverbutton.h"
#include <QDebug>

HoverButton::HoverButton(QWidget *parent) : QPushButton(parent)
{
    setStyleSheet("QPushButton{border:0px;}");
}

void HoverButton::setImage(QString pathNormal, QString pathHover, int width, int height){
    iconNormal = new QIcon(pathNormal);
    iconHover = (pathHover != QString("") ? new QIcon(pathHover) : new QIcon(pathNormal));

    this->setIcon(*iconNormal);
    this->setIconSize(QSize(width, height));
}

void HoverButton::setSound(QString pathHover, QString pathLeave, QString pathPress, QString pathRelease){
    soundHover = (pathHover != QString("") ? new QSound(pathHover, this) : nullptr);
    soundPress = (pathPress != QString("") ? new QSound(pathPress, this) : nullptr);
    soundRelease = (pathRelease != QString("") ? new QSound(pathRelease, this) : nullptr);
    soundLeave = (pathLeave != QString("") ? new QSound(pathLeave, this) : nullptr);
}

bool HoverButton::event(QEvent *e) {
    switch(e->type()){
    case QEvent::Enter:
        setIcon(*iconHover);
        if(soundHover)
            soundHover->play();
        break;
    case QEvent::Leave:
        setIcon(*iconNormal);
        if(soundLeave)
            soundLeave->play();
        break;
    case QEvent::MouseButtonPress:
        if(soundPress)
            soundPress->play();
        break;
    case QEvent::MouseButtonRelease:
        if(soundRelease)
            soundRelease->play();
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}
