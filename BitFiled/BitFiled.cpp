#include "BitField.h"
#include <stdexcept>
#include <string>

const size_t BITS_IN_WORD = sizeof(unsigned int) * 8;

BitField::BitField() {
    _data = nullptr;
    _size = 0;
}

BitField::BitField(size_t size) {
    _size = size;
    size_t words = (_size + BITS_IN_WORD - 1) / BITS_IN_WORD;
    if (words > 0) {
        _data = new unsigned int[words]();
    } else {
        _data = nullptr;
    }
}

BitField::BitField(const BitField& other) {
    _size = other._size;
    size_t words = (_size + BITS_IN_WORD - 1) / BITS_IN_WORD;

    if (words > 0) {
        _data = new unsigned int[words];
        for (size_t i = 0; i < words; i++) {
            _data[i] = other._data[i];
        }
    } else {
        _data = nullptr;
    }
}

BitField::BitField(BitField&& other) noexcept {
    _data = other._data;
    _size = other._size;
    other._data = nullptr;
    other._size = 0;
}

BitField::~BitField() {
    delete[] _data;
}

void BitField::set_bit(size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("BitField: position out of range");
    }
    _data[get_index(pos)] |= get_mask(pos);
}
void BitField::reset_bit(size_t pos) {
    if (pos - 1 >= _size) {
        throw std::out_of_range("BitField: position out of range");
    }
    _data[get_index(pos)] &= ~get_mask(pos);
}
bool BitField::get_bit(size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("BitField: position out of range");
    }
    return (_data[get_index(pos)] & get_mask(pos)) != 0;
}

size_t BitField::get_index(size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("BitField: position out of range");
    }
    return pos / BITS_IN_WORD;
}

size_t BitField::get_size() const {
    return _size;
}

unsigned int BitField::get_mask(size_t pos) const {
    return 1 << (pos % BITS_IN_WORD);
}

BitField& BitField::operator = (const BitField& other) {
    if (this == &other) {
        return *this;
    }
    BitField temp(other);
    unsigned int* old_data = _data;
    _data = temp._data;
    _size = temp._size;
    temp._data = old_data;
    return *this;
}
BitField& BitField::operator = (BitField&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] _data;
    _data = other._data;
    _size = other._size;
    other._data = nullptr;
    other._size = 0;
    return *this;
}

BitField BitField::operator & (const BitField& other) const {
    if (_size != other._size) {
        throw std::invalid_argument("Different BitField sizes");
    }
    BitField result(_size);
    size_t words = (_size + BITS_IN_WORD - 1) / BITS_IN_WORD;
    for (size_t i = 0; i < words; ++i) {
        result._data[i] = _data[i] & other._data[i];
    }
    return result;
}
BitField BitField::operator | (const BitField& other) const {
    if (_size != other._size) {
        throw std::invalid_argument("Different BitField sizes");
    }
    BitField result(_size);
    size_t words = (_size + BITS_IN_WORD - 1) / BITS_IN_WORD;
    for (size_t i = 0; i < words; ++i) {
        result._data[i] = _data[i] | other._data[i];
    }
    return result;
}
BitField& BitField::operator &= (const BitField& other) {
    *this = *this & other;
    return *this;
}
BitField& BitField::operator |= (const BitField& other) {
    *this = *this | other;
    return *this;
}
BitField BitField::operator ~ () const {
    BitField result(_size);
    for (size_t i = 0; i < _size; ++i) {
        if (!get_bit(i)) {
            result.set_bit(i);
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const BitField& v) {
    for (size_t i = 0; i < v._size; i++) {
        if (v.get_bit(i)) {
            os << '1';
        } else {
            os << '0';
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, BitField& v) {
    std::string input;
    is >> input;
    if (!is) {
        return is;
    }
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != '0' && input[i] != '1') {
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    BitField temp(input.size());
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '1') {
            temp.set_bit(i);
        }
        else {
            temp.reset_bit(i);
        }
    }
    v = std::move(temp);
    return is;
}