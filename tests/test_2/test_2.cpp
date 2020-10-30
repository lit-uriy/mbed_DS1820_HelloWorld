
#include "mbed.h"
#include "DS1820.h"



#define DATA_PIN        A0
#define MAX_PROBES      16



DigitalIn mybutton(USER_BUTTON);
//bool mybutton = true;

DigitalOut syncroPin(LED2);
//bool test = false;

DS1820* makeDevice(PinName name, int num_devices);

void convertTemperature(DS1820 *dev, int num_device);

void printTemperature(float temp, int num_devices);
void printRam(DS1820 *dev, int num_devices);

Serial port(USBTX, USBRX);

int main() {


    char command[80];

    while(1){
        port.printf("\r\n------------ Ready ----------------\r\n");
        port.scanf("%s", command);

        if (strncmp(command , "test1()", sizeof("test1()")) == 0){
            port.printf("Command test1(): Finding multiple devices...\r\n");
            DS1820 *probe[MAX_PROBES];

            // Initialize the probe array to DS1820 objects
            int num_devices = 0;
            while(DS1820::unassignedProbe(DATA_PIN)) {
                num_devices++;
                DS1820 *dev = makeDevice(DATA_PIN, num_devices);
                probe[num_devices-1] = dev;
                if (num_devices == MAX_PROBES)
                    break;
            }
            if (num_devices){
                port.printf("Found %d device(s)\r\n\n", num_devices);

                while(1) {
                    convertTemperature(probe[0], 0);
                    float temp = 0;
                    for (int i = 0; i<num_devices; i++){
                        float t = probe[i]->temperature();
                        temp += t;
                        printTemperature(t, i+1);
                    }
                    port.printf("Mean temperature: %3.1f %sC\r\n", temp/num_devices, (char*)(248));
                    port.printf("\r\n");
                }
            }else{
//                error("No devices!\r\n");
                port.printf("No devices!\r\n");
            }

        }else if (strncmp(command , "test2()", sizeof("test2()")) == 0){
            port.printf("Command test2(): Finding single devices...\r\n");


            DS1820 *probe = makeDevice(DATA_PIN, 1);


            port.printf("Found %d device(s)\r\n\n", 1);

            while(1) {
                convertTemperature(probe, 1);
                printTemperature(probe->temperature(), 1);
                port.printf("\r\n");
            }
        }else{
            port.printf("Unknown command: %s\r\n", command);
        }
    }
}


DS1820* makeDevice(PinName name, int num_devices)
{
    char romString[2*8+1]; // в два раза больше символов + замыкающий нуль

    DS1820 *dev = new DS1820(name);
    dev->romCode(romString);
    port.printf("Found %d device, ROM=%s\r\n", num_devices, romString);
    port.printf("\tparasite powered: %s\r\n", dev->isParasitePowered()? "Yes": "No");
    char ramString[2*9+1]; // в два раза больше символов + замыкающий нуль
    dev->ramToHex(ramString);
    port.printf("\tRAM: %s\r\n", ramString);
    port.printf("\tresolution: %d bits\r\n", dev->resolution());
    port.printf("\r\n");

    return dev;
}

void convertTemperature(DS1820 *dev, int num_device)
{
    dev->convertTemperature(true, DS1820::all_devices);         //Start temperature conversion, wait until ready
}

void printTemperature(float temp, int num_devices)
{
    port.printf("Device %d returns %3.1f %sC\r\n", num_devices, temp, (char*)(248));
}


void printRam(DS1820 *dev, int num_devices)
{
    char ramString[2*9+1]; // в два раза больше символов + замыкающий нуль
    dev->ramToHex(ramString);
    port.printf("Device %d RAM: %s\r\n", num_devices, ramString);
}
