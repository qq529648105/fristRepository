#include "readme.h"
#include "ui_readme.h"

readMe::readMe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::readMe)
{
    ui->setupUi(this);
}

readMe::~readMe()
{
    delete ui;
}
