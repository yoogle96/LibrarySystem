#include "library.h"
#include "ui_library.h"

Library::Library(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);
}

Library::~Library()
{
    delete ui;
}
