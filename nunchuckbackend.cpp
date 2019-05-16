#include "nunchuckbackend.h"
#include <iostream>

NunchuckBackend::NunchuckBackend(QObject *parent) :
    QThread(parent)
{  	
}

int NunchuckBackend::parseZbit(unsigned char data)
{
    return (data & 0x01);
}

int NunchuckBackend::parseCbit(unsigned char data)
{
   return (data & 0X02)>>1;
}

//Checks if p1(x,y) is equal to p2(x, y)

bool NunchuckBackend::isEqual(QPoint p1, QPoint p2)
{
	bool value;
	if(p1.rx() == p2.rx() && p1.ry() == p2.ry())
		value = true;
	else
		value = false;
	return value;
}

//Overriden run() method

void NunchuckBackend::run()
{
	        
	unsigned char* buffer = new unsigned char[BYTE_DATA_LENGTH];
        //QPoint po(129, 129); //Origin
        //QPoint ppd = po; // ppd : previous point read by the device
	QPoint p1, p2; // pd : present point read by the device  
	int xd, yd;        
        short currentZBit;
        
        int fd = open_nunchuck(I2C_NODE, O_RDWR);
        setupSlave(fd, ADDRESS);
        initialize_nunchuck(fd, REGISTER, COMMAND);
        
        smbus_write_byte(fd, COMMAND);
        smbus_read_byte(fd, buffer);
        QPoint po(buffer[0], 255-buffer[1]);
        QPoint ppd = po;      
        
        do{
		/*get data from the device*/		
		smbus_write_byte(fd, COMMAND);
        	smbus_read_byte(fd, buffer);
		QPoint pd(buffer[0], 255-buffer[1]);/*New point read by the  device*/								
		
		//Trajectory computation logic	
 		if(this->isEqual(po, pd))
		{
			/*This is the case that occurs, when the pen is released after moving the trajectory till the end of the window	*/
			if(!this->isEqual(ppd, po))		
			{
				//This logic ensures that the trajectory stays at the last point it was released before coming to the origin
				p1 = p1;				
				p2 = p2;					
			}							
		}
		else
		{
			if(this->isEqual(pd, ppd))
			{
				//This is the case when the joystick is held constant
				p1 = p2;
				xd = pd.rx()-po.rx();
				yd = pd.ry()-po.ry();
				p2.setX(p1.rx()+xd/SCALE_F);
				p2.setY(p1.ry()+yd/SCALE_F);
			}
			else //This case happens when the joystick is moved
			{
				//Joy stick is moved for the 1st time
				if(this->isEqual(po, ppd))
				{
					p1 = po;
					xd = pd.rx()-po.rx();
					yd = pd.ry()-po.ry();
					p2.setX(p1.rx()+xd/SCALE_F);
					p2.setY(p1.ry()+yd/SCALE_F);
					ppd = pd;
				}
				else
				{
					p1 = p2;					
					xd = pd.rx()-po.rx();
					yd = pd.ry()-po.ry();
					p2.setX(p1.rx()+xd/SCALE_F);
					p2.setY(p1.ry()+yd/SCALE_F);
					ppd =pd;
			
				}
			}
		}
		
		/*p1.setY(p1.ry());
		p2.setY(p2.ry());*/
		currentZBit = parseZbit(buffer[5]);					
		NunchuckPacket packet(p1, p2, parseCbit(buffer[5]));
		emit signal_update(packet);		
		emit signal_colorChange(currentZBit);		
            	usleep(10000);
            			
        }while(1);
        
        close(fd);
        delete buffer;
}
