#ifndef DDS_H
#define DDS_H

#include <QString>

class DDS {
public:
    virtual ~DDS(){};
    virtual int initDDS(int port, QString device, bool multiplier) = 0;
    virtual void setFrequency(unsigned int n) = 0;
    virtual void closePort() = 0;
    virtual int getError() = 0;
};

#endif // DDS_H
