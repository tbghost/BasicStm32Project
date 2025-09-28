#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../IpAddressV4.hpp"
#include <cstring>
#include <memory>

using ::testing::_; // NOSONAR gtest wild card parameter
using namespace Utils;

namespace GTest {


/**
*   TestList
*   ========
* 
*   Explanation
*   -----------
*   (0) Done.\n             
*   (1) Masked by code inspection, no test necessary.\n
*   (2) Operation deleted, no test necessary.\n
*   (3) Disabled because th test has been failed. To-do: find and fix the bug.\n
*----------------------------------------------------------------------------------------- 
*   (0)  InstantiateByValue
*   (0)  InstantiateByString
*   (0)  InstantiateByBadString
*   (0)  InstantiateByBadString2
*   (0)  TestMoveAssignment
*   (0)  TestEqualOperator
*/

//################################### Tests start here #######################################


TEST(IpAddressV4_Test,InstantiateByValue)
{
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(0x00000000U);

    ASSERT_NE(spUUT, nullptr);  
    ASSERT_EQ(spUUT->GetValue(), 0x00000000U);
}

TEST(IpAddressV4_Test, InstantiateByString)
{
    const std::string_view ipStr{"192.168.178.193"};  //NOSONAR because the hard coded IP address is save
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(ipStr.data());

    ASSERT_NE(spUUT, nullptr);  
    ASSERT_EQ(spUUT->GetValue(), 0xC0A8B2C1U);
}

TEST(IpAddressV4_Test, InstantiateByBadString)
{
    const std::string_view ipStr{"192.bad.178.193"};  
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(ipStr.data());

    ASSERT_NE(spUUT, nullptr);
    ASSERT_EQ(spUUT->GetValue(), 0x00000000U);
}

TEST(IpAddressV4_Test, InstantiateByBadString2)
{
    const std::string_view ipStr{"192168.178.193"};  
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(ipStr.data());

    ASSERT_NE(spUUT, nullptr);
    ASSERT_EQ(spUUT->GetValue(), 0x00000000U);
}

TEST(IpAddressV4_Test, TestMoveAssignment)
{
    std::unique_ptr<Utils::IpAddressV4> spIpDefault = std::make_unique<Utils::IpAddressV4>(0xC0A8B2C1U);
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(0x00000000U);

    ASSERT_NE(spUUT, nullptr);  
    ASSERT_EQ(spUUT->GetValue(), 0x00000000U);

    spUUT = std::move(spIpDefault);
    ASSERT_EQ(spUUT->GetValue(), 0xC0A8B2C1U);
}

TEST(IpAddressV4_Test, TestEqualOperator)
{
    std::unique_ptr<Utils::IpAddressV4> spIpDefault = std::make_unique<Utils::IpAddressV4>(0xC0A8B2C1U);
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(0xC0A8B2C1U);
    
    bool compare = (*spUUT == *spIpDefault);

    EXPECT_TRUE(compare);
}

TEST(IpAddressV4_Test, TestGetString)
{
    const std::string_view ipStr{"192.168.178.193"};  //NOSONAR because the hard coded IP address is save
    std::unique_ptr<Utils::IpAddressV4> spUUT = std::make_unique<Utils::IpAddressV4>(ipStr.data());
    ASSERT_NE(spUUT, nullptr);
    std::string cmpStr = spUUT->GetStr();
    ASSERT_EQ(cmpStr.compare(ipStr),0);
}


}  // end namespace GTest

