/*
 * Copyright (c) 2015 Rémi Saurel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//
//  unistring.hpp
//
//  Created by Rémi on 22/09/2015.
//
//

#ifndef unistring_hpp
#define unistring_hpp

#include <iostream>
#include <string>
#include <vector>
#include <experimental/type_traits>
#include <sstream>
#include <array>
namespace std {
    using namespace std::experimental;
}

template <typename CharType, typename _T = std::enable_if_t<std::is_same_v<CharType, char> || std::is_same_v<CharType, char16_t> || std::is_same_v<CharType, char32_t>>>
class basic_unistring : private std::basic_string<CharType> {
    using base_type = std::basic_string<CharType>;

public:
    using typename base_type::value_type;
    using char_type = typename base_type::value_type;

    using typename base_type::size_type;
    using typename base_type::difference_type;

    using typename base_type::reference;
    using typename base_type::const_reference;
    using typename base_type::pointer;
    using typename base_type::const_pointer;

    using typename base_type::iterator;
    using typename base_type::const_iterator;
    using typename base_type::reverse_iterator;
    using typename base_type::const_reverse_iterator;

    using typename base_type::allocator_type;


    using base_type::npos;

    template <typename CharType2, typename... Args>
    static basic_unistring createWithFormat(basic_unistring<CharType2> format, Args &&... args) {
        basic_unistring string;
        string.append_format(format, std::forward<Args &&>(args)...);

        return string;
    }

    using std::basic_string<CharType>::basic_string;

    basic_unistring() {}

    basic_unistring(basic_unistring const &other, size_type pos, size_type count = basic_unistring::npos) {
        auto first = other.begin() + pos, last = other.end();
        if(count != basic_unistring::npos) {
            last = other.begin() + pos + count;
        }

        *this = basic_unistring(first, last);
    }

    /**
     * Constructs the basic_unistring object from the given null-terminated, wide or narrow char array, performaing the
     * necessary
     * conversion.
     */
    template <typename CharType2, typename = std::enable_if_t<!std::is_same_v<CharType, CharType2>>>
    basic_unistring(CharType2 const *str)
            : basic_unistring(std::basic_string<CharType2>(str)) {}

    /**
     * Constructs the basic_unistring object from the given basic_unistring, performing the necessary conversion.
     */
    template <typename CharType2, typename = std::enable_if_t<!std::is_same_v<CharType, CharType2>>>
    basic_unistring(basic_unistring<CharType2> const &str);

    /**
     * Constructs the basic_unistring object from the given basic_string, performing the necessary conversion.
     */
    basic_unistring(std::basic_string<CharType> const &str) : std::basic_string<CharType>(str) {}
    template <typename CharType2, typename = std::enable_if_t<!std::is_same_v<CharType, CharType2>>>
    basic_unistring(std::basic_string<CharType2> const &str)
            : basic_unistring<CharType>(basic_unistring<CharType2>(str)) {}

    template <typename T>
    basic_unistring &operator=(T const &value) {
        using std::swap;
        basic_unistring tmp(value);
        swap(*this, tmp);

        return *this;
    }
    template <typename T>
    basic_unistring &assign(T const &value) {
        return *this = value;
    }

    using base_type::begin;
    using base_type::end;
    using base_type::cbegin;
    using base_type::cend;
    using base_type::rbegin;
    using base_type::rend;
    using base_type::crbegin;
    using base_type::crend;

    using base_type::front;
    using base_type::back;

    template <typename T>
    basic_unistring &append(T const &value) {
        return *this += value;
    }

    template <typename T>
    basic_unistring &operator+=(T const &value) {
        basic_unistring tmp(value);
        this->basic_string::operator+=(tmp);
        return *this;
    }

    using base_type::at;
    using base_type::operator[];

    using base_type::c_str;
    using base_type::data;

    using base_type::size;
    using base_type::length;
    using base_type::empty;

    using base_type::capacity;
    using base_type::reserve;
    using base_type::max_size;
    using base_type::shrink_to_fit;

    using base_type::clear;
    using base_type::insert;
    using base_type::erase;
    using base_type::push_back;
    using base_type::pop_back;

    using base_type::compare;
    using base_type::replace;

    template <typename InputIt>
    std::enable_if_t<std::is_convertible_v<typename InputIt::iterator_category, std::input_iterator_tag>, basic_unistring &>
        replace(InputIt it1, InputIt it2, basic_unistring<CharType> const &value) {
        this->basic_string::replace(it1, it2, value.begin(), value.end());
        return *this;
    }
    template <typename InputIt>
    std::enable_if_t<std::is_convertible_v<typename InputIt::iterator_category, std::input_iterator_tag>, basic_unistring &>
        replace(InputIt it1, InputIt it2, std::basic_string<CharType> const &value) {
        this->basic_string::replace(it1, it2, value.begin(), value.end());
        return *this;
    }
    template <typename InputIt>
    std::enable_if_t<std::is_convertible_v<typename InputIt::iterator_category, std::input_iterator_tag>, basic_unistring &>
        replace(InputIt it1, InputIt it2, CharType const *value) {
        this->basic_string::replace(it1, it2, value);
        return *this;
    }
    template <typename InputIt>
    std::enable_if_t<std::is_convertible_v<typename InputIt::iterator_category, std::input_iterator_tag>, basic_unistring &>
        replace(InputIt it1, InputIt it2, size_type count, CharType value) {
        this->basic_string::replace(it1, it2, count, value);
        return *this;
    }

    basic_unistring &replace(size_type pos, size_type count, basic_unistring<CharType> const &value) {
        this->replace(begin() + pos, begin() + pos + count, value.begin(), value.end());
        return *this;
    }
    basic_unistring &replace(size_type pos, size_type count, std::basic_string<CharType> const &value) {
        this->replace(begin() + pos, begin() + pos + count, value.begin(), value.end());
        return *this;
    }
    basic_unistring &replace(size_type pos, size_type count, CharType const *value) {
        this->replace(begin() + pos, begin() + pos + count, value);
        return *this;
    }
    basic_unistring &replace(size_type pos, size_type count, size_type count2, CharType value) {
        this->replace(begin() + pos, begin() + pos + count, count2, value);
        return *this;
    }

    basic_unistring substr(size_type pos = 0, size_type n = npos) const {
        return basic_unistring(*this, pos, n);
    }
    using base_type::copy;
    using base_type::resize;
    using base_type::swap;

    using base_type::find;
    using base_type::rfind;
    using base_type::find_first_of;
    using base_type::find_first_not_of;
    using base_type::find_last_of;
    using base_type::find_last_not_of;

    template <bool = std::is_same_v<CharType, char>>
    std::string to_utf8() const {
        return {begin(), end()};
    }

    template <bool = std::is_same_v<CharType, char16_t>>
    std::u16string to_utf16() const {
        return {begin(), end()};
    }

    template <bool = std::is_same_v<CharType, char32_t>>
    std::u32string to_utf32() const {
        return {begin(), end()};
    }

    operator std::string() const {
        return to_utf8();
    }

    operator std::basic_string<char16_t>() const {
        return to_utf16();
    }

    operator std::basic_string<char32_t>() const {
        return to_utf32();
    }

    void normalize() {
        *this = this->get_normalized();
    }

    basic_unistring get_normalized() const;

    template <template <typename...> class Container = std::vector>
    Container<basic_unistring> split(CharType separator) const {
        return this->split(basic_unistring{separator});
    }

    template <template <typename...> class Container = std::vector>
    Container<basic_unistring> split(basic_unistring const &separator) const;

    template <typename CharType2>
    bool starts_with(basic_unistring<CharType2> const &substr) {
        return this->startsWith(basic_unistring{substr});
    }
    bool starts_with(basic_unistring<CharType> const &substr);

    template <typename CharType2>
    bool ends_with(basic_unistring<CharType2> const &substr) {
        return this->ends_with(basic_unistring{substr});
    }
    bool ends_with(basic_unistring<CharType> const &substr);

    template <typename CharType2, typename... Args>
    basic_unistring &append_format(basic_unistring<CharType2> format, Args &&... args) {
        return this->append_format(basic_unistring{format}, args...);
    }

    template <typename CharType2, typename... Args>
    basic_unistring &append_format(std::basic_string<CharType2> format, Args &&... args) {
        return this->append_format(basic_unistring{format}, args...);
    }

    template <typename... Args>
    basic_unistring &append_format(basic_unistring format, Args &&... args);

