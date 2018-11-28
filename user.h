#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlQueryModel>


QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class User : public QDialog
{
    Q_OBJECT

public:
    User(QString userNumberId);
    void Init();
    void ListInit();
    QSqlDatabase userDatabase;

    QString bookId;
    QString rentalId;
    QString currentUserId;
    QString userId;
    QString userName;

private:
    QTableView * tableView;
    QTableView * returnTableView;
    QTextCodec * codec;
    QLabel * lbl_search;
    QLineEdit * te_search;
    QPushButton * searchBtn;
    QPushButton * returnBtn;

    QLabel * lbl_title;
    QLabel * lbl_title_result;
    QLabel * lbl_author;
    QLabel * lbl_author_result;
    QLabel * lbl_publisher;
    QLabel * lbl_publisher_result;
    QLabel * lbl_content;
    QTextEdit * te_content_result;
    QLabel * lbl_count;
    QLabel * lbl_count_result;

    QLabel * lbl_top_userId;
    QLabel * lbl_top_userName;


    QVBoxLayout * listLayout;
    QVBoxLayout * returnListLayout;
    QHBoxLayout * contentLayout;
    QHBoxLayout * topLayout;
    QVBoxLayout * mainLayout;


    QPushButton * rentalBtn;
    QPushButton * logoutBtn;

    void createAction();
    void bookCount(QString bookCountId);
    void reflashView();

private slots:
    void rentalTableViewSelect(const QModelIndex &index);
    void returnTableViewSelect(const QModelIndex &index);
    void search();
    void rentalAct();
    void returnAct();
    void logoutAct();
};

#endif // USER_H
