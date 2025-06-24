#include "MainServiceUnitTests.h"

using ::testing::Exactly;

namespace UnitTestsSourceCodeWithIoCAndDI
{

    void MainServiceTest::SetUp() 
    {
        mockMultiService = std::make_shared<::testing::StrictMock<MockMultiService>>();
        service = std::make_unique<MainService>(mockMultiService);
    }

    void MainServiceTest::TearDown() 
    {
        // Automatically handled due to smart pointers.
    }

    TEST_F(MainServiceTest, ShouldReturn1994WhenInputsAreEqual) 
    {
        // Arrange
        // (done in SetUp)

        // Expect
        EXPECT_CALL(*mockMultiService, compute(::testing::_, ::testing::_))
            .Times(Exactly(0));

        // Act
        int result = service->computeNumber(5, -5);

        // Assert
        EXPECT_EQ(result, 1994);
    }

    TEST_F(MainServiceTest, ShouldCallMultiServiceComputeWhenInputsAreNotEqual) 
    {
        // Arrange
        // (done in SetUp)

        // Expect
        EXPECT_CALL(*mockMultiService, compute(3, 4))
            .Times(Exactly(1))
            .WillOnce(::testing::Return(7));

        // Act
        int result = service->computeNumber(3, 4);

        // Assert
        EXPECT_EQ(result, 7);
    }

    TEST_F(MainServiceTest, ShouldCallMultiServiceComputeWithNegativeInputs) 
    {
        // Arrange
        // (done in SetUp)

        // Expect
        EXPECT_CALL(*mockMultiService, compute(-2, -3))
            .Times(Exactly(1))
            .WillOnce(::testing::Return(-5));

        // Act
        int result = service->computeNumber(-2, -3);

        // Assert
        EXPECT_EQ(result, -5);
    }

    TEST_F(MainServiceTest, ShouldCallMultiServiceComputeWithMixedInputs) 
    {
        // Arrange
        // (done in SetUp)

        // Expect
        EXPECT_CALL(*mockMultiService, compute(10, -5))
            .Times(Exactly(1))
            .WillOnce(::testing::Return(5));

        // Act
        int result = service->computeNumber(10, -5);

        // Assert
        EXPECT_EQ(result, 5);
    }

} // namespace UnitTestsSourceCodeWithIoCAndDI