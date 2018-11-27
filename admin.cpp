#include "admin.h"
#include "ui_admin.h"
#include "login.h"

Admin::Admin(){
    Init();
    setTableView();
    setAdminLayout();
    actionConnect();
}

Admin::~Admin()
{

}

void Admin::Init(){
    tableView = new QTableView;
    listLayout = new QVBoxLayout;
    listSubLayout = new QVBoxLayout;
    btnLayout = new QHBoxLayout;
    mainLayout = new QHBoxLayout;

    lbl_title = new QLabel("제목 : ");
    lbl_author = new QLabel("저자 : ");
    lbl_publisher = new QLabel("출판사 : ");
    lbl_content = new QLabel("내용 : ");
    lbl_count = new QLabel("수량 : ");
    lbl_totalCount = new QLabel("총 수량 : ");

    title = new QLineEdit;
    author = new QLineEdit;
    publisher = new QLineEdit;
    content = new QTextEdit;
    count = new QLineEdit;
    totalCount = new QLineEdit;

    updateBtn = new QPushButton("수정");
    deleteBtn = new QPushButton("삭제");
    addBtn = new QPushButton("추가");
}

void Admin::setAdminLayout(){
    flayout = new QFormLayout;

    flayout -> addRow(lbl_title, title);
    flayout -> addRow(lbl_author, author);
    flayout -> addRow(lbl_publisher, publisher);
    flayout -> addRow(lbl_content, content);
    flayout -> addRow(lbl_count, count);
    flayout -> addRow(lbl_totalCount, totalCount);

    btnLayout -> addWidget(addBtn);
    btnLayout -> addWidget(updateBtn);
    btnLayout -> addWidget(deleteBtn);

    listLayout -> addWidget(tableView);
    listSubLayout -> addLayout(flayout);
    listSubLayout -> addLayout(btnLayout);

    mainLayout -> addLayout(listLayout);
    mainLayout -> addLayout(listSubLayout);

    setLayout(mainLayout);
    resize(1000,500);
}

void Admin::setTableView(){
    Login login;
    QSqlQueryModel * model = new QSqlQueryModel;
    QSqlQuery * qry = new QSqlQuery(login.db);

    qry->prepare("select id as id, title as 제목, author as 저자, publisher as 출판사 from Books");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    // id 값을 숨긴다.
    tableView->hideColumn(0);
    QHeaderView *verticalHeader = tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Admin::actionConnect(){
    connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(selectTableView(const QModelIndex)));
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtnAct()));
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateBtnAct()));
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteBtnAct()));
}

void Admin::selectTableView(const QModelIndex &index){
    qDebug() << "여기실행?";
    int rowIndex;
    rowIndex = index.row();
    bookId = tableView -> model() -> data(tableView -> model() -> index(rowIndex, 0)).toString();

    QSqlQuery qry;
    qry.prepare("select title, author, publisher, content, count, total_count from Books where id="+ bookId +"");

    if(qry.exec()){
        while(qry.next()){
            title -> setText(qry.value(0).toString());
            author -> setText(qry.value(1).toString());
            publisher -> setText(qry.value(2).toString());
            content -> setText(qry.value(3).toString());
            count -> setText(qry.value(4).toString());
            totalCount -> setText(qry.value(5).toString());
        }
    }
    else{
        QMessageBox::critical(this, tr("Erorr"),qry.lastError().text());
    }
}

void Admin::addBtnAct(){
    QString updateTitle;
    QString updateAuthor;
    QString updatePublisher;
    QString updateContent;
    QString updateCount;
    QString updateTotalCount;

    updateTitle = title -> text();
    updateAuthor = author -> text();
    updatePublisher = publisher -> text();
    updateContent = content -> toPlainText();
    updateCount = count -> text();
    updateTotalCount = totalCount -> text();

    QSqlQuery qry;
    qry.prepare("insert into Books (title, author, publisher, content, count, total_count) values(:updateTitle, :updateAuthor, :updatePublisher, :updateContent, :updateCount, :updateTotalCount)");
    qry.bindValue(":updateTitle", updateTitle);
    qry.bindValue(":updateAuthor", updateAuthor);
    qry.bindValue(":updatePublisher", updatePublisher);
    qry.bindValue(":updateContent", updateContent);
    qry.bindValue(":updateCount", updateCount);
    qry.bindValue(":updateTotalCount", updateTotalCount);
    qry.exec();

    setTableView();
}

void Admin::updateBtnAct(){
    QString updateTitle;
    QString updateAuthor;
    QString updatePublisher;
    QString updateContent;
    QString updateCount;
    QString updateTotalCount;

    updateTitle = title -> text();
    updateAuthor = author -> text();
    updatePublisher = publisher -> text();
    updateContent = content -> toPlainText();
    updateCount = count -> text();
    updateTotalCount = totalCount -> text();

    QSqlQuery qry;
    qry.prepare("update Books set title = :updateTitle, author = :updateAuthor, publisher = :updatePublisher, content = :updateContent, count = :updateCount, total_count = :updateTotalCount where id = "+bookId+"");
    qry.bindValue(":updateTitle", updateTitle);
    qry.bindValue(":updateAuthor", updateAuthor);
    qry.bindValue(":updatePublisher", updatePublisher);
    qry.bindValue(":updateContent", updateContent);
    qry.bindValue(":updateCount", updateCount);
    qry.bindValue(":updateTotalCount", updateTotalCount);
    qry.exec();

    setTableView();

//    setTableView();
}

void Admin::deleteBtnAct(){
    QSqlQuery qry;
    qry.prepare("delete from Books where id = "+bookId+"");
    qry.exec();

    setTableView();
    qDebug() << "delete여기실행";

}
