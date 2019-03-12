#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    menu_status=0;

}

menu::~menu()
{
    delete ui;
}

void menu::on_new_g_clicked()
{
    menu_status=1;
    emit enterData(1);
    this->hide();
}

void menu::on_continue_2_clicked()
{
    menu_status=2;
    emit enterData(2);
    this->hide();
}

void menu::on_exit_clicked()
{
    menu_status=0;
    emit enterData(0);
    this->hide();
}
