//
//  unistring.cpp
//
//  Created by Rémi on 22/09/2015.
//
//

#include "basic_unistring.h"
#include <codecvt>
#include "utf8proc.h"
#include <algorithm>
#include <sstream>
#include <array>

namespace {
    template <typename FromType, typename ToType>
    std::basic_string<ToType>
    convertIn(basic_unistring<FromType> const &internal, std::codecvt<FromType, ToType, std::mbstate_t> const &codecvt);

    template <typename FromType, typename ToType>
    std::basic_string<FromType>
    convertOut(basic_unistring<ToType> const &internal, std::codecvt<FromType, ToType, std::mbstate_t> const &codecvt);
}

template <>
template <typename CharType2, typename>
basic_unistring<char>::basic_unistring(basic_unistring<CharType2> const &str)
        : basic_string<char>(str.to_utf8()) {}

template <>
template <typename CharType2, typename>
basic_unistring<char16_t>::basic_unistring(basic_unistring<CharType2> const &str)
        : basic_string<char16_t>(str.to_utf16()) {}

template <>
template <typename CharType2, typename>
basic_unistring<char32_t>::basic_unistring(basic_unistring<CharType2> const &str)
        : basic_string<char32_t>(str.to_utf32()) {}

template <typename CharType, typename T>
basic_unistring<CharType, T> basic_unistring<CharType, T>::get_normalized() const {
    return reinterpret_cast<char const *>(utf8proc_NFC(reinterpret_cast<utf8proc_uint8_t const *>(to_utf8().c_str())));
}

template <>
template <>
std::string basic_unistring<char16_t>::to_utf8<false>() const {
    return convertIn(*this, std::codecvt_utf8_utf16<char_type>{});
}

template <>
template <>
std::string basic_unistring<char32_t>::to_utf8<false>() const {
    return convertIn(*this, std::codecvt_utf8<char_type>{});
}

template <>
template <>
std::u16string basic_unistring<char>::to_utf16<false>() const {
    return convertOut(*this, std::codecvt_utf8_utf16<char16_t>{});
}

template <>
template <>
std::basic_string<char16_t> basic_unistring<char32_t>::to_utf16<false>() const {
    return basic_unistring<char>(this->to_utf8()).to_utf16();
}

template <>
template <>
std::u32string basic_unistring<char>::to_utf32<false>() const {
    return convertOut(*this, std::codecvt_utf8<char32_t>{});
}

template <>
template <>
std::basic_string<char32_t> basic_unistring<char16_t>::to_utf32<false>() const {
    return basic_unistring<char>(this->to_utf8()).to_utf32();
}

template <typename CharType, typename T>
bool basic_unistring<CharType, T>::starts_with(basic_unistring<CharType> const &substr) {
    return this->size() >= substr.size() && std::equal(substr.begin(), substr.end(), this->begin());
}

template <typename CharType, typename T>
bool basic_unistring<CharType, T>::ends_with(basic_unistring<CharType> const &substr) {
    return this->size() >= substr.size() && std::equal(substr.rbegin(), substr.rend(), this->rbegin());
}

namespace {
    template <typename FromType, typename ToType>
    std::basic_string<ToType>
    convertIn(basic_unistring<FromType> const &internal, std::codecvt<FromType, ToType, std::mbstate_t> const &codecvt) {
        std::string external(internal.size() * codecvt.max_length(), '\0');
        const FromType *from_next;
        std::mbstate_t mb = std::mbstate_t();
        ToType *to_next;
        codecvt.out(mb, internal.data(), internal.data() + internal.size(), from_next, &external[0], &external[external.size()], to_next);
        external.resize(to_next - &external[0]);

        return external;
    }

    template <typename FromType, typename ToType>
    std::basic_string<FromType>
    convertOut(basic_unistring<ToType> const &internal, std::codecvt<FromType, ToType, std::mbstate_t> const &codecvt) {
        std::basic_string<FromType> external(internal.size() * codecvt.max_length(), '\0');
        const ToType *from_next;
        std::mbstate_t mb = std::mbstate_t();
        FromType *to_next;
        codecvt.in(mb, internal.data(), internal.data() + internal.size(), from_next, &external[0], &external[external.size()], to_next);
        external.resize(to_next - &external[0]);

        return external;
    }
}

namespace unistring_literals {
    basic_unistring<char> operator""_u8(char const *str, size_t length) {
        return {str};
    }

    basic_unistring<char16_t> operator""_u16(char const *str, size_t length) {
        return {str};
    }

    basic_unistring<char32_t> operator""_u32(char const *str, size_t length) {
        return {str};
    }
}

template class basic_unistring<char>;
template class basic_unistring<char16_t>;
template class basic_unistring<char32_t>;

template basic_unistring<char>::basic_unistring(basic_unistring<char16_t> const &);
template basic_unistring<char>::basic_unistring(basic_unistring<char32_t> const &);
template basic_unistring<char16_t>::basic_unistring(basic_unistring<char> const &);
template basic_unistring<char16_t>::basic_unistring(basic_unistring<char32_t> const &);
template basic_unistring<char32_t>::basic_unistring(basic_unistring<char> const &);
template basic_unistring<char32_t>::basic_unistring(basic_unistring<char16_t> const &);
