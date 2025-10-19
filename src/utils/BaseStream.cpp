/**
 ********************************************************************************
 * @file        BaseStream.cpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, BaseStream implementation.
 * 
 * @author      toberg
 * 
 * @date        2025/10/30
********************************************************************************/

#include "BaseStream.hpp"
#include <cstring>

using namespace Utils;

/// \copydoc Utils::BaseStream::BaseStream
BaseStream::BaseStream(IStream::StreamType const type, IStream::ByteOrder const endianness, uint8_t* const data, uint16_t const len) 
: IStream()
{
    IStream::StreamHeader header{type, endianness, len};

    mData.reset();
    if ((data == nullptr) || (len == 0) || (len > IStream::MAXDATALENGTH))
    {
        // invalid parameters, reset to default
        header.type = IStream::StreamType::GENERIC;
        header.endianness = IStream::ByteOrder::LENDIAN;
        header.length = 0u;
    }
    else
    {
        // valid parameters, copy data
        mData = std::make_unique<uint8_t[]>(header.length + HEADERLENGTH);
        if (mData != nullptr)
        {
            uint8_t *ptr = mData.get();
            std::memcpy(ptr, &header, sizeof(header));
            ptr += sizeof(header);
            std::memcpy(ptr, data, header.length);
        }
    }
};

IStream::StreamHeader BaseStream::GetHeader() const  
{
    IStream::StreamHeader header;
    if (mData != nullptr)
    {
        std::memcpy(&header, mData.get(), sizeof(IStream::StreamHeader));
    }
    return header;
};
