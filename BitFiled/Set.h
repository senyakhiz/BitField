#pragma once
#include "BitField.h"
class Set {
    BitField _field;
public:
    Set();
    explicit Set(size_t size);
    Set(const BitField& field);
    Set(const Set& other);
    Set(Set&& other) noexcept;

    ~Set() = default;
    Set& operator = (const Set& other);
    Set& operator = (Set&& other) noexcept;

    void insert(size_t pos);
    void erase(size_t pos);

    bool is_member(size_t pos) const;

    Set operator + (size_t element) const;
    Set operator - (size_t element) const;
    bool operator * (size_t element) const;

    Set operator + (const Set& other) const;
    Set operator - (const Set& other) const;
    Set operator * (const Set& other) const;

    Set& operator+=(size_t element);
    Set& operator-=(size_t element);

    Set& operator+=(const Set& other);
    Set& operator-=(const Set& other);
    Set& operator*=(const Set& other);

    Set operator - () const;

    explicit operator BitField() const;

    friend std::ostream& operator<<(std::ostream& os, const Set& v);
    friend std::istream& operator>>(std::istream& is, Set& v);
};