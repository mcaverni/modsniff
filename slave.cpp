#include "slave.h"
Slave::Slave(string device, int address, int baudrate, char parity, int databits, int stopbits)
{
    mRunning = true;
    mSetupDone = false;
    mContext = modbus_new_rtu(device.c_str(), baudrate, parity, databits, stopbits);
    mAddress = address;
}

Slave::~Slave()
{
    mRunning = false;
    if(mSetupDone){
        modbus_mapping_free(mMapping);
        releaseContext();
    }
}

void Slave::printSeparator()
{
    cout << "*************************************************" << endl;
}

void Slave::setup(int coils, int holding, int analog, int digital)
{
    printSeparator();
    if(mContext == NULL){
	    cerr << "Unable to create a context: " << modbus_strerror(errno) << endl;
    } else if(modbus_set_slave(mContext, mAddress) == -1){
        releaseContext();
	    cerr << "Can't address the slave: " << modbus_strerror(errno) << endl;
    } else if(modbus_connect(mContext) == -1) {
	    cerr << "Can't connect the context: " << modbus_strerror(errno) << endl;
        releaseContext();
    } else {
	    cout << "Creating the registers..." << endl;
        mMapping = modbus_mapping_new(coils, digital, holding, analog);
        if(mMapping == NULL){
	        cerr << "Unable to create the mapping: " << modbus_strerror(errno) << endl;
            releaseContext();
        }else{
            mSetupDone = true;
            cout << "Ready." << endl << endl;
        }
    }
    printSeparator();
}

bool Slave::start()
{
    if(!mSetupDone)
        return false;

    uint8_t *mBuffer = new uint8_t[100000];
    cout << "Starting" << endl;
    while(mRunning){ // FIXME di merda
        int len = modbus_receive(mContext, mBuffer);
        if(len == -1)
            cerr << "Error:" << modbus_strerror(errno) << endl;
        else {
            // clear the buffer and print it
            logRequest(len, mBuffer);
        }
    }
    delete [] mBuffer;
    return mRunning;
}


void Slave::logRequest(int length, u_int8_t *request) {
    /*
    string slaveAddress = "\tSlave address:";
    
    slaveAddress.append(QString::number(request[0]));  // byte 0

    QString startAddress = "\tStarting reg address:";
    u_int8_t msb = request[2];
    u_int8_t lsb = request[3];
    short combined = (msb << 8) | (lsb & 0xff);
    startAddress.append(QString::number(combined));  // bytes 2, 3
    startAddress.append(" (from 0)");

    QString regCount = "\tRegister count:";
    msb = request[4];
    lsb = request[5];
    combined = (msb << 8) | (lsb & 0xff);
    regCount.append(QString::number(combined));  // bytes 4, 5

    QString operation = "\tOperation:";
    switch (request[1]) {  // byte 1
    case 1:
        operation.append("Read Coils");
        break;
    case 2:
        operation.append("Read Discrete Inputs");
        break;
    case 3:
        operation.append("Read Holding Registers");
        break;
    case 4:
        operation.append("Read Input Registers");
        break;
    case 5:
        operation.append("Write Single Coils");
        break;
    case 6:
        operation.append("Write Single Register");
        break;
    case 7:
        operation.append("Read Exception Status");
        break;
    case 8:
        operation.append("Diagnostics (serial line only)");
        break;
    case 11:
        operation.append("Get Comm Event Counter (serial line only)");
        break;
    case 12:
        operation.append("Get Comm Event Log (serial line only)");
        break;
    case 14:
        operation.append("Read Device Identification");
        break;
    case 15:
        operation.append("Write Multiple Coils");
        break;
    case 16:
        operation.append("Write Multiple Registers");
        break;
    case 17:
        operation.append("Report Server ID (serial line only)");
        break;
    case 20:
        operation.append("Read File Record");
        break;
    case 21:
        operation.append("Write File Record");
        break;
    case 22:
        operation.append("Mask Write Register");
        break;
    case 23:
        operation.append("Read/Write Multiple Registers");
        break;
    case 24:
        operation.append("Read FIFO Queue");
        break;
    case 43:
        operation.append("Read Device Identification");
        break;
    }

    QString msg = "Received message length: ";
    msg.append(QString::number(length)).append("\n");

    msg.append(slaveAddress).append("\n");
    msg.append(operation).append("\n");
    msg.append(startAddress).append("\n");
    msg.append(regCount).append("\n");
    msg.append(printHex(length, request)).append("\n");
    msg.append(printInt(length, request));
    qDebug() << msg;
    */
    cout << "Called print method" << endl;
}

string Slave::printHex(int length, u_int8_t *request) {
    /*
    for (int i = 0; i < length * 2; i += 2) {
        msg.append("0x").append(hex.at(i)).append(hex.at(i + 1)).append(" ");
    }
    */
    string output = "\tContent (HEX): ";
    for (int i = 0; i < length; i++) {
        //msg.append(QString::number((int)array.at(i))).append(" ");
    }
    return output;
}

void Slave::releaseContext()
{
    modbus_close(mContext);
    modbus_free(mContext);
}

string Slave::printInt(int length, u_int8_t *request) {
    string output = "\tContent (INT): ";
    for (int i = 0; i < length; i++) {
        //msg.append(QString::number((int)array.at(i))).append(" ");
    }
    return output;
}
