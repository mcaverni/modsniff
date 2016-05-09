#ifndef SLAVE_H
#define SLAVE_H
#include <modbus/modbus-rtu.h>
#include <iostream>
#include <string>
#include <cerrno>
#include <cstdio>

using namespace std;

class Slave
{
public:
    explicit Slave(string device, int address, int baudrate, char parity, int databits, int stopbits);
    ~Slave();
    void setup(int coils, int holding, int analog, int digital);
    bool start();

private:
    modbus_mapping_t *mMapping;
    bool mRunning;
    modbus_t *mContext;
    bool mSetupDone;
    int mAddress;
    int mRequests;

    void logRequest(int length, u_int8_t *request);
    string printInt(int length, u_int8_t *request);
    string printHex(int length, u_int8_t *request);
    void releaseContext();
    void printSeparator();
};

#endif // SLAVE_H
