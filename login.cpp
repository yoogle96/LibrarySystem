#include <QtGui>
#include <QDebug>
#include "login.h"
#include "admin.h"
#include "user.h"
#include "ui_login.h"



Login::Login(){
    Init();
    try {
        dbConnect();
    } catch (char const* errorContent) {
        QMessageBox::critical(this, tr("DB Connection Error"), errorContent);
    }
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
    te_userPassword -> setEchoMode(QLineEdit::Password);

    te_adminId = new QLineEdit;
    te_adminPassword = new QLineEdit;
    te_adminPassword -> setEchoMode(QLineEdit::Password);

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
    if(loginValue(0)){
        this -> hide();
        User user(userNumberId);
        user.setModal(true);
        user.exec();
    }
}

void Login::adminLoginAction(){
    if(loginValue(1)){
        this -> hide();
        Admin admin;
        admin.setModal(true);
        admin.exec();
    }
}

void Login::dbConnect(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if(db.open()){
    }
    else{
        throw "데이터베이스 연결 오류";
    }
}

bool Login::loginValue(int whoIs){
    QString id;
    QString password;
    QSqlQuery * qry = new QSqlQuery(db);

        if(whoIs == 0){
            id = te_userId -> text();
            password = te_userPassword -> text();

            if(qry->exec("select * from Users where user_id='"+ id +"' and user_password='"+ password +"'") == true){
                while(qry->next()){
                    userNumberId = qry->value(0).toString();
                    userId = qry->value(1).toString();
                    return true;
                }
                QMessageBox::critical(this, tr("DB Connection Error"), "아이디 또는 비밀번호를 잘못 입력하셨습니다");
            }
        }
        else{
            id = te_adminId -> text();
            password = te_adminPassword -> text();

            if(qry->exec("select * from Admins where admin_id='"+ id +"' and admin_password='"+ password +"'")){
                while(qry->next()){
                    return true;
                }
                QMessageBox::critical(this, tr("DB Connection Error"), "아이디 또는 비밀번호를 잘못 입력하셨습니다");
            }
        }
      return false;
}




