#ifndef STARTWIDGET_H
#define STARTWIDGET_H
#include "menubutton.h"
#include "gamewidget.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include "hoverbutton.h"
#include <QTableWidget>
#include <settingwidget.h>
#include <QTextEdit>
#include <dialog.h>

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
    bool is_end = false;        //背景音乐循环是否结束
private:
    Ui::StartWidget *ui;
    QTextEdit* userText;
    QPushButton* maskUserText;
    Dialog* setNameDialog;
    QLabel* transitionLabel;
    //记录栏
    QWidget* recordWidget;
    QLabel *recordLabel, *topRankLabel;
    QPushButton* mask;
    QTableWidget* recordTable;

    //设置栏
    SettingWidget* settingWidget;

    void ShowBackground();  //背景图片
    void ShowButton();        //显示按钮
    void TransitionAnimation();
    void ShowRecordBoard();
    void HideRecordBoard();
    void InitWidget();
    void InitUser();
    void SetUser();
    void Restore();

    void ChildFrameClose();
};
#endif // STARTWIDGET_H
