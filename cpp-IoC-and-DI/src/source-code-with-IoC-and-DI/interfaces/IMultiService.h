#ifndef I_MULTI_SERVICE_H
#define I_MULTI_SERVICE_H

#include <string>
#include <vector>

class IMultiService 
{
public:
    virtual ~IMultiService() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual int compute(int x, int y) = 0;
    virtual std::string getName() const = 0;
    virtual bool sendData(const std::vector<uint8_t>& data) = 0;
    virtual double calculateLoad(float factor, int units) = 0;

    virtual void log(const std::string& message) = 0;
    virtual void log(const std::string& message, int severity) = 0;  // Sobrecarga

    virtual bool isAvailable() const = 0;

protected:

    IMultiService() = default;

    IMultiService(const IMultiService&) = default;

    IMultiService& operator=(const IMultiService&) = default;

    IMultiService(IMultiService&&) = default;
};

#endif // I_MULTI_SERVICE_H
