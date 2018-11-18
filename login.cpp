#include <QtGui>
#include "login.h"
#include "admin.h"
#include "user.h"
#include "ui_login.h"

Login::Login(){
    Init();
    createAction();
}

void Login::Init(){
    resize(500, 200);
    // tab들을 담을 tabWidget 생성
    tabWidget = new QTabWidget;
    tabWidget -> setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    tabWidget -> setTabShape(QTabWidget::Triangular);

    // user, admin 로그인텝 생성
    userLogin = new QWidget;
    adminLogin = new QWidget;

    // user, admin 로그인 ID, Password 텍스트필드 생성

    lbl_userId = new QLabel("UserID");
    lbl_userPassword = new QLabel("UserPassword");

    lbl_adminId = new QLabel("AdminID");
    lbl_adminPassword = new QLabel("AdminPassword");

    te_userId = new QLineEdit;
    te_userPassword = new QLineEdit;

    te_adminId = new QLineEdit;
    te_adminPassword = new QLineEdit;

    userLoginBtn = new QPushButton(tr("Login"));
    adminLoginBtn = new QPushButton(tr("Login"));

    QFormLayout * userLayout = new QFormLayout;
    userLayout -> addRow(lbl_userId, te_userId);
    userLayout -> addRow(lbl_userPassword, te_userPassword);
    userLayout -> addWidget(userLoginBtn);

    QFormLayout * adminLayout = new QFormLayout;
    adminLayout -> addRow(lbl_adminId, te_adminId);
    adminLayout -> addRow(lbl_adminPassword, te_adminPassword);
    adminLayout -> addWidget(adminLoginBtn);

    userLogin -> setLayout(userLayout);
    adminLogin -> setLayout(adminLayout);

    // tabWidget공간에 user,admin 로그인텝 추가
    tabWidget -> addTab(userLogin, tr("UserLogin"));
    tabWidget -> addTab(adminLogin, tr("AdminLogin"));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(tabWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("LoginBar"));
}

void Login::createAction(){
    connect(userLoginBtn,SIGNAL(clicked()), this,SLOT(userLoginAction()));
    connect(adminLoginBtn, SIGNAL(clicked()), this, SLOT(adminLoginAction()));
}

void Login::userLoginAction(){
    this -> hide();
    User user;
    user.setModal(true);
    user.exec();
}

void Login::adminLoginAction(){
    this -> hide();
    Admin admin;
    admin.setModal(true);
    admin.exec();
}



