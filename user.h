#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlQueryModel>


QT_BEGIN_NAMESPACE
//class QListView;
//class QListWidget;
//class QTableWidget;
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
    QString currentUserId;

//    ~User();

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


    QVBoxLayout * listLayout;
    QVBoxLayout * returnListLayout;
    QHBoxLayout * mainLayout;

    QPushButton * rentalBtn;

    void createAction();
    void rentalCount();

private slots:
    void tableViewSelect(const QModelIndex &index);
    void search();
    void rental();
};

#endif // USER_H
