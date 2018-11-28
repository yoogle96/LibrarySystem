#ifndef RATING_H
#define RATING_H

#include <QDialog>
#include <QSlider>
#include <QtWidgets>
#include <QSqlQuery>

class Rating : public QDialog
{
    Q_OBJECT

public:
    Rating(QString bookId);
    ~Rating();

private:
    QString _bookId;
    QHBoxLayout * ratingMainLayout;

    QLabel * lbl_ratingTitle;
    QLabel * sliderValue;
    QSlider * ratingSlider;

    QPushButton * ratingBtn;

    void ratingInit();
    void connectionAct();

signals:
    void valueChanged(int value);

private slots:
    void setValue(int value);
    void ratingAct();
};

#endif // RATING_H
