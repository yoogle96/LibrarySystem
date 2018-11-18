#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtWidgets>

class QTabWidget;
class QMenu;
class QHBOxLayuout;

class Login : public QWidget
{
    Q_OBJECT

public:
    Login();
    ~Login();

private:
    QTabWidget * tabWidget;
    QWidget * userLogin;
    QWidget * adminLogin;

private slots:

};

#endif // LOGIN_H
