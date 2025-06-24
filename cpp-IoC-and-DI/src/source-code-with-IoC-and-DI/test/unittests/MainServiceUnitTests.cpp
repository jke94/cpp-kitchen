#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "MainService.h"
#include "MockMultiService.h"

using ::testing::StrictMock;
using ::testing::NiceMock;
using ::testing::Exactly;

TEST(MainServiceTest, ShouldReturn1994WhenInputsAreEqual) 
{
    // Arrange
    auto mockMultiService = std::make_shared<StrictMock<MockMultiService>>();
    MainService service(mockMultiService);

    // Expect: se espera que compute() no se llame
    EXPECT_CALL(*mockMultiService, compute(testing::_, testing::_))
        .Times(Exactly(0));

    // Act
    int result = service.computeNumber(5, -5); // Inputs are equal in absolute value

    // Assert
    EXPECT_EQ(result, 1994);
}

TEST(MainServiceTest, ShouldCallMultiServiceComputeWhenInputsAreNotEqual) 
{
    // Arrange
    auto mockMultiService = std::make_shared<StrictMock<MockMultiService>>();
    MainService service(mockMultiService);

    // Expect: se espera que compute() se llame con los valores correctos
    EXPECT_CALL(*mockMultiService, compute(3, 4))
        .Times(Exactly(1))
        .WillOnce(testing::Return(7));

    // Act
    int result = service.computeNumber(3, 4); // Inputs are not equal in absolute value

    // Assert
    EXPECT_EQ(result, 7);
}

TEST(MainServiceTest, ShouldCallMultiServiceComputeWithNegativeInputs) 
{
    // Arrange
    auto mockMultiService = std::make_shared<StrictMock<MockMultiService>>();
    MainService service(mockMultiService);

    // Expect: se espera que compute() se llame con los valores correctos
    EXPECT_CALL(*mockMultiService, compute(-2, -3))
        .Times(Exactly(1))
        .WillOnce(testing::Return(-5));

    // Act
    int result = service.computeNumber(-2, -3); // Inputs are not equal in absolute value

    // Assert
    EXPECT_EQ(result, -5);
}

TEST(MainServiceTest, ShouldCallMultiServiceComputeWithMixedInputs) 
{
    // Arrange
    auto mockMultiService = std::make_shared<StrictMock<MockMultiService>>();
    MainService service(mockMultiService);

    // Expect: se espera que compute() se llame con los valores correctos
    EXPECT_CALL(*mockMultiService, compute(10, -5))
        .Times(Exactly(1))
        .WillOnce(testing::Return(5));

    // Act
    int result = service.computeNumber(10, -5); // Inputs are not equal in absolute value

    // Assert
    EXPECT_EQ(result, 5);
}