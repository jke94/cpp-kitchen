#ifndef I_SERVICE_H
#define I_SERVICE_H

class IService
{
public:
    virtual ~IService() = default;
    virtual void execute() = 0;
};

#endif // I_SERVICE_H