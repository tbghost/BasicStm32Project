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

#include "IpAddressV4.hpp"

using namespace Utils;


std::string IpAddressV4::ValueToStr(uint32_t value)
{
    char ipStr[16];
    uint32_t ipValue = value;
    unsigned int c1 = static_cast<unsigned int>(((ipValue >> 24u) & 0xFFu));  // NOSONAR
    unsigned int c2 = static_cast<unsigned int>(((ipValue >> 16u) & 0xFFu));    // NOSONAR
    unsigned int c3 = static_cast<unsigned int>(((ipValue >> 8u) & 0xFFu)); // NOSONAR
    unsigned int c4 = static_cast<unsigned int>((ipValue & 0xFFu)); // NOSONAR
    snprintf(ipStr, sizeof(ipStr), "%u.%u.%u.%u", c1,c2,c3,c4 ); 
    return std::string(ipStr);
}


uint32_t IpAddressV4::IpStrDecToValue (const char* pChar)
{
    uint32_t ipValue{ 0U };

    if (pChar != nullptr)
    {
        uint8_t byteCnt{ 0u };                  // count of valid bytes

        for (uint32_t i=0;i<bytesExpected;i++)
        {
            uint32_t digits{ 0u };
            uint8_t queuedChars{ 0u };  // queued good chars  

            while (*pChar != '\0')
            {
                if ((*pChar >= '0') && (*pChar <= '9'))
                {
                    // a good char has been detect
                    digits = (digits << 4u) | (static_cast<uint32_t>(*pChar) - 0x30u);    
                    queuedChars++;              // queued good chars increase
                }
                else 
                {
                    // a delimiter or a bad char has been detect
                    // If there was min 1 queued good char before ...   
                    if (queuedChars > 0)
                    {
                        break;        
                    }    
                }
                pChar++;
            }

            // if the value between delimiters is between 0...255 
            if ((queuedChars > 0u) && (queuedChars < 4u))
            {
                // this performs a valid byte
                byteCnt++;  
                uint8_t byte{ 0u };
                byte = static_cast<uint8_t>(((digits >> 8) & 0x0F) * 100u);
                byte = byte + static_cast<uint8_t>(((digits >> 4) & 0x0F) * 10u);
                byte = byte + static_cast<uint8_t>(digits & 0x0F);
                ipValue = (ipValue << 8) | byte;
            }
        }

        // For a valid ipvalue, exactly 4 bytes must been detected
        if (byteCnt != bytesExpected)
        {
            ipValue = 0U;
        }
    }
    return ipValue;
}

