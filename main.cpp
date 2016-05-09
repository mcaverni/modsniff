#include "slave.h"
using namespace std;
int main(int argc, char *argv[])
{
	string device;
	if(argc > 1)
		device = argv[1];
	else
	{
    	cout << "Please insert serial device name (COM): ";
	    getline(cin, device);
    }
    
    Slave s(device, 0, 19200, 'E', 8, 1);
    s.setup(2500, 2500, 2500, 2500);
    return s.start();
}
