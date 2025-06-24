#ifndef MOCK_MESSAGE_SENDER_H
#define MOCK_MESSAGE_SENDER_H

#include <gmock/gmock.h>

#include "IMessageSender.h"

namespace UnitTestsSourceCodeWithIoCAndDI
{
    class MockMessageSender : public IMessageSender 
    {
    public:
        MOCK_METHOD(void, send, (std::string message), (override));
    };

} // namespace UnitTestsSourceCodeWithIoCAndDI

#endif // MOCK_MESSAGE_SENDER_H