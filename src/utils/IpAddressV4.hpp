/**
 ********************************************************************************
 * @file        Utils.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils.
 * 
 * @author      toberg
 * 
 * @date        2024/12/07
********************************************************************************/

#pragma once

#include <string>
#include <cstdint>
namespace Utils {


/**
 * @brief   This class provides a IPV4 address on different formats.
 * 
 * - - - 
 * 
 * __Thread safety:__
 * The access to this class is not thread safe.\n
 * It's in users reponsibility to make sure a clean access in one context.     
 *
 */
class IpAddressV4
{
    public:
            
        /**
        * @brief This member stays for the count of bytes, expected by a valid ipvalue.
        */
        static constexpr uint32_t bytesExpected{4U};

        /**
         * @brief   Constructs a IpAddressV4 object from a 32bit value.
         *           
         * @param   value       uint32_t value of ipv4.
         */
        explicit IpAddressV4(uint32_t ipvalue)
        : mIpValue(ipvalue)
        , mIpString(ValueToStr(mIpValue))
        {};

        /**
         * @brief   Constructs a IpAddressV4 object from a string.
         *           
         * @param   ipStr       String of Ip v4.
         */
        explicit IpAddressV4(char const* ipStr)
        : mIpValue(IpStrDecToValue(ipStr))
        , mIpString(ValueToStr(mIpValue))
        {};

        /**
         * @brief   Copy Constructor IpAddressV4 object from another IpAddressV4 object.
         *           
         * @param   other       Another IpAddressV4 object.
         */
        IpAddressV4(const IpAddressV4& other) = default;
       
        /**
         * @brief   Destructor
         */
        ~IpAddressV4()=default;

        /**
         * @brief Operator equal for comparization of two IpAddressV4 objects.
         * 
         * @return true if equal, otherwise false.
         */  
        friend bool operator == (const IpAddressV4& lhs, const IpAddressV4& rhs ) 
        {
            return (lhs.mIpValue == rhs.mIpValue);
        }; 

        /**
         * @brief   Move assignment operator. The contents of other object moved to this object 
         *           
         * @param   other       another oject of the same type
         */
        IpAddressV4& operator=(IpAddressV4 &&other) noexcept
        {
            if (&other != this)
            {
                mIpValue = other.mIpValue;
                mIpString = ValueToStr(mIpValue);
            }
            return *this;
        }        

        /**
        * @brief  Provides the actual Ip as a 32bit value. 
        * For example: The 32bit equivalent of IP-valuestring "192.168.178.193" is 0xC0A8B2C1.
        * 
        *
        * @return  Ip value  
        */
        uint32_t GetValue() const {return mIpValue;};

        /**
        * @brief  Provides the actual Ip value in string format: "192.168.178.193".
        * 
        *
        * @return  Ip string  
        */
        std::string GetStr() const {return mIpString;};

    private:
        
        /**
         * @brief  Convert a IP value string, which contains decimal digits like is format: "192.168.178.193",
         *         to a 32 bit value.
         *
         * @param   pChar pointer to a zero terminated char array
         *  
         * @return  32 bit value, which contains the IP in hexadecimal format.  
         */
        uint32_t IpStrDecToValue (const char* pChar);


        /**
         * @brief  Convert a Ip value to a string, which contains decimal digits like is format: "192.168.178.1".
         *
         * @param   value  uint32 
         *  
         * @return  std::string object.  
         */
        std::string ValueToStr(uint32_t value);

        /// @brief ipvalue
        uint32_t mIpValue{0U};

        /// @brief ipvalue string
        std::string mIpString{};
 
};

}  // namespace Utils
