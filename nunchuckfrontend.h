/* Defines the application frontend class,
The responsibilities of the frontend involves,
painting and updating the widget when signalled*/

#ifndef NUNCHUCKFRONTEND_H
#define NUNCHUCKFRONTEND_H

#define W_SIZE 1024
#define SCALE_FACTOR 2

#include <QWidget>
#include "qpainter.h"
#include "nunchuckbackend.h"

class Nunchuckfrontend : public QWidget //Extends QWidget
{
    Q_OBJECT
    NunchuckBackend* backend;    
    QImage* linesImage;
    QRect* rectangle;    
    QPen pen;

public:

    explicit Nunchuckfrontend(QWidget *parent = 0);
    void paintEvent(QPaintEvent* event);    
    ~Nunchuckfrontend();
    
signals:
    
public slots:

    void on_update_signalled(NunchuckPacket lp); //signal handler, invoked by the backend when new data is received by the backend
    void on_colorChange_signalled(short zbit);
};

#endif // NUNCHUCKFRONTEND_H