private:
    static void map_arguments(std::vector<basic_unistring> &vect) {}

    template <typename Arg, typename... Args>
    static void map_arguments(std::vector<basic_unistring> &vect, Arg &&arg, Args &&... args);
};

template <typename CharType, typename T>
template <template <typename...> class Container>
Container<basic_unistring<CharType, T>> basic_unistring<CharType, T>::split(basic_unistring<CharType, T> const &separator) const {
    Container<basic_unistring<CharType, T>> container;
    size_type start = 0;
    auto pos = this->find(separator, start);
    while(pos != basic_unistring::npos) {
        container.emplace_back(*this, start, pos - start);
        start = pos + 1;
        pos = this->find(separator, start);
    }
    if(start < this->length())
        container.emplace_back(*this, start, this->size() - start);

    return container;
}

template <typename CharType, typename T>
template <typename Arg, typename... Args>
void basic_unistring<CharType, T>::map_arguments(std::vector<basic_unistring> &vect, Arg &&arg, Args &&... args) {
    std::stringstream ss;
    ss << arg;
    vect.push_back(ss.str());

    map_arguments(vect, args...);
}

template <typename CharType, typename T>
template <typename... Args>
basic_unistring<CharType, T> &basic_unistring<CharType, T>::append_format(basic_unistring format, Args &&... args) {
    std::vector<basic_unistring> formattedArgs;
    basic_unistring::map_arguments(formattedArgs, args...);

    size_type start = 0;
    auto pos = format.find((CharType)'{', start);
    while(pos != basic_unistring::npos) {
        start = pos + 1;
        pos = format.find((CharType)'{', start);
        if(pos != start) {
            auto end = format.find((CharType)'}', start + 1);
            if(end != npos) {
                size_t index;
                std::stringstream str(format.substr(start, end - start).to_utf8());
                str >> index;
                if(str.fail() || !str.eof()) {
                    index = -1;
                }
                auto const &arg = formattedArgs.at(index);
                auto sizeDiff = arg.size() - (end - start);
                format.replace(start - 1, end - start + 2, arg);
                start += sizeDiff;
            }
        } else {
            ++start;
        }
        pos = format.find((CharType)'{', start);
    }

    this->append(format);

    return *this;
}

