/** @file efc_variant.cpp
 *  @brief TODO: Put description here.
 *
 * TODO: Put description here.
 *
 * @copyright
 * This file is part of the EFC library.
 * @n@n
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * @n@n
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * @n@n
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * @n@n
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "efc_variant.h"

#include <efc/network/byteorder.h>
#include <brolly/assert.h>
#include <cstring>

#if PLATFORM_OS(UNIX)
#include <netinet/in.h>
#elif PLATFORM_OS(WINDOWS)
#include <winsock.h>
#endif


namespace {

#if PLATFORM_CPU(LITTLE_ENDIAN)
enum
{
    LoByte = 0,
    LoWord = 0,
    LoDWord = 0,
    HiByte = sizeof(uint64_t) - 1,
    HiWord = sizeof(uint64_t) / sizeof(uint16_t) - 1,
    HiDWord = sizeof(uint64_t) / sizeof(uint32_t) - 1
};
#else
#error This platform is not supported
#endif
}


namespace EFC {

typedef unsigned char byte_t;

struct Variant::Private
{
    Private(uint8_t value) :
        type(UINT8),
        data()
    { data.uint_value.uint8_value[LoByte] = value; }

    Private(int8_t value) :
        type(INT8),
        data()
    { data.int_value.int8_value[LoByte] = value; }

    Private(uint16_t value) :
        type(UINT16),
        data()
    { data.uint_value.uint16_value[LoWord] = value; }

    Private(int16_t value) :
        type(INT16),
        data()
    { data.int_value.int16_value[LoWord] = value; }

    Private(uint32_t value) :
        type(UINT32),
        data()
    { data.uint_value.uint32_value[LoDWord] = value; }

    Private(int32_t value) :
        type(INT32),
        data()
    { data.int_value.int32_value[LoDWord] = value; }

    Private(uint64_t value) :
        type(UINT64),
        data()
    { data.uint_value.uint64_value = value; }

    Private(int64_t value) :
        type(INT64),
        data()
    { data.int_value.int64_value = value; }

    Private(float value) :
        type(FLOAT),
        data()
    { data.float_value = value; }

    Private(double value) :
        type(DOUBLE),
        data()
    { data.double_value = value; }

    Private(bool value) :
        type(BOOL),
        data()
    { data.int_value.int8_value[LoByte] = value; }

    Private(char value) :
        type(CHAR),
        data()
    { data.int_value.int8_value[LoByte] = value; }

    Private(const char *value, size_t size) :
        type(STRING),
        data()
    {
        ::memcpy(data.string_value.value, value, size);
        data.string_value.size = size;
    }

    Private(const char *value, size_t size, bool) :
        type(STRING),
        data()
    {
        ::memcpy(data.string_value.value, value, size);
        data.string_value.value[size] = 0;
        data.string_value.size = size + 1;
    }

    Private(const unsigned char *value, size_t size) :
        type(BINARY),
        data()
    {
        ::memcpy(data.string_value.value, value, size);
        data.string_value.size = size;
    }

    struct StringData
    {
        size_t size;
        char value[1];
    };

    Type type;
    union UIntData
    {
        uint64_t uint64_value;
        uint32_t uint32_value[HiDWord + 1];
        uint16_t uint16_value[HiWord + 1];
        uint8_t uint8_value[HiByte + 1];
    };
    union IntData
    {
        int64_t int64_value;
        int32_t int32_value[HiDWord + 1];
        int16_t int16_value[HiWord + 1];
        int8_t int8_value[HiByte + 1];
    };
    union Data
    {
        UIntData   uint_value;
        IntData    int_value;
        float      float_value;
        double     double_value;
        StringData string_value;
    } data;
};

Variant::Variant()
{}

Variant::Variant(uint8_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(int8_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(uint16_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(int16_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(uint32_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(int32_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(uint64_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(int64_t value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(float value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(double value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(bool value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(char value)
{
    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private)])
        m_data.reset(new (buffer) Private(value));
}

Variant::Variant(const char *value)
{
    ASSERT(value);
    size_t len = ::strlen(value);

    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private) + len])
        m_data.reset(new (buffer) Private(value, len + 1));
}

Variant::Variant(const unsigned char *value, size_t size)
{
    ASSERT(value && size);

    if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private) + size - 1])
        m_data.reset(new (buffer) Private(value, size));
}

Variant::~Variant()
{}

Variant::Type Variant::type() const
{
    ASSERT(isValid());
    return m_data->type;
}

size_t Variant::size() const
{
    ASSERT(isValid());

    switch (m_data->type)
    {
        case UINT8:
            return sizeof(uint8_t);

        case INT8:
            return sizeof(int8_t);

        case UINT16:
            return sizeof(uint16_t);

        case INT16:
            return sizeof(int16_t);

        case UINT32:
            return sizeof(uint32_t);

        case INT32:
            return sizeof(int32_t);

        case UINT64:
            return sizeof(uint64_t);

        case INT64:
            return sizeof(int64_t);

        case FLOAT:
            return sizeof(float);

        case DOUBLE:
            return sizeof(double);

        case BOOL:
            return sizeof(bool);

        case CHAR:
            return sizeof(char);

        case STRING:
            return m_data->data.string_value.size - 1;

        case BINARY:
            return m_data->data.string_value.size;
    }

    return 0;
}

uint8_t Variant::asUint8() const
{
    ASSERT(isValid());
    return m_data->data.uint_value.uint8_value[LoByte];
}

void Variant::setUint8(uint8_t value)
{
    (*this) = Variant(value);
}

int8_t Variant::asInt8() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int8_value[LoByte];
}

void Variant::setInt8(int8_t value)
{
    (*this) = Variant(value);
}

uint16_t Variant::asUint16() const
{
    ASSERT(isValid());
    return m_data->data.uint_value.uint16_value[LoWord];
}

void Variant::setUint16(uint16_t value)
{
    (*this) = Variant(value);
}

int16_t Variant::asInt16() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int16_value[LoWord];
}

void Variant::setInt16(int16_t value)
{
    (*this) = Variant(value);
}

uint32_t Variant::asUint32() const
{
    ASSERT(isValid());
    return m_data->data.uint_value.uint32_value[LoDWord];
}

void Variant::setUint32(uint32_t value)
{
    (*this) = Variant(value);
}

int32_t Variant::asInt32() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int32_value[LoDWord];
}

void Variant::setInt32(int32_t value)
{
    (*this) = Variant(value);
}

uint64_t Variant::asUint64() const
{
    ASSERT(isValid());
    return m_data->data.uint_value.uint64_value;
}

void Variant::setUint64(uint64_t value)
{
    (*this) = Variant(value);
}

int64_t Variant::asInt64() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int64_value;
}

void Variant::setInt64(int64_t value)
{
    (*this) = Variant(value);
}

float Variant::asFloat() const
{
    ASSERT(isValid());
    return m_data->data.float_value;
}

void Variant::setFloat(float value)
{
    (*this) = Variant(value);
}

double Variant::asDouble() const
{
    ASSERT(isValid());
    return m_data->data.double_value;
}

void Variant::setDouble(double value)
{
    (*this) = Variant(value);
}

bool Variant::asBool() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int8_value[LoByte];
}

void Variant::setBool(bool value)
{
    (*this) = Variant(value);
}

char Variant::asChar() const
{
    ASSERT(isValid());
    return m_data->data.int_value.int8_value[LoByte];
}

void Variant::setChar(char value)
{
    (*this) = Variant(value);
}

const char *Variant::asString() const
{
    ASSERT(isValid());
    return m_data->data.string_value.value;
}

void Variant::setString(const char *value)
{
    (*this) = Variant(value);
}

const unsigned char *Variant::asBinary(size_t &size) const
{
    ASSERT(isValid());
    size = m_data->data.string_value.size;
    return reinterpret_cast<const unsigned char *>(m_data->data.string_value.value);
}

void Variant::setBinary(const unsigned char *value, size_t size)
{
    (*this) = Variant(value, size);
}

size_t Variant::serialize(unsigned char *buffer, size_t size) const
{
    if (!isValid())
        return 0;
    else
    {
        size_t minBufSize = Variant::size() + 1;

        if (m_data->type == STRING || m_data->type == BINARY)
            minBufSize += sizeof(uint32_t);

        if (size >= minBufSize)
        {
            ASSERT(buffer != NULL);

            *buffer = static_cast<uint8_t>(m_data->type);
            ++buffer;

            switch (m_data->type)
            {
                case UINT8:
                case INT8:
                case BOOL:
                case CHAR:
                    *buffer = m_data->data.uint_value.uint8_value[LoByte];
                    break;

                case UINT16:
                case INT16:
                    *reinterpret_cast<uint16_t *>(buffer) = htons(m_data->data.uint_value.uint16_value[LoWord]);
                    break;

                case UINT32:
                case INT32:
                    *reinterpret_cast<uint32_t *>(buffer) = htonl(m_data->data.uint_value.uint32_value[LoDWord]);
                    break;

                case UINT64:
                case INT64:
                    *reinterpret_cast<uint64_t *>(buffer) = htonll(m_data->data.uint_value.uint64_value);
                    break;

                case FLOAT:
                    *reinterpret_cast<uint32_t *>(buffer) = htonf(m_data->data.float_value);
                    break;

                case DOUBLE:
                    *reinterpret_cast<uint64_t *>(buffer) = htond(m_data->data.double_value);
                    break;

                case STRING:
                case BINARY:
                {
                    *reinterpret_cast<uint32_t*>(buffer) = htonl(m_data->data.string_value.size);
                    ::memcpy(buffer + sizeof(uint32_t), m_data->data.string_value.value, m_data->data.string_value.size);

                    break;
                }
            }
        }

        return minBufSize;
    }
}

size_t Variant::deserialize(const unsigned char *buffer, size_t size)
{
    if (buffer != NULL && size > 0)
    {
        Variant::Type type = static_cast<Variant::Type>(*buffer);
        ++buffer;

        switch (type)
        {
            case UINT8:
            {
                typedef uint8_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setUint8(static_cast<Type>(*buffer));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case INT8:
            {
                typedef uint8_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setInt8(static_cast<Type>(*buffer));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case UINT16:
            {
                typedef uint16_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setUint16(ntohs(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case INT16:
            {
                typedef int16_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setInt16(ntohs(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case UINT32:
            {
                typedef uint32_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setUint32(ntohl(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case INT32:
            {
                typedef int32_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setInt32(ntohl(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case UINT64:
            {
                typedef uint64_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setUint64(ntohll(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case INT64:
            {
                typedef int64_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setInt64(ntohll(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case FLOAT:
            {
                typedef uint32_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setFloat(ntohf(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case DOUBLE:
            {
                typedef uint64_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setDouble(ntohd(*reinterpret_cast<const Type *>(buffer)));
                    size = sizeof(Type) + 1;
                }
                break;
            }

            case BOOL:
            {
                typedef bool Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setBool(*reinterpret_cast<const Type *>(buffer));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case CHAR:
            {
                typedef char Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    setChar(*reinterpret_cast<const Type *>(buffer));
                    size = sizeof(Type) + 1;
                }

                break;
            }

            case STRING:
            {
                typedef uint32_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    Type len = ntohl(*reinterpret_cast<const Type *>(buffer));

                    if (len > 0 && len <= ((size - 1) - sizeof(Type)))
                        setString(reinterpret_cast<const char *>(buffer + sizeof(Type)), len);

                    size = sizeof(Type) + 1 + len;
                }

                break;
            }

            case BINARY:
            {
                typedef uint32_t Type;

                if ((size - 1) < sizeof(Type))
                    return 0;
                else
                {
                    Type len = ntohl(*reinterpret_cast<const Type *>(buffer));

                    if (len > 0 && len <= ((size - 1) - sizeof(Type)))
                        setBinary(reinterpret_cast<const unsigned char *>(buffer + sizeof(Type)), len);

                    size = sizeof(Type) + 1 + len;
                }

                break;
            }
        }

        return size;
    }

    return 0;
}

void Variant::setString(const char *value, size_t size)
{
    if (value[size - 1] == 0)
    {
        if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private) + size - 1])
            m_data.reset(new (buffer) Private(value, size));
    }
    else
    {
        if (byte_t *buffer = new (std::nothrow) byte_t [sizeof(Private) + size])
            m_data.reset(new (buffer) Private(value, size, true));
    }
}

}
