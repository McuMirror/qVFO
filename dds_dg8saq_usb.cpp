#include "dds_dg8saq_usb.h"
#include "usb_backend.h"
#include <iostream>

#define USBDEV_SHARED_VENDOR       0x16C0  /* VOTI */
#define USBDEV_SHARED_VENDOR_NAME  "www.obdev.at"
#define USBDEV_SHARED_PRODUCT      0x05DC  /* Obdev's free shared PID */
#define USBDEV_SHARED_PRODUCT_NAME "DG8SAQ-DDS"
/* Use obdev's generic shared VID/PID pair and follow the rules outlined
 * in firmware/usbdrv/USBID-License.txt.
 */

#define PSCMD_ECHO  0
#define PSCMD_GET   1
#define PSCMD_ON    2
#define PSCMD_OFF   3
/* These are the vendor specific SETUP commands implemented by our USB device */

using namespace std;

USB_Backend *usb_backend;

int rval; //for return values
unsigned char data[5] = {0, 0, 0, 0, 0}; //data to write

DDS_DG8SAQ::DDS_DG8SAQ()
{

}

DDS_DG8SAQ::~DDS_DG8SAQ()
{
}

int DDS_DG8SAQ::initDDS(int port_init, QString device_init, bool multiplier)
{
    if (multiplier)
        data[4] = 1;
    else
        data[4] = 0;

    rval = usb_backend->open_device_with_name(USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT, USBDEV_SHARED_PRODUCT_NAME);

    if (rval < 0)
        return(rval);
    else
    {
        rval = usb_backend->usb_transfer(data);  //enable serial mode
        return 0;
    }
}

void DDS_DG8SAQ::setFrequency(unsigned int n)
{
    int actual=0; //used to find out how many bytes were written
    unsigned char *data = new unsigned char[5]; //data to write
    int i;
    unsigned int k;

    k = n;
    for(i=0;i<4;i++)
    {
        data[i] = k % 255;
        k = k / 256;
    }
    
//    cout<<"Data->"<<data<<"<-"<<endl; //just to see the data we want to write : abcd
    cout<<"Writing Command..."<<endl;
    rval = usb_backend->usb_transfer(data);
    if(rval == 5) //we wrote the 5 bytes successfully
        cout<<"Writing Successful!"<<endl;
    else
        cout<<"Write Error:"<< rval << " bytes:"<< actual <<endl;
}

void DDS_DG8SAQ::closePort()
{
    data[4] += 4;   //power down the DDS
    rval = usb_backend->usb_transfer(data);

    usb_backend->usb_close();
}

int DDS_DG8SAQ::getError()
{
    return(rval);
}

