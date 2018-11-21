#ifndef USER_H
#define USER_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>
//#include <QTableWidget>
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
    User();
    void Init();
    void ListInit();
    QSqlDatabase userDatabase;

//    ~User();

private:
//    QTableWidget * tableWidget;
    QTableView * tableView;
    QTextCodec * codec;
    QLabel * lbl_search;
    QLineEdit * te_search;
    QPushButton * searchBtn;

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

    QGridLayout * listLayout;
    QHBoxLayout * mainLayout;

    void createAction();

private slots:
    void tableViewSelect(const QModelIndex &index);
    void search();
};

#endif // USER_H
