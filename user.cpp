#include "user.h"
#include "rating.h"
#include "ui_user.h"
#include "login.h"
#include <QTableView>
#include <QTableWidget>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QSqlRecord>

User::User(QString userNumberId)
{
    currentUserId = userNumberId;
    Init();
    createAction();
}

void User::Init(){
    tableView = new QTableView;
    returnTableView = new QTableView;
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
    rentalBtn = new QPushButton("대여");
    logoutBtn = new QPushButton("로그아웃");

    topLayout = new QHBoxLayout;
    lbl_top_userId = new QLabel("");
    lbl_top_userName = new QLabel("");

    topLayout -> addWidget(lbl_top_userId);
    topLayout -> addWidget(lbl_top_userName);
    topLayout -> addWidget(logoutBtn);
    topLayout -> setAlignment(Qt::AlignRight);


    QFormLayout * flayout = new QFormLayout;
    flayout -> addRow(lbl_search, te_search);
    flayout -> addRow(te_search, searchBtn);
    QFormLayout * subLayout = new QFormLayout;
    subLayout -> addRow(lbl_title, lbl_title_result);
    subLayout -> addRow(lbl_author, lbl_author_result);
    subLayout -> addRow(lbl_publisher, lbl_publisher_result);
    subLayout -> addRow(lbl_content, te_content_result);
    subLayout -> addRow(lbl_count, lbl_count_result);

    listLayout = new QVBoxLayout;
    listLayout -> addLayout(flayout);
    listLayout -> addWidget(tableView);

    returnBtn = new QPushButton("반납");
    returnListLayout = new QVBoxLayout;
    returnListLayout -> addLayout(topLayout);
    returnListLayout -> addWidget(returnTableView);
    returnListLayout -> addWidget(returnBtn);

    QVBoxLayout * subContentLayout = new QVBoxLayout;
    subContentLayout -> setContentsMargins(0,50,0,0);
    subContentLayout -> addLayout(subLayout);
    subContentLayout -> addWidget(rentalBtn);

    contentLayout = new QHBoxLayout;
    contentLayout -> addLayout(listLayout);
    contentLayout -> addLayout(subContentLayout);
    contentLayout -> addLayout(returnListLayout);
//    contentLayout -> addLayout(topLayout);

    mainLayout = new QVBoxLayout;
//    mainLayout -> addLayout(returnListLayout);
    mainLayout -> addLayout(contentLayout);
    setLayout(mainLayout);

    reflashView();
    resize(1200, 550);
}

void User::createAction(){
    connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(rentalTableViewSelect(QModelIndex)));
    connect(returnTableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(returnTableViewSelect(QModelIndex)));
    connect(searchBtn, SIGNAL(clicked()), this, SLOT(search()));
    connect(rentalBtn, SIGNAL(clicked()), this, SLOT(rentalAct()));
    connect(returnBtn, SIGNAL(clicked()), this, SLOT(returnAct()));
    connect(logoutBtn, SIGNAL(clicked()), this, SLOT(logoutAct()));
}

void User::rentalTableViewSelect(const QModelIndex &index){
    int rowIndex;
    rowIndex= index.row();
    bookId = tableView->model()->data(tableView->model()->index(rowIndex,0)).toString();

    qDebug() << "bookId = " << bookId;
    qDebug() << "rentalId = " << rentalId;

    QSqlQuery qry;
    qry.prepare("select title, author, publisher, content, count, total_count from Books where id="+ bookId +"");

    if(qry.exec()){
        while(qry.next()){
            lbl_title_result -> setText(qry.value(0).toString());
            lbl_author_result -> setText(qry.value(1).toString());
            lbl_publisher_result -> setText(qry.value(2).toString());
            te_content_result -> setText(qry.value(3).toString());
            lbl_count_result -> setText(""+qry.value(4).toString()+" / "+qry.value(5).toString()+"");

            if(qry.value(4).toInt() == 0){
                qDebug() << "toInt" << qry.value(4).toInt();
                rentalBtn->setEnabled(false);
            }
            else{
                rentalBtn->setEnabled(true);
            }
        }
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry.lastError().text());
    }
}

