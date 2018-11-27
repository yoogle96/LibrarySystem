#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QtWidgets>

#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>


class Admin : public QDialog{
    Q_OBJECT

public:
    Admin();
    ~Admin();
    QSqlDatabase userDatabase;
    QString bookId;

private:
    QTableView * tableView;
    QVBoxLayout * listLayout;
    QVBoxLayout * listSubLayout;
    QHBoxLayout * mainLayout;
    QFormLayout * flayout;
    QHBoxLayout * btnLayout;

    QLabel * lbl_title;
    QLabel * lbl_author;
    QLabel * lbl_publisher;
    QLabel * lbl_content;
    QLabel * lbl_count;
    QLabel * lbl_totalCount;

    QLineEdit * title;
    QLineEdit * author;
    QLineEdit * publisher;
    QTextEdit * content;
    QLineEdit * count;
    QLineEdit * totalCount;

    QPushButton * updateBtn;
    QPushButton * deleteBtn;
    QPushButton * addBtn;

    void Init();
    void setAdminLayout();
    void setTableView();
    void actionConnect();

private slots:
    void selectTableView(const QModelIndex &index);
    void updateBtnAct();
    void deleteBtnAct();
    void addBtnAct();
};

#endif // ADMIN_H
