#include "usb_backend.h"
#include <iostream>
#include <qstring.h>

using namespace std;

libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
libusb_device_handle *dev_handle; //a device handle
libusb_context *ctx = NULL; //a libusb session
ssize_t cnt; //holding number of devices in list
int r; //for return values

USB_Backend::USB_Backend()
{
}

USB_Backend::~USB_Backend()
{
}

int USB_Backend::open_device_with_name(uint16_t vid, uint16_t pid, QString name)
{
    r = libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0)
    {
        cout<<"Init Error "<<r<<endl; //there was an error
        return -4;
    }
    libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0)
    {
        cout<<"Get Device Error"<<endl; //there was an error
        return -5;
    }
    cout<<cnt<<" Devices in list."<<endl;

    unsigned char strDesc[256];
    QString QstrDesc;
    bool found = false;
    for (int idx = 0; idx < cnt; ++idx)
    {
        libusb_device *device = devs[idx];
        libusb_device_descriptor desc = {0};

        r = libusb_get_device_descriptor(device, &desc);

        cout << "Vendor:Device = " << desc.idVendor << ":" << desc.idProduct << endl;
        if ((desc.idVendor == vid) && (desc.idProduct == pid))
        {
            r = libusb_open (device, &dev_handle);
            if (desc.iProduct > 0)
            {
                 r = libusb_get_string_descriptor_ascii
                          (dev_handle, desc.iProduct, strDesc, 256);
                 QstrDesc = QString::fromAscii(reinterpret_cast<const char*>(strDesc));
                 if (QstrDesc == name)
                 {
                     idx = cnt;
                     found = true;
                     cout << "Found: " << QstrDesc.toStdString() << endl;
                 }
            }
            if (!found)
                libusb_close (dev_handle);
        }
    }

//    dev_handle = libusb_open_device_with_vid_pid(ctx, USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it
    if (!found)
    {
        dev_handle = NULL;
        cout<<"Cannot open device"<<endl;
        return(-6);
    }
    else
    {
        cout<<"Device Opened"<<endl;
        if(libusb_kernel_driver_active(dev_handle, 0) == 1) //find out if kernel driver is attached
        {
            cout<<"Kernel Driver Active"<<endl;
            if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
                cout<<"Kernel Driver Detached!"<<endl;
        }
        r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
        if(r < 0)
        {
            cout<<"Cannot Claim Interface"<<endl;
            return -7;
        }
        cout<<"Claimed Interface"<<endl;

        return(0);
    }
}

int USB_Backend::usb_transfer(unsigned char *data)
{
    r = libusb_control_transfer(dev_handle,
                                (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT),
                                5, 1, 0, data, 5, 5000);
    return (r);
}

void USB_Backend::usb_close()
{
    r = libusb_release_interface(dev_handle, 0); //release the claimed interface
    if(r!=0)
    {
        cout<<"Cannot Release Interface"<<endl;
//        return 1;
    }
    cout<<"Released Interface"<<endl;

    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the
}

