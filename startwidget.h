#ifndef STARTWIDGET_H
#define STARTWIDGET_H
#include "menubutton.h"
#include "gamewidget.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include "hoverbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWidget; }
QT_END_NAMESPACE

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    StartWidget(QWidget *parent = nullptr);
    ~StartWidget();
    //下一层界面
    GameWidget* gameWidget;
    //按钮
    MenuButton* startButton, *recordButton, *settingButton;
    HoverButton* exitButton;
    //音效
    QMediaPlayer* bgLoading, *bgMainmenu;
    bool is_end = false;
private:
    Ui::StartWidget *ui;
    void ShowBackground();  //背景图片
    void ShowButton();        //显示按钮
    void TransitionAnimation();
};
#endif // STARTWIDGET_H
