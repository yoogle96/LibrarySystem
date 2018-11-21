#include "user.h"
#include "ui_user.h"
#include "login.h"
#include <QTableView>
#include <QTableWidget>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>

User::User(QString userNumberId)
{
    currentUserId = userNumberId;
    Init();
    createAction();
}

void User::Init(){
    ListInit();
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
    returnListLayout -> addWidget(returnTableView);
    returnListLayout -> addWidget(returnBtn);

    QVBoxLayout * contentLayout = new QVBoxLayout;
    contentLayout -> addLayout(subLayout);
    contentLayout -> addWidget(rentalBtn);

    mainLayout = new QHBoxLayout;
//    mainLayout -> addLayout(flayout);
    mainLayout -> addLayout(listLayout);
    mainLayout -> addLayout(contentLayout);
    mainLayout -> addLayout(returnListLayout);
    setLayout(mainLayout);

    resize(1000, 500);
}

void User::ListInit(){
    Login login;
    QSqlQueryModel * model = new QSqlQueryModel;
    QSqlQueryModel * reModel = new QSqlQueryModel;
    QSqlQuery * qry = new QSqlQuery(login.db);
    QSqlQuery * reQry = new QSqlQuery(login.db);
    qry->prepare("select id as id, title as 제목, author as 저자, publisher as 출판사 from Books");
    qry->exec();
    model->setQuery(*qry);
    tableView = new QTableView;
    tableView->setModel(model);
    QHeaderView *verticalHeader = tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

    reQry->prepare("select Books.title as 제목, Books.author as 저자, Rentals.rental_date as 대여일자, Rentals.return_date as 반납일자 From Rentals INNER JOIN Users, Books ON Rentals.user_id = :rent_userId and Users.id = :user_userId and Rentals.book_id = Books.id;");
    reQry->bindValue(":rent_userId", currentUserId);
    reQry->bindValue(":user_userId", currentUserId);
    reQry->exec();
    reModel -> setQuery(*reQry);
    returnTableView = new QTableView;
    returnTableView -> setModel(reModel);
    QHeaderView *reVerticalHeader = returnTableView->verticalHeader();
    reVerticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);

    qDebug() << (model->rowCount());
}

void User::createAction(){
    connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tableViewSelect(QModelIndex)));
    connect(searchBtn, SIGNAL(clicked()), this, SLOT(search()));
    connect(rentalBtn, SIGNAL(clicked()), this, SLOT(rental()));
}

void User::tableViewSelect(const QModelIndex &index){
    QString val = tableView -> model() -> data(index).toString();
    qDebug() << val;
    bookId = val;
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

void User::rental(){
    Login login;
    QSqlQuery * qry = new QSqlQuery(login.db);

    qry->prepare("INSERT INTO Rentals (user_id, book_id, rental_date, return_date) VALUES (:user_id, :book_id, DATETIME('NOW'), DATETIME('NOW', '7 days'));");
    qry->bindValue(":user_id", currentUserId);
    qry->bindValue(":book_id", bookId);

    if(qry->exec()){
        rentalCount();
        qDebug() << "실행됨" << currentUserId;
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry->lastError().text());
    }
}

void User::rentalCount(){
    Login login;
    QSqlQuery * qry = new QSqlQuery(login.db);
    int count = 0, totalCount = 0;

    qry->prepare("select count, total_count from Books where id = "+bookId+"");
    qDebug() << "북아이디" << bookId;
    if(qry->exec()){
        while(qry->next()){
            count = qry->value(0).toInt();
            totalCount = qry->value(1).toInt();
        }

        if(count >= 1){
            count = count -1;
            qry->prepare("update Books set count = :count where id = "+bookId+"");
            qry->bindValue(":count", count);
            qry->exec();
            lbl_count_result->setText(""+ QString::number(count) +" / "+QString::number(totalCount) +"");
        }
        if(count <= 0){
            rentalBtn->setEnabled(false);
        }
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry->lastError().text());
    }

}
