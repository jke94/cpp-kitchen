#ifndef MOCK_MULTI_SERVICE_H
#define MOCK_MULTI_SERVICE_H

#include <gmock/gmock.h>

#include "IMultiService.h"

#include <string>

class MockMultiService : public IMultiService 
{
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));

    MOCK_METHOD(int, compute, (int x, int y), (override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(bool, sendData, (const std::vector<uint8_t>& data), (override));
    MOCK_METHOD(double, calculateLoad, (float factor, int units), (override));

    // Sobrecargas: GMock requiere casting o "using"
    MOCK_METHOD(void, log, (const std::string& message), (override));
    MOCK_METHOD(void, log, (const std::string& message, int severity), (override));

    MOCK_METHOD(bool, isAvailable, (), (const, override));
};

#endif // MOCK_MULTI_SERVICE_H
