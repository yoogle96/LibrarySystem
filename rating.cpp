#include "rating.h"
#include "login.h"
#include "ui_rating.h"

Rating::Rating(QString bookId)
{
    _bookId = bookId;
    ratingInit();
    connectionAct();
}

void Rating::ratingInit(){
    lbl_ratingTitle = new QLabel("평가하기");
    ratingSlider = new QSlider;
    sliderValue = new QLabel;
    ratingBtn = new QPushButton("평가");

    ratingSlider -> setMaximum(5);
    ratingSlider -> setMinimum(0);

    ratingMainLayout = new QHBoxLayout;
    ratingMainLayout -> addWidget(lbl_ratingTitle);
    ratingMainLayout -> addWidget(ratingSlider);
    ratingMainLayout -> addWidget(sliderValue);
    ratingMainLayout -> addWidget(ratingBtn);

    setLayout(ratingMainLayout);
}

void Rating::connectionAct(){
    connect(ratingSlider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
    connect(ratingBtn, SIGNAL(clicked()), this, SLOT(ratingAct()));
}

void Rating::setValue(int value){
    ratingSlider -> setValue(value);
    sliderValue -> setText(QString::number(value));
}

void Rating::ratingAct(){
    Login login;
    float currentRatingCount = 0.0;
    float currentRatingTotal = 0.0;
    float currentRating = 0.0;
    QSqlQuery * qry = new QSqlQuery(login.db);
    qry -> prepare("select rating_count, rating, rating_total from Books where id = :bookId");
    qry -> bindValue(":bookId", _bookId);
    if(qry->exec()){
        while(qry->next()){
            currentRatingCount = qry->value(0).toInt();
            currentRating = qry->value(1).toFloat();
            currentRatingTotal = qry->value(2).toInt();
        }
    }

    currentRatingCount += 1;
    currentRatingTotal += sliderValue -> text().toInt();
    currentRating = (currentRatingTotal / currentRatingCount);

    qry -> prepare("update Books set rating_count = :currentRatingCount, rating = round(:currentRating, 2), rating_total = :currentRatingTotal where id = :bookId");
    qry -> bindValue(":currentRatingCount", currentRatingCount);
    qry -> bindValue(":currentRating", currentRating);
    qry -> bindValue(":currentRatingTotal", currentRatingTotal);
    qry -> bindValue(":bookId", _bookId);
    qry->exec();
    this->close();
}

Rating::~Rating()
{
}
