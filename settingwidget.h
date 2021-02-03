#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <hoverbutton.h>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(int type, int curDifficulty=2, QWidget *parent = nullptr);
    ~SettingWidget();
    void switchToDifficulty(int d);
    QPushButton* mask();
    HoverButton* button();
    HoverButton* easyButton();
    HoverButton* defaultButton();
    HoverButton* hardButton();
    void setDifficulity(int curDifficulty);

private:
    Ui::SettingWidget *ui;
};

#endif // SETTINGWIDGET_H