namespace unistring_literals {
    basic_unistring<char> operator""_u8(char const *str, size_t length);
    basic_unistring<char16_t> operator""_u16(char const *str, size_t length);
    basic_unistring<char32_t> operator""_u32(char const *str, size_t length);
}

extern template class basic_unistring<char>;
extern template class basic_unistring<char16_t>;
extern template class basic_unistring<char32_t>;

template <typename CharType>
std::ostream &operator<<(std::ostream &stream, basic_unistring<CharType> const &str) {
    stream << str.to_utf8();
    return stream;
}

// operator+

template <typename CharType, typename T>
basic_unistring<CharType, T> operator+(basic_unistring<CharType, T> const &lhs, basic_unistring<CharType, T> const &rhs) {
    return basic_unistring<CharType, T>(lhs).append(rhs);
}

template <typename CharType, typename T, typename CharType2, typename Traits, typename Alloc>
basic_unistring<CharType, T>
    operator+(basic_unistring<CharType, T> const &lhs, std::basic_string<CharType2, Traits, Alloc> const &rhs) {
    return basic_unistring<CharType, T>(lhs).append(rhs);
}

template <typename CharType, typename T, typename CharType2>
basic_unistring<CharType, T> operator+(basic_unistring<CharType, T> const &lhs, CharType2 const *rhs) {
    return basic_unistring<CharType, T>(lhs) + basic_unistring<CharType, T>(rhs);
}

template <typename CharType, typename T>
basic_unistring<CharType, T> operator+(basic_unistring<CharType, T> const &lhs, CharType rhs) {
    return basic_unistring<CharType, T>(lhs) + basic_unistring<CharType, T>(1, rhs);
}

template <typename CharType, typename T, typename CharType2, typename Traits, typename Alloc>
basic_unistring<CharType, T>
    operator+(std::basic_string<CharType2, Traits, Alloc> const &rhs, basic_unistring<CharType, T> const &lhs) {
    return basic_unistring<CharType, T>(lhs).append(rhs);
}

template <typename CharType, typename T, typename CharType2>
basic_unistring<CharType, T> operator+(CharType2 const *rhs, basic_unistring<CharType, T> const &lhs) {
    return basic_unistring<CharType, T>(lhs) + basic_unistring<CharType, T>(rhs);
}

template <typename CharType, typename T>
basic_unistring<CharType, T> operator+(CharType rhs, basic_unistring<CharType, T> const &lhs) {
    return basic_unistring<CharType, T>(lhs) + basic_unistring<CharType, T>(1, rhs);
}

// Comparison operators

inline bool operator==(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator==(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} == basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator==(T const &lhs, basic_unistring<CharType> const &rhs) {
    return rhs == lhs;
}

inline bool operator<(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator<(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} < basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator<(T const &lhs, basic_unistring<CharType> const &rhs) {
    return rhs > lhs;
}

inline bool operator!=(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return !(lhs == rhs);
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator!=(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} != basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator!=(T const &lhs, basic_unistring<CharType> const &rhs) {
    return rhs != lhs;
}

inline bool operator>(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return rhs < lhs;
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator>(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} > basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator>(T const &lhs, basic_unistring<CharType> const &rhs) {
    return rhs < lhs;
}

inline bool operator>=(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return !(lhs < rhs);
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator>=(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} >= basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator>=(T const &lhs, basic_unistring<CharType> const &rhs) {
    return !(rhs < lhs);
}

inline bool operator<=(basic_unistring<char32_t> const &lhs, basic_unistring<char32_t> const &rhs) {
    return !(lhs > rhs);
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator<=(basic_unistring<CharType> const &lhs, T const &rhs) {
    return basic_unistring<char32_t>{lhs} <= basic_unistring<char32_t>{rhs};
}

template <typename CharType, typename T>
inline std::enable_if_t<!std::is_same_v<T, basic_unistring<char32_t>>, bool>
    operator<=(T const &lhs, basic_unistring<CharType> const &rhs) {
    return !(rhs > lhs);
}

template <typename CharType>
struct std::hash<basic_unistring<CharType>> {
    // Horribly inefficient, this has to be rewritten.
    size_t operator()(basic_unistring<CharType> const &key) const {
        return std::hash<std::basic_string<char32_t>>{}(key.to_utf32());
    }
};

#endif /* unistring_hpp */
