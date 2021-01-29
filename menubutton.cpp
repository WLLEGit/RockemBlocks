#include "menubutton.h"
#include <QTimer>


MenuButton::MenuButton(int r, int x, int y, int width, int height, QString path, QString path2, QWidget *parent) : QLabel(parent)
{
    pushLabel->setParent(parent);
    textButton->setParent(parent);

    pushLabel->setGeometry(x - r, y - r, 2*r, 2*r);
    buttonGif->setScaledSize(QSize(pushLabel->width(), pushLabel->height()));
    pushLabel->setMovie(buttonGif);
    buttonGif->start();

    textButton->setGeometry(x - r, y - r, 2*r, 2*r);
    textButton->setFlat(true);
    textButton->setStyleSheet(QString("QPushButton:!hover{border-image:url(%1);}").arg(path)  + \
                              QString("QPushButton:hover{border-image:url(%1);}").arg(path2));


    animation->setParent(parent);
    animation->setTargetObject(pushLabel);
    animation->setDuration(2000);
    animation->setPropertyName("geometry");
    animation->setStartValue(QRect(x - r, height, 2*r, 2*r));
    animation->setEndValue(QRect(x - r, y - r, 2*r, 2*r));
    animation->setEasingCurve(QEasingCurve::InOutCubic);

    connect(textButton, &QPushButton::clicked, [=](){
        emit clicked();
    });
}

void MenuButton::show(){
    pushLabel->setVisible(true);
    animation->start();
    QTimer::singleShot(2000, textButton, [=](){
        textButton->setVisible(true);
    });
}

void MenuButton::setVisible(bool flag){
    pushLabel->setVisible(flag);
    textButton->setVisible(flag);
    QLabel::setVisible(flag);
}
