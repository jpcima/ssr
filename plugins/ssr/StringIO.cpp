#include "StringIO.h"

void StringWriter::encodeHex(const uint8_t *byte, uint32_t count)
{
    std::string &str = _str;
    const char alphabet[] = "0123456789ABCDEF";
    for (uint32_t i = 0; i < count; ++i) {
        str.push_back(alphabet[byte[i] >> 4]);
        str.push_back(alphabet[byte[i] & 0xF]);
    }
}

void StringWriter::encodeU32(uint32_t value)
{
    uint8_t byte[4];
    byte[0] = (value      ) & 0xFF;
    byte[1] = (value >>  8) & 0xFF;
    byte[2] = (value >> 16) & 0xFF;
    byte[3] = (value >> 24);
    encodeHex(byte, sizeof(byte));
}

void StringWriter::encodeF32(float value)
{
    union { uint32_t i; float f; } u;
    u.f = value;
    encodeU32(u.i);
}

bool StringReader::decodeHex(uint8_t *byte, uint32_t count)
{
    const char *str = _base + _index;

    for (uint32_t i = 0; i < count; ++i) {
        if (!*str) return false;
        char digit1 = *str++;
        if (!*str) return false;
        char digit2 = *str++;
        auto digitValue = [](char d) -> int {
            if (d >= '0' && d <= '9') return d - '0';
            if (d >= 'A' && d <= 'F') return d - 'A' + 10;
            if (d >= 'a' && d <= 'f') return d - 'a' + 10;
            return -1;
        };
        int val1 = digitValue(digit1);
        int val2 = digitValue(digit2);
        if (val1 == -1 || val2 == -1)
            return false;
        byte[i] = (val1 << 4) | val2;
    }

    _index = str - _base;
    return true;
}

bool StringReader::decodeU32(uint32_t &value)
{
    uint8_t byte[4];
    if (!decodeHex(byte, sizeof(byte)))
        return false;
    value = byte[0] | (byte[1] << 8) | (byte[2] << 16) | (byte[3] << 24);
    return true;
}

bool StringReader::decodeF32(float &value)
{
    union { uint32_t i; float f; } u;
    if (!decodeU32(u.i))
        return false;
    value = u.f;
    return true;
}
