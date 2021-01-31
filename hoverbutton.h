#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include <QSound>

class HoverButton : public QPushButton
{
    Q_OBJECT
public:
    explicit HoverButton(QWidget *parent = nullptr);
    void setImage(QString pathNormal, QString pathHover, int width, int height);
    void setSound(QString pathHover, QString pathLeave, QString pathPress, QString pathRelease);


protected:
    bool event(QEvent *e) override;

private:
    QSound *soundHover = nullptr, *soundPress = nullptr, *soundRelease = nullptr, *soundLeave = nullptr;
    QIcon *iconNormal, *iconHover;

signals:

};

#endif // HOVERBUTTON_H
