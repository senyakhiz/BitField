#include "Set.h"
#include <iostream>
#include <stdexcept>
#include <vector>

Set::Set() {}

Set::Set(size_t size) {
    _field = BitField(size);
}

Set::Set(const BitField& field) {
    _field = field;
}

Set::Set(const Set& other) {
    _field = other._field;
}

Set::Set(Set&& other) noexcept {
    _field = std::move(other._field);
}

Set& Set::operator=(const Set& other) {
    if (this == &other) {
        return *this;
    }
    _field = other._field;
    return *this;
}

Set& Set::operator=(Set&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    _field = std::move(other._field);
    return *this;
}

void Set::insert(size_t pos) {
    _field.set_bit(pos);
}

void Set::erase(size_t pos) {
    _field.reset_bit(pos);
}

bool Set::is_member(size_t pos) const {
    return _field.get_bit(pos);
}

Set Set::operator+(size_t element) const {
    size_t new_size = _field.get_size();
    if (element >= new_size) {
        new_size = element + 1;
    }
    Set result(new_size);
    for (size_t i = 0; i < _field.get_size(); i++) {
        if (is_member(i)) {
            result.insert(i);
        }
    }
    result.insert(element);
    return result;
}

Set Set::operator-(size_t element) const {
    Set result(_field.get_size());
    for (size_t i = 0; i < _field.get_size(); i++) {
        if (is_member(i) && i != element) {
            result.insert(i);
        }
    }
    return result;
}

bool Set::operator*(size_t element) const {
    if (element >= _field.get_size()) {
        return false;
    }
    return is_member(element);
}

Set Set::operator+(const Set& other) const {
    size_t size1 = _field.get_size();
    size_t size2 = other._field.get_size();
    size_t result_size = size1;
    if (size2 > result_size) {
        result_size = size2;
    }
    Set result(result_size);
    for (size_t i = 0; i < result_size; i++) {
        bool first = false;
        bool second = false;
        if (i < size1) {
            first = is_member(i);
        }
        if (i < size2) {
            second = other.is_member(i);
        }
        if (first || second) {
            result.insert(i);
        }
    }
    return result;
}

Set Set::operator-(const Set& other) const {
    Set result(_field.get_size());
    for (size_t i = 0; i < _field.get_size(); i++) {
        if (is_member(i)) {
            if (i >= other._field.get_size() ||
                !other.is_member(i)) {
                result.insert(i);
            }
        }
    }
    return result;
}

Set Set::operator*(const Set& other) const {
    size_t size1 = _field.get_size();
    size_t size2 = other._field.get_size();
    size_t result_size = size1;
    if (size2 < result_size) {
        result_size = size2;
    }
    Set result(result_size);
    for (size_t i = 0; i < result_size; i++) {
        if (is_member(i) && other.is_member(i)) {
            result.insert(i);
        }
    }
    return result;
}

Set& Set::operator+=(size_t element) {
    *this = *this + element;
    return *this;
}

Set& Set::operator-=(size_t element) {
    *this = *this - element;
    return *this;
}


Set& Set::operator+=(const Set& other) {
    *this = *this + other;
    return *this;
}

Set& Set::operator-=(const Set& other) {
    *this = *this - other;
    return *this;
}

Set& Set::operator*=(const Set& other) {
    *this = *this * other;
    return *this;
}

Set Set::operator - () const {
    Set result(_field.get_size());
    for (size_t i = 0; i < _field.get_size(); i++) {
        if (!is_member(i)) {
            result.insert(i);
        }
    }
    return result;
}

Set::operator BitField() const {
    return _field;
}

std::ostream& operator<<(std::ostream& os, const Set& v) {
    os << "{ ";
    bool first = true;
    for (size_t i = 0; i < v._field.get_size(); i++) {
        if (v.is_member(i)) {
            if (!first) {
                os << ", ";
            }
            os << i;
            first = false;
        }
    }
    if (first) {
        os << "}";
    } else {
        os << " }";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Set& v) {
    char symbol;
    is >> symbol;
    if (!is) {
        return is;
    }
    if (symbol != '{') {
        is.setstate(std::ios::failbit);
        return is;
    }
    std::vector<size_t> elements;
    is >> std::ws;
    if (is.peek() == '}') {
        is.get();
        Set temp;
        v = std::move(temp);
        return is;
    }
    while (true) {
        size_t element;
        if (!(is >> element)) {
            return is;
        }
        elements.push_back(element);
        is >> std::ws;
        if (!(is >> symbol)) {
            return is;
        }
        if (symbol == '}') {
            break;
        }
        if (symbol != ',') {
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    size_t max_element = 0;
    for (size_t i = 0; i < elements.size(); i++) {
        if (elements[i] > max_element) {
            max_element = elements[i];
        }
    }
    Set temp(max_element + 1);
    for (size_t i = 0; i < elements.size(); i++) {
        temp.insert(elements[i]);
    }
    v = std::move(temp);
    return is;
}