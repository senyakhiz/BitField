#pragma once
#include <iostream>

class BitField {
    unsigned int* _data;
    size_t _size;
    
    unsigned int get_mask(size_t pos) const;

public:
    BitField();
    explicit BitField(size_t size);
    BitField(const BitField& other);
    BitField(BitField&& other) noexcept;
    ~BitField();

    void set_bit(size_t pos);
    void reset_bit(size_t pos);
    bool get_bit(size_t pos) const;

    size_t get_index(size_t pos) const;
    size_t get_size() const;

    BitField& operator = (const BitField& other);
    BitField& operator = (BitField&& other) noexcept;

    BitField operator & (const BitField& other) const;
    BitField operator | (const BitField& other) const;
    BitField& operator &= (const BitField& other);
    BitField& operator |= (const BitField& other);
    BitField operator ~ () const;

    friend std::ostream& operator<<(std::ostream& os, const BitField& v);
    friend std::istream& operator>>(std::istream& is, BitField& v);
};
