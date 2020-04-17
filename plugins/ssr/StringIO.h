#pragma once
#include <string>

class StringWriter {
public:
    explicit StringWriter(std::string &str) : _str(str) {}

    void encodeHex(const uint8_t *byte, uint32_t count);
    void encodeU32(uint32_t value);
    void encodeF32(float value);

private:
    std::string &_str;
};

class StringReader {
public:
    explicit StringReader(const char *str) : _base(str), _index(0) {}

    bool decodeHex(uint8_t *byte, uint32_t count);
    bool decodeU32(uint32_t &value);
    bool decodeF32(float &value);

private:
    const char *_base = nullptr;
    size_t _index = 0;
};
