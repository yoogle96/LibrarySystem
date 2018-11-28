#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


class QTabWidget;
class QMenu;
class QHBOxLayuout;
class QAction;
class QLineEdit;
class QGroupBox;
class QLabel;
class QPushButton;

class Login : public QDialog
{
    Q_OBJECT

public:
    Login();
    QSqlDatabase db;
    void dbConnect();
    QString userId;
    QString userNumberId;

private:
    QTabWidget * tabWidget;
    QWidget * userLogin;
    QWidget * adminLogin;

    QLabel * lbl_userId;
    QLabel * lbl_userPassword;

    QLabel * lbl_adminId;
    QLabel * lbl_adminPassword;

    QLineEdit * te_userId;
    QLineEdit * te_userPassword;

    QLineEdit * te_adminId;
    QLineEdit * te_adminPassword;

    QPushButton * userLoginBtn;
    QPushButton * adminLoginBtn;

    void Init();
    void createAction();
    bool loginValue(int whoIs);

private slots:
    void userLoginAction();
    void adminLoginAction();

};

#endif // LOGIN_H
