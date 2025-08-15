#ifndef I_MAIN_SERVICE_H
#define I_MAIN_SERVICE_H

class IMainService
{
public:
    virtual ~IMainService() = default;

    virtual int computeNumber(int x, int y) = 0;

protected:
    
    IMainService() = default;

    IMainService(const IMainService&) = default;

    IMainService& operator=(const IMainService&) = default;

    IMainService(IMainService&&) = default;    
};

#endif // I_MAIN_SERVICE_H