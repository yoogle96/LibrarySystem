#include "user.h"
#include "ui_user.h"
#include "login.h"
#include <QTableView>
#include <QTableWidget>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>

User::User()
{
    ListInit();
    Init();
    createAction();
}

void User::Init(){
    codec = QTextCodec::codecForName("eucKR");
    QFont font("unifont", 10, QFont::Bold);

    lbl_search = new QLabel("Search");
    te_search = new QLineEdit("");

    lbl_title = new QLabel("제목 : ");
    lbl_author = new QLabel("저자 : ");
    lbl_publisher = new QLabel("출판사 : ");
    lbl_content = new QLabel("내용 : ");
    lbl_count = new QLabel("수량 : ");

    lbl_title_result = new QLabel("-");
    lbl_author_result = new QLabel("-");
    lbl_publisher_result = new QLabel("-");
    te_content_result = new QTextEdit("-");
    lbl_count_result = new QLabel("-");

    searchBtn = new QPushButton("검색");

    QFormLayout * flayout = new QFormLayout;
    flayout -> addRow(lbl_search, te_search);
    flayout -> addRow(te_search, searchBtn);
    QFormLayout * subLayout = new QFormLayout;
    subLayout -> addRow(lbl_title, lbl_title_result);
    subLayout -> addRow(lbl_author, lbl_author_result);
    subLayout -> addRow(lbl_publisher, lbl_publisher_result);
    subLayout -> addRow(lbl_content, te_content_result);
    subLayout -> addRow(lbl_count, lbl_count_result);

    listLayout = new QGridLayout;
    listLayout -> addWidget(tableView);

    QVBoxLayout * contentLayout = new QVBoxLayout;
    contentLayout -> addLayout(subLayout);

    mainLayout = new QHBoxLayout;
    mainLayout -> addLayout(flayout);
    mainLayout -> addLayout(listLayout);
    mainLayout -> addLayout(contentLayout);
    setLayout(mainLayout);

    resize(1000, 500);
}

void User::ListInit(){
    Login login;
    QSqlQueryModel * model = new QSqlQueryModel;
    QSqlQuery * qry = new QSqlQuery(login.db);
    qry->prepare("select id as id, title as 제목, author as 저자, publisher as 출판사 from Books");
    qry->exec();
    model->setQuery(*qry);
    tableView = new QTableView;
    tableView->setModel(model);
//    tableView->setColumnWidth(50,50);
    QHeaderView *verticalHeader = tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

//    verticalHeader->resize(100,100);
//    verticalHeader->setDefaultSectionSize(24);
    qDebug() << (model->rowCount());
}

void User::createAction(){
    connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tableViewSelect(QModelIndex)));
    connect(searchBtn, SIGNAL(clicked()), this, SLOT(search()));
}

void User::tableViewSelect(const QModelIndex &index){
    QString val = tableView -> model() -> data(index).toString();
    qDebug() << val;
    QSqlQuery qry;
    qry.prepare("select title, author, publisher, content, count, total_count from Books where id="+ val +"");
//    qDebug() << ;
    if(qry.exec()){
        while(qry.next()){
            lbl_title_result -> setText(qry.value(0).toString());
            lbl_author_result -> setText(qry.value(1).toString());
            lbl_publisher_result -> setText(qry.value(2).toString());
            te_content_result -> setText(qry.value(3).toString());
            lbl_count_result -> setText(""+qry.value(4).toString()+" / "+qry.value(5).toString()+"");
        }
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry.lastError().text());
    }
}

void User::search(){
    QString searchValue = te_search -> text();
    Login login;
    QSqlQueryModel * model = new QSqlQueryModel;
    QSqlQuery * qry = new QSqlQuery(login.db);
    qry->prepare("select id as id, title as 제목, author as 저자, publisher as 출판사 from Books where title LIKE '%"+searchValue+"%'");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    QHeaderView *verticalHeader = tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

    qDebug() << (model->rowCount());
}
