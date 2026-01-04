/**
 ********************************************************************************
 * @file        IStream.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, Interface for Stream functionality.
 * 
 * @author      toberg
 * 
 * @date        2025/10/28
********************************************************************************/

#pragma once

#include <string>
#include <cstdint>
#include "SysTypes.hpp"


namespace Utils 
{


    
/**
* @brief This interface class provides access to a stream object.
*
* - - -
*
* __Thread safety:__
* The access to this class is not safe.
*
* - - -
*
*
*/ 
class IStream
{
    public:

        /// @brief Length of a Stream header in bytes 
        static constexpr uint8_t HEADERLENGTH{8U};   
        
        /// @brief Maximum Length of Stream data in bytes 
        static constexpr uint16_t MAXDATALENGTH{1024U};   

        /// @brief Maximum length of a Stream in bytes include header 
        static constexpr uint32_t MAXSTREAMLENGTH{HEADERLENGTH + MAXDATALENGTH};   


        /// @brief Types of Streams
        enum class StreamType : uint8_t
        {
            GENERIC = 0U,      //!< byte based stream
            TEXT    = 1U       //!< char based stream
        };

        /// @brief Byteorders of Streams
        enum class ByteOrder : uint8_t
        {
            LENDIAN     = 0U,      //!< bytes ordered little endian, on first address stays the lsb, this is default on arm systems.
            BENDIAN     = 1U       //!< bytes ordered big endian, on first address stays the msb.
        };

        /// @brief Stream Header
        struct StreamHeader
        {
            StreamType  type{StreamType::GENERIC};          //!< Type of the Stream
            ByteOrder   endianness{ByteOrder::LENDIAN};     //!< Endianness of the Stream data
            uint16_t    flags{0u};                          //!< flags of the stream object
            uint16_t    length{0u};                         //!< Length of the stream data in bytes
            uint16_t    crc{0u};                            //!< 16 bit CRC of the stream data arround the length
        };
        
        /**
        * @fn IStream::GetHeader
        * @brief This method provides the header of a stream object.\n
        * - - - 
        * 
        * __Thread safety:__ 
        * The access to this object is not thread safe.
        * 
        * - - -
        * @return StreamHeader of the stream object.
        * 
        */ 
        virtual IStream::StreamHeader GetHeader() const = 0; 
        
        /**
        * @fn IStream::GetData
        * @brief This method provides a pointer to the first data byte.\n
        * - - - 
        * 
        * __Thread safety:__ 
        * The access to this object is not thread safe.
        * 
        * - - -
        * @return Ptr to stream data.
        * 
        */ 
        virtual IStream::StreamHeader GetData() const = 0; 
    //    /**
    //    * @brief This Method creates and get back the ownership of a serialized stream object on heap.\n
    //    * @verbatim
    //    * In general, the serialized stream header is stored on heap in littleEndian format: \n
    //    * stream header[4] = | type[1] | endianness[1] | len[0..1] | 
    //    *
    //    * The Data of stream is stored in order to @ref StreamType and @ref ByteOrder: \n
    //    * @endverbatim
    //    * - - - 
    //    * 
    //    * __Thread safety:__ 
    //    * The access to this object is thread safe.
    //    * 
    //    * - - - 
    //    * 
    //    * @param type Enum of @ref StreamType 
    //    * @param endianness Endianness of the data (0 = little endian = default, 1 = big endian)
    //    * @param data Pointer to the BaseStream data
    //    * @param len Length of the Stream data in bytes
    //    * 
    //    * @return Unique pointer to the created stream object on heap (moves the ownership), nullptr if an error occurred.
    //    */
    //    static std::unique_ptr<uint8_t[]> CreateObj(StreamType const type, ByteOrder const endianness, uint8_t* const data, uint16_t const len)
    //    {
    //        if ((data != nullptr) && (len > 0) && (len < MAXDATALENGTH))
    //        {
    //            std::unique_ptr<uint8_t[]> mData{ std::make_unique<uint8_t[]>(len + HEADERLENGTH) };
//
    //            if (mData != nullptr)
    //            {
    //                // first four bytes contains the header of the payload 
    //                mData[0] = static_cast<uint8_t>(type);
    //                mData[1] = static_cast<uint8_t>(endianness);
    //                mData[2] = static_cast<uint8_t>((len >> 8) & 0xFFU);
    //                mData[4] = static_cast<uint8_t>(len & 0xFFU);
    //                // copy the BaseStream
    //                for (uint32_t i = 0; i < len; ++i)
    //                {
    //                    mData[i + HEADERLENGTH] = data[i];
    //                }
    //                return mData;
    //            }
    //            else
    //            {
    //                //this should never happen except the heap is exhausted 
    //            }
    //            
    //        }
    //        return nullptr;
    //    };
//
//
    //    /**
    //    * @brief This Method provides data and size of a referenced BaseStream object.
    //    * 
    //    * - - - 
    //    * 
    //    * __Thread safety:__
    //    * The access to this class is thread safe.
    //    * 
    //    * - - - 
    //    * 
    //    * @param pData Unique_ptr to the refernced BaseStream object
    //    * 
    //    * @return std::span of a referenced BaseStream object.
    //    */
    //    static std::span<uint8_t> GetObjData(std::unique_ptr<uint8_t[]>& pData) 
    //    {
    //        if (pData != nullptr)
    //        {
    //            uint16_t len{ static_cast<uint16_t>(pData[2]) << 8);
    //            len |= static_cast<uint16_t>(pData[3]);
    //            if (len > MAXDATALENGTH)
    //            {
    //                len = MAXDATALENGTH;
    //            }
    //            return std::span<uint8_t>(pData.get() + HEADERLENGTH, len);
    //        }
    //        return {};  // empty span
    //    };
//
//
    protected:    

        /**
        * @brief Default Constructor.
        */ 
        IStream() = default;
        IStream(IStream const&) = delete;              //!< none copy constructor
        IStream(IStream&&) = delete;                   //!< none move constructor
        
        /**
        * @brief Default Destructor.
        */ 
        ~IStream() = default;
        IStream& operator=(IStream const&) = delete;   //!< none copy operator
        IStream& operator=(IStream&&) = delete;        //!< none move operator
            
};


} // end namespace Utils
