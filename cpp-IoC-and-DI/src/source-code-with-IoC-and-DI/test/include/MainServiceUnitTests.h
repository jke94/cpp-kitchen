#ifndef MAIN_SERVICE_TEST_H
#define MAIN_SERVICE_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "MainService.h"
#include "MockMultiService.h"

namespace UnitTestsSourceCodeWithIoCAndDI
{
    class MainServiceTest : public ::testing::Test 
    {
    protected:
        std::shared_ptr<::testing::StrictMock<MockMultiService>> mockMultiService;
        std::unique_ptr<MainService> service;

        void SetUp() override;

        void TearDown() override;
    };

} // namespace UnitTestsSourceCodeWithIoCAndDI

#endif // MAIN_SERVICE_TEST_H