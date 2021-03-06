#ifndef WORKER1_H
#define WORKER1_H

#include "mbed.h"
#include "Containers/ylist.h"

#include "1Wire/one_wire.h"
#include "1Wire/y_ds1820.h"

class Worker1
{
public:
    Worker1();

    int exec();

private:
    void startFind();
    void nextFind();
    void findAll();
    void printNext();
    void printAll();


private:
    char romString[2*8+1]; // в два раза больше символов + замыкающий нуль
    OneWire wire;
    YList<OneWireDevice> list;
    int currentIndex;
};

#endif // WORKER1_H
