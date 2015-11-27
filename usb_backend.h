#ifndef USB_BACKEND_H
#define USB_BACKEND_H

#include <libusb.h>    /* this is libusb, see http://libusb.sourceforge.net/ */
#include <qstring.h>

class USB_Backend
{
public:
    USB_Backend();
    ~USB_Backend();
    int open_device_with_name(uint16_t vid, uint16_t pid, QString name);
    int usb_transfer(unsigned char *data);
    void usb_close();
};

#endif // USB_BACKEND_H
