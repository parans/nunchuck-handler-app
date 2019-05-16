#ifndef I2C_NUNCHUCK_H
#define I2C_NUNCHUCK_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_NODE "/dev/i2c-14"
#define ADDRESS 0x52
#define READ_BACK_INTERVAL 100
#define REGISTER 0x40
#define COMMAND 0x00
#define BYTE_DATA_LENGTH 6
#define DECODE_VALUE 0x17
#define ERROR_BYTE 0xFF


//Opening the device file associated with the adapter
static int open_nunchuck(const char* file, mode_t mode)
{
    int fd = open(file, mode);
    if(fd<0)
    {
        fprintf(stderr, "\nError opening the file:%d", fd);
        exit(fd);
    }
    return fd;
}

//Setting slave address
static void setupSlave(int fd, unsigned short address)
{
    if(ioctl(fd, I2C_SLAVE, address)<0)
    {
        fprintf(stderr,"\nFailed to set slave address:%m");
        exit(2);
    }
}

//Initializing the device
static void initialize_nunchuck(int fd, unsigned short reg, unsigned short cmd)
{
    int status;
    if((status=i2c_smbus_write_byte_data(fd, reg, cmd))<0)
    {
        fprintf(stderr,"\nFailed to initialize nunchuck:%m");
        exit(status);
    }
}

//smbus write byte
static void smbus_write_byte(int fd, unsigned short cmd)
{
    int status;
    if((status=i2c_smbus_write_byte(fd, cmd))<0)
    {
        fprintf(stderr,"\nFailed to write data:%m");
        exit(status);
    }
}

//decode the data read from nunchuck
static char decode_nunchuck_byte(unsigned char data)
{
    data = (data ^ DECODE_VALUE) + DECODE_VALUE;
    return data;
}

//smbus read byte
static void smbus_read_byte(int fd, unsigned char* buffer)
{
    int i;
    char value;
        for(i=0; i<BYTE_DATA_LENGTH; i++)
        {
            value = i2c_smbus_read_byte(fd);
            buffer[i] = decode_nunchuck_byte(value);
            if(value==ERROR_BYTE)
            {
                fprintf(stderr,"\nFailed to read data:%m");
                break;
            }
        }
}

#endif // I2C_NUNCHUCK_H
