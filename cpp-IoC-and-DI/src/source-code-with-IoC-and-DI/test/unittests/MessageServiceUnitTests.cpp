#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "MessageService.h"
#include "MockMessageSender.h"

using ::testing::StrictMock;
using ::testing::NiceMock;
using ::testing::Exactly;
using ::testing::StrEq;

TEST(MessageServiceTest, ShouldNotSendMessageWhenSenderIsNull) 
{
    // Arrange
    auto mockSender = std::make_unique<StrictMock<MockMessageSender>>();
    std::string testMessage = "Should not crash";

    // Expect
    EXPECT_CALL(*mockSender, send(StrEq(testMessage))).Times(Exactly(0));

    // Act
    MessageService service(nullptr);
    service.sendMessage(testMessage); // Do not expect crash

    // Assert

    // After
}

TEST(MessageServiceTest, ShouldSendMessageWhenSenderIsValid) 
{
    // Arrange
    auto mockSender = std::make_unique<StrictMock<MockMessageSender>>();
    std::string testMessage = "Hello";

    // Expect: se espera que send() se llame con el mensaje correcto
    EXPECT_CALL(*mockSender, send(StrEq(testMessage))).Times(Exactly(1));

    // Act
    MessageService service(std::move(mockSender));
    service.sendMessage(testMessage);

    // Assert

    // After
}