void User::returnTableViewSelect(const QModelIndex &index){
    int rowIndex;

    rowIndex= index.row();
    rentalId = returnTableView -> model() -> data(returnTableView -> model() -> index(rowIndex,0)).toString();
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
}

void User::rentalAct(){
    Login login;
    QSqlQuery * qry = new QSqlQuery(login.db);

    qry->prepare("INSERT INTO Rentals (user_id, book_id, rental_date, return_date) VALUES (:user_id, :book_id, DATETIME('NOW'), DATETIME('NOW', '7 days'));");
    qry->bindValue(":user_id", currentUserId);
    qry->bindValue(":book_id", bookId);

    if(qry->exec()){
        qry->exec("update Books set count = count - 1 where id = "+ bookId +"");
        qDebug() << "실행됨" << currentUserId;
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry->lastError().text());
    }

    bookCount(bookId);
    reflashView();
}

void User::returnAct(){
    Login login;
    QSqlQuery * qry = new QSqlQuery(login.db);
    QString rentBookId;

    qry->prepare("select book_id from Rentals where id = "+ rentalId +"");
    if(qry -> exec()){
        while(qry->next()){
            rentBookId = qry->value(0).toString();
        }
    }

    qry->exec("update Books set count = count + 1 where id = "+ rentBookId +"");
    qDebug() << "렌트북아이디 = " << rentBookId;
    qry->exec("delete from Rentals where id = "+ rentalId +"");

    bookCount(rentBookId);

    Rating rating(rentBookId);

    rating.exec();
    rating.setWindowFlags(Qt::WindowCloseButtonHint);

    reflashView();
}

void User::bookCount(QString bookCountId){
    Login login;
    QSqlQuery * qry = new QSqlQuery(login.db);
    int count = 0, totalCount = 0;

    qry->prepare("select count, total_count from Books where id = "+bookCountId+"");
    if(qry->exec()){
        while(qry->next()){
            count = qry->value(0).toInt();
            totalCount = qry->value(1).toInt();
        }

        if(count >= 1){
            lbl_count_result->setText(""+ QString::number(count) +" / "+QString::number(totalCount) +"");
        }
        if(count <= 0){
            lbl_count_result->setText(""+ QString::number(count) +" / "+QString::number(totalCount) +"");
            rentalBtn->setEnabled(false);
        }
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry->lastError().text());
    }
}

void User::reflashView(){
    Login login;
    QSqlQueryModel * model = new QSqlQueryModel;
    QSqlQuery * qry = new QSqlQuery(login.db);

    qry->prepare("select user_id, name from Users where id = "+currentUserId+"");
    if(qry->exec()){
        while(qry->next()){
            lbl_top_userId -> setText(qry->value(0).toString());
            lbl_top_userName -> setText(qry->value(1).toString());
        }
    }

    qry->prepare("select id as id, title as 제목, author as 저자, publisher as 출판사, rating as 평점 from Books");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    // id 값을 숨긴다.
    tableView->hideColumn(0);
    QHeaderView *verticalHeader = tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlQueryModel * reModel = new QSqlQueryModel;
    QSqlQuery * reQry = new QSqlQuery(login.db);

    reQry->prepare("select Rentals.id as id, Books.title as 제목, Books.author as 저자, Rentals.rental_date as 대여일자, Rentals.return_date as 반납일자 From Rentals INNER JOIN Users, Books ON Rentals.user_id = :rent_userId and Users.id = :user_userId and Rentals.book_id = Books.id;");
    reQry->bindValue(":rent_userId", currentUserId);
    reQry->bindValue(":user_userId", currentUserId);
    reQry->exec();
    reModel -> setQuery(*reQry);
    returnTableView -> setModel(reModel);
    returnTableView -> hideColumn(0);
    QHeaderView *reVerticalHeader = returnTableView->verticalHeader();
    reVerticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void User::logoutAct(){
    qDebug() << "로그아웃 실행";
    Login login;
    this -> hide();
    login.setModal(true);
    login.exec();
}
