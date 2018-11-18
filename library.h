#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMainWindow>

namespace Ui {
class Library;
}

class Library : public QMainWindow
{
    Q_OBJECT

public:
    explicit Library(QWidget *parent = nullptr);
    ~Library();

private:
    Ui::Library *ui;
};

#endif // LIBRARY_H
