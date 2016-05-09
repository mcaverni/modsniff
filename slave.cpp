#include "slave.h"
Slave::Slave(string device, int address, int baudrate, char parity, int databits, int stopbits)
{
    mRunning = true;
    mSetupDone = false;
    mContext = modbus_new_rtu(device.c_str(), baudrate, parity, databits, stopbits);
    mAddress = address;
    mRequests = 0;
}

Slave::~Slave()
{
	cout << "Closing the sniffer... ";
    mRunning = false;
    if(mSetupDone){
        modbus_mapping_free(mMapping);
        releaseContext();
    }
    cout << "Done." << endl;
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
			printSeparator();
            mRequests++;
            logRequest(len, mBuffer);
        }
    }
    delete [] mBuffer;
    return mRunning;
}


void Slave::logRequest(int length, u_int8_t *request) {
	printf("Request Number: %d\n", mRequests);
    // **************** message length
    ;
    // **************** slave address
    int address = request[0];  // byte 0
    // **************** operation
    const char *operation;
    switch (request[1]) {  // byte 1
    case 1:
        operation = "Read Coils";
        break;
    case 2:
        operation = "Read Discrete Inputs";
        break;
    case 3:
        operation = "Read Holding Registers";
        break;
    case 4:
        operation = "Read Input Registers";
        break;
    case 5:
        operation = "Write Single Coils";
        break;
    case 6:
        operation = "Write Single Register";
        break;
    case 7:
        operation = "Read Exception Status";
        break;
    case 8:
        operation = "Diagnostics (serial line only)";
        break;
    case 11:
        operation = "Get Comm Event Counter (serial line only)";
        break;
    case 12:
        operation = "Get Comm Event Log (serial line only)";
        break;
    case 14:
        operation = "Read Device Identification";
        break;
    case 15:
        operation = "Write Multiple Coils";
        break;
    case 16:
        operation = "Write Multiple Registers";
        break;
    case 17:
        operation = "Report Server ID (serial line only)";
        break;
    case 20:
        operation = "Read File Record";
        break;
    case 21:
        operation = "Write File Record";
        break;
    case 22:
        operation = "Mask Write Register";
        break;
    case 23:
        operation = "Read/Write Multiple Registers";
        break;
    case 24:
        operation = "Read FIFO Queue";
        break;
    case 43:
        operation = "Read Device Identification";
        break;
    }
    // **************** start address
    u_int8_t msb = request[2];
    u_int8_t lsb = request[3];
    short start = (msb << 8) | (lsb & 0xff);  // bytes 2, 3
    // **************** count
    msb = request[4];
    lsb = request[5];
    short count = (msb << 8) | (lsb & 0xff);  // bytes 4, 5
    // **************** content
    string hex = printHex(length, request);
    
    
	// PRINT EVERYTHING    
    printf("\tReceived message length:         %d\n", length);
    printf("\tRequest is for SLAVE address:    %d\n", address);
    printf("\tOperation:                       %s\n", operation);
    printf("\tStarting reg address (from 0):   %d\n", start);
    printf("\tRegister count:                  %d\n", count);
	printf("\tContent (HEX):                   %s\n", hex.c_str());
}

string Slave::printHex(int length, u_int8_t *request) {
    string output = "";
    char *buff = new char[1];
    for (int i = 0; i < length; i++) {
    	sprintf(buff, "%#2x ", request[i]);
    	output += buff;
    }
    delete buff;
    return output;
}

string Slave::printInt(int length, u_int8_t *request) {
    string output = "";
    char *buff = new char[1];
    for (int i = 0; i < length; i++) {
    	sprintf(buff, "%d ", request[i]);
    	output += buff;
    }
    delete buff;
    return output;
}

void Slave::releaseContext()
{
    modbus_close(mContext);
    modbus_free(mContext);
}
