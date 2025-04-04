#ifndef I_APPLICATION_H
#define I_APPLICATION_H

class IApplication 
{
public:
    virtual ~IApplication() = default;
    virtual void run() = 0;
};
#endif // I_APPLICATION_H