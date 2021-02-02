#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <hoverbutton.h>
#include <QLineEdit>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QLabel* title();
    QLineEdit* lineEdit();
    HoverButton* button();
    QLabel* descriptionLabel();
    QPushButton* mask();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
