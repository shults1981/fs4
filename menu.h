#ifndef MENU_H
#define MENU_H

#include <QDialog>

namespace Ui {
class menu;
}

class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();

private:
    int menu_status;
    Ui::menu *ui;

signals:
    void enterData(int data);

private slots:
    void on_new_g_clicked();
    void on_continue_2_clicked();
    void on_exit_clicked();
};

#endif // MENU_H
