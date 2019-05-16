#include "nunchuckfrontend.h"
#include <iostream> 

//This is the frontend's constructor, it initializes the backend and starts the backend.

Nunchuckfrontend::Nunchuckfrontend(QWidget *parent) :
    QWidget(parent)
{
    linesImage = new QImage(W_SIZE, W_SIZE, QImage::Format_ARGB32);
    rectangle = new QRect(10, 25, W_SIZE, W_SIZE);    
    this->pen = QPen(Qt::black, 2);
    this->backend = new NunchuckBackend(this);
    qRegisterMetaType<NunchuckPacket>("NunchuckPacket");
    connect(backend, SIGNAL(signal_update(NunchuckPacket)), SLOT(on_update_signalled(NunchuckPacket)), Qt::QueuedConnection);
    connect(backend, SIGNAL(signal_colorChange(short)), SLOT(on_colorChange_signalled(short)), Qt::QueuedConnection);
    this->backend->start();
    this->setBaseSize(W_SIZE, W_SIZE);
}

//Signal handler, handles the color change logic

void Nunchuckfrontend::on_colorChange_signalled(short zbit)
{
	static int i=0;
	if(zbit==1)
		return;
		
	else
	{
		if(i==0)
		{
			this->pen.setColor(Qt::blue);
			i++;
		}
		else if(i==1)
		{
			this->pen.setColor(Qt::black);
			i++;
		}
		else 
		{
			this->pen.setColor(Qt::red);
			i = 0;
		}		
	}	
}

//Signal handler, invoked by the backend on receiving new data from the device

void Nunchuckfrontend::on_update_signalled(NunchuckPacket lp){	
		
	if(lp.Cbit == 0)    		
	{
		delete this->linesImage;
		linesImage = new QImage(W_SIZE, W_SIZE, QImage::Format_ARGB32);
		this->update();
		return;
	}	
	
	QPainter imagePainter(this->linesImage);
	imagePainter.setRenderHint(QPainter::Antialiasing, true);
		    			    				
	imagePainter.setPen(this->pen);
	
	int x1 = lp.point_1.rx()*SCALE_FACTOR;
	x1 = x1>W_SIZE? W_SIZE : x1;
	int y1 = lp.point_1.ry()*SCALE_FACTOR;
	y1 = y1>W_SIZE? W_SIZE : y1;
	int x2 = lp.point_2.rx()*SCALE_FACTOR;
	x2 = x2>W_SIZE? W_SIZE : x2;
	int y2 = lp.point_2.ry()*SCALE_FACTOR;
	y2 = y2>W_SIZE? W_SIZE : y2;
    	
    	imagePainter.drawLine(x1, y1, x2, y2);

    	this->setAutoFillBackground(false);
    	this->update();    
}

void Nunchuckfrontend::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(*this->rectangle, *this->linesImage);
}

Nunchuckfrontend::~Nunchuckfrontend()
{
    delete backend;
    delete linesImage;
    delete rectangle;
}
