#include <QWidget>
#include "login.h"
#include "ui_login.h"

Login::Login(){
    resize(500, 200);
    tabWidget = new QTabWidget;
    tabWidget -> setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    tabWidget -> setTabShape(QTabWidget::Triangular);

    userLogin = new QWidget;
    adminLogin = new QWidget;

    tabWidget -> addTab(userLogin, tr("UserLogin"));
    tabWidget -> addTab(adminLogin, tr("AdminLogin"));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(tabWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("LoginBar"));
}


Login::~Login()
{

}
