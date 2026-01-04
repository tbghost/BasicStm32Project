/**
 ********************************************************************************
 * @file        BaseStream.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, BaseStream functionality.
 * 
 * @author      toberg
 * 
 * @date        2025/10/28
********************************************************************************/

#pragma once

#include <IStream.hpp>
#include "SysTypes.hpp"
#include <memory>
#include <span>

namespace Utils 
{

    
/**
* @brief This class provides a base stream object.
*
* - - -
*
* __Thread safety:__
* The access to this class is not safe.
*
* - - -
*
*
*/ class BaseStream final : public IStream
{
    public: 

        /**
        * @fn BaseStream::BaseStream
        * @brief This Constructor creates a BaseStream object.\n
        * @verbatim
        * In general, the serialized stream header is stored on heap in littleEndian format: \n
        * stream header[4] = | type[1] | endianness[1] | len[0..1] | 
        *
        * The Data of stream is stored in order to @ref StreamType and @ref ByteOrder: \n
        * @endverbatim
        * - - - 
        * 
        * __Thread safety:__ 
        * The access to this object is not thread safe.
        * 
        * - - - 
        * 
        * @param type Enum of @ref StreamType 
        * @param endianness Endianness of the data (0 = little endian = default, 1 = big endian)
        * @param data Pointer to the raw data
        * @param len Length of the data in bytes
        * 
        */
        BaseStream(IStream::StreamType const type, IStream::ByteOrder const endianness, uint8_t* const data, uint16_t const len); 
       
        BaseStream(BaseStream const&) = delete;              //!< none copy constructor
        BaseStream(BaseStream&&) = delete;                   //!< none move constructor
        
        /**
        * @brief Default Destructor.
        */ 
        ~BaseStream() = default;

        BaseStream& operator=(BaseStream const&) = delete;   //!< none copy operator
        BaseStream& operator=(BaseStream&&) = delete;        //!< none move operator
        
        /// @copydoc IStream::GetHeader
        IStream::StreamHeader GetHeader() const override; 

        /// @copydoc IStream::GetData
        IStream::StreamHeader GetData() const override; 


    private:

        std::unique_ptr<uint8_t[]> mData{ nullptr };
        
        
};



} // end namespace Utils
