#pragma once

#include <iostream>
#include <vector>
#include <memory>      
#include <string_view> 
#include <compare>     
#include <algorithm>   
#include <stdexcept>   


namespace nerett {

template <typename CharT>
class CowString {
private:
    struct InternalBuffer {
        std::vector<CharT> m_data;

        explicit InternalBuffer(std::basic_string_view<CharT> sv)
            : m_data(sv.begin(), sv.end()) {
            m_data.push_back(CharT{});
        }
        
        InternalBuffer(const InternalBuffer& other) = default;
    };

    std::shared_ptr<InternalBuffer> m_p_buf;

    void detach() {
        if (m_p_buf.use_count() > 1) {
            m_p_buf = std::make_shared<InternalBuffer>(*m_p_buf);
        }
    }

public:
    CowString() noexcept
        : m_p_buf(std::make_shared<InternalBuffer>(std::basic_string_view<CharT>{})) {}

    CowString(std::basic_string_view<CharT> sv)
        : m_p_buf(std::make_shared<InternalBuffer>(sv)) {}

    CowString(const CharT* s)
        : CowString(std::basic_string_view<CharT>(s)) {}

    CowString(const CowString& other) = default;
    CowString& operator=(const CowString& other) = default;
    CowString(CowString&& other) = default;
    CowString& operator=(CowString&& other) = default;
    ~CowString() = default;

    const CharT& at(size_t pos) const {
        if (pos >= size()) {
            throw std::out_of_range("CowString::at");
        }
        return m_p_buf->m_data[pos];
    }
    
    const CharT& operator[](size_t pos) const & noexcept {
        return m_p_buf->m_data[pos];
    }

    CharT& at(size_t pos) & {
        if (pos >= size()) {
            throw std::out_of_range("CowString::at");
        }
        detach();
        return m_p_buf->m_data[pos];
    }

    CharT& operator[](size_t pos) & {
        detach();
        return m_p_buf->m_data[pos];
    }

    size_t size() const noexcept {
        return m_p_buf->m_data.size() - 1;
    }

    bool empty() const noexcept {
        return size() == 0;
    }

    const CharT* c_str() const & noexcept {
        return m_p_buf->m_data.data();
    }
    
    std::basic_string_view<CharT> view() const noexcept {
        return {m_p_buf->m_data.data(), size()};
    }

    operator std::basic_string_view<CharT>() const noexcept {
        return view();
    }

    long use_count() const noexcept {
        return m_p_buf.use_count();
    }
};

template <typename CharT>
auto operator<=>(const CowString<CharT>& lhs, const CowString<CharT>& rhs) noexcept {
    return lhs.view() <=> rhs.view();
}

template <typename CharT>
bool operator==(const CowString<CharT>& lhs, const CowString<CharT>& rhs) noexcept {
    return lhs.view() == rhs.view();
}

template <typename CharT>
std::ostream& operator<<(std::ostream& os, const CowString<CharT>& s) {
    return os << s.view();
}

} // namespace nerett
