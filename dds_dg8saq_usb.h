#include "dds.h"
#include <serialport.h>
#include <serialport-global.h>
#include <QMainWindow>

class DDS_DG8SAQ: public virtual DDS
{

public:
    DDS_DG8SAQ();
    virtual ~DDS_DG8SAQ();
    int initDDS(int port, QString device, bool multiplier);
    void setFrequency(unsigned int n);
    void closePort();
    int getError();
private:
};
