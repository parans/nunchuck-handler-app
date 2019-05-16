#ifndef NUNCHUCKBACKEND_H
#define NUNCHUCKBACKEND_H

#include <QThread>
#include <QPoint>
#include "i2c_nunchuck.h"

#define SCALE_F 8

// This class describes the data which is passed by the backend to the frontend on signalling
class NunchuckPacket{

public:

    QPoint point_1;
    QPoint point_2;    
    short Cbit;

    NunchuckPacket()
    {
        point_1 = QPoint();
        point_2 = QPoint();
        Cbit = 0;                
    }

    NunchuckPacket(QPoint point_1, QPoint point_2, short Cbit)
    {
        this->point_1 = point_1;
        this->point_2 = point_2;
        this->Cbit = Cbit;                  
    }
};

/* This is the backend class and is responsible for collecting data from the device,
and encompasses the logic for computing points of the trajectory*/

class NunchuckBackend : public QThread
{
    Q_OBJECT    
    int parseCbit(unsigned char data);
    int parseZbit(unsigned char data);    
    bool isEqual(QPoint p1, QPoint p2);

public:    
    explicit NunchuckBackend(QObject *parent = 0);     
    void run();

signals:

    void signal_update(NunchuckPacket);
    void signal_colorChange(short zbit);

public slots:
    
};

#endif // NUNCHUCKBACKEND_H
