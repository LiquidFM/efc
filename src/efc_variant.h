/** @file efc_variant.h
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

#ifndef EFC_VARIANT_H_
#define EFC_VARIANT_H_

#include <cstdint>
#include <cstddef>
#include <efc/SharedPointer>


namespace EFC {

class Variant
{
public:
    enum Type
    {
        UINT8,
        INT8,
        UINT16,
        INT16,
        UINT32,
        INT32,
        UINT64,
        INT64,
        FLOAT,
        DOUBLE,
        BOOL,
        CHAR,
        STRING,
        BINARY
    };

public:
    Variant();
    Variant(uint8_t value);
    Variant(int8_t value);
    Variant(uint16_t value);
    Variant(int16_t value);
    Variant(uint32_t value);
    Variant(int32_t value);
    Variant(uint64_t value);
    Variant(int64_t value);
    Variant(float value);
    Variant(double value);
    Variant(bool value);
    Variant(char value);
    Variant(const char *value);
    Variant(const unsigned char *value, size_t size);
    ~Variant();

    bool isValid() const { return m_data.get() != NULL; }

    Type type() const;
    size_t size() const;

    template <typename T>
    inline void set(T value);

    template <typename T>
    inline T get();

    uint8_t asUint8() const;
    void setUint8(uint8_t value);

    int8_t asInt8() const;
    void setInt8(int8_t value);

    uint16_t asUint16() const;
    void setUint16(uint16_t value);

    int16_t asInt16() const;
    void setInt16(int16_t value);

    uint32_t asUint32() const;
    void setUint32(uint32_t value);

    int32_t asInt32() const;
    void setInt32(int32_t value);

    uint64_t asUint64() const;
    void setUint64(uint64_t value);

    int64_t asInt64() const;
    void setInt64(int64_t value);

    float asFloat() const;
    void setFloat(float value);

    double asDouble() const;
    void setDouble(double value);

    bool asBool() const;
    void setBool(bool value);

    char asChar() const;
    void setChar(char value);

    const char *asString() const;
    void setString(const char *value);

    const unsigned char *asBinary(size_t &size) const;
    void setBinary(const unsigned char *value, size_t size);

private:
    struct Private;
    SharedPointer<Private> m_data;
};


template <> inline void Variant::set(uint8_t value) { setUint8(value); }
template <> inline void Variant::set(int8_t value) { setInt8(value); }
template <> inline void Variant::set(uint16_t value) { setUint16(value); }
template <> inline void Variant::set(int16_t value) { setInt16(value); }
template <> inline void Variant::set(uint32_t value) { setUint32(value); }
template <> inline void Variant::set(int32_t value) { setInt32(value); }
template <> inline void Variant::set(uint64_t value) { setUint64(value); }
template <> inline void Variant::set(int64_t value) { setInt64(value); }
template <> inline void Variant::set(float value) { setFloat(value); }
template <> inline void Variant::set(double value) { setDouble(value); }
template <> inline void Variant::set(bool value) { setBool(value); }
template <> inline void Variant::set(char value) { setChar(value); }
template <> inline void Variant::set(const char *value) { setString(value); }

template <> inline uint8_t Variant::get<uint8_t>() { return asUint8(); }
template <> inline int8_t Variant::get<int8_t>() { return asInt8(); }
template <> inline uint16_t Variant::get<uint16_t>() { return asUint16(); }
template <> inline int16_t Variant::get<int16_t>() { return asInt16(); }
template <> inline uint32_t Variant::get<uint32_t>() { return asUint32(); }
template <> inline int32_t Variant::get<int32_t>() { return asInt32(); }
template <> inline uint64_t Variant::get<uint64_t>() { return asUint64(); }
template <> inline int64_t Variant::get<int64_t>() { return asInt64(); }
template <> inline float Variant::get<float>() { return asFloat(); }
template <> inline double Variant::get<double>() { return asDouble(); }
template <> inline bool Variant::get<bool>() { return asBool(); }
template <> inline char Variant::get<char>() { return asChar(); }
template <> inline const char *Variant::get<const char *>() { return asString(); }

}

#endif /* EFC_VARIANT_H_ */
