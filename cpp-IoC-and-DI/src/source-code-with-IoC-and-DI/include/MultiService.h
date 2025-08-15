#ifndef MULTISERVICE_H
#define MULTISERVICE_H

#include "IMultiService.h"

class MultiService final : public IMultiService
{
public:
    MultiService() = default;
    ~MultiService() = default;

    void start() override;
    void stop() override;

    int compute(int x, int y) override;
    std::string getName() const override;

    bool sendData(const std::vector<uint8_t>& data) override;
    double calculateLoad(float factor, int units) override;

    void log(const std::string& message) override;
    void log(const std::string& message, int severity) override;

    bool isAvailable() const override;
};

#endif // MULTISERVICE_H