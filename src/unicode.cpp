#include "unicode.h"

namespace utf_str {

    code_point_ref::code_point_ref(unsigned char *ptr, size_t length) : ptr{ptr}, length{length} {}

    std::vector<uint8_t> code_point_ref::to_vec() const {
        std::vector<uint8_t> temp{};

        if (length > 0) {
            // Push contents from pointer location with given length into temp
            for (size_t i{}; i < length; i++) {
                temp.push_back(ptr[i]);
            }

            temp.push_back('\0');
        }

        return temp;
    }

    std::ostream &operator<<(std::ostream &os, const code_point_ref &cp_ref) {
        auto temp = cp_ref.to_vec();
        temp.pop_back(); // remove the null terminator

        for (size_t i{}; i < temp.size(); i++) {
            os << temp[i];
        }

        return os;
    }

    utf_str::String::String(const char *str) {
        push_back(str);
    }

    void String::push_back(const char *str) {
        while (*str) {
            data_push_back_null(static_cast<uint8_t>(*str));
            ++str;
        }

        update_cp_index_vector();
    }

    void utf_str::String::push_back(uint32_t code_point) {
        // Extracts bits from code_point and constructs the bytes according to the UTF-8 encoding rules

        /* * * * * * * * * * * * * * * * * * * * * * * * *
         * 1 byte  : 0ZZZZZZZ
         * 2 bytes : 110YYYYY 10ZZZZZZ
         * 3 bytes : 1110XXXX 10YYYYYY 10ZZZZZZ
         * 4 bytes : 11110VVV 10WWXXXX 10YYYYYY 10ZZZZZZ
         * * * * * * * * * * * * * * * * * * * * * * * * */

        if (code_point <= 0x7F) {
            // If in the range U+0000 to U+007F (ascii), use 1 byte
            data_push_back_null(static_cast<uint8_t>(code_point)); // push byte
        } else if (code_point <= 0x7FF) {
            // If in the range U+0080 to U+07FF, use 2 bytes
            data_push_back_null(static_cast<uint8_t>(0b11000000 | ( code_point  >> 6)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ( code_point           & 0b00111111)));
        } else if (code_point <= 0xFFFF) {
            // If in the range U+0800 to U+FFFF, use 3 bytes
            data_push_back_null(static_cast<uint8_t>(0b11100000 | ( code_point >> 12)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ((code_point >> 6 )    & 0b00111111)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ( code_point           & 0b00111111)));
        } else if (code_point <= 0x10FFFF) {
            // If in the range U+10000 to U+10FFFF, use 4 bytes
            data_push_back_null(static_cast<uint8_t>(0b11110000 | ( code_point >> 18)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ((code_point >> 12)    & 0b00111111)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ((code_point >> 6 )    & 0b00111111)));
            data_push_back_null(static_cast<uint8_t>(0b10000000 | ( code_point           & 0b00111111)));
        } else {
            throw std::invalid_argument("Invalid unicode code point");
        }

        update_cp_index_vector();
    }

    void String::update_cp_index_vector() {
        // Scan through the vector, detect the code_points, update the cp_index_data with the data_index
        // data_index: index of the first byte of a codepoint in the cp_index_data vector

        // Clear the current cp_index_data vector
        cp_index_data.clear();

        // Scan through the data vector, find the codepoints and add them to the cp_index_data vector
        for (size_t data_index = 0; data_index < data.size();) {
            /* Check how many bytes the character is by checking the marker bits in the first byte */

            auto cp_length = get_codepoint_length(data[data_index]);

            if (cp_length == 1) {
                // If the most significant bit is 0 it's a single-byte character
                cp_index_data.push_back(data_index);
                data_index++;
            } else if (cp_length == 2) {
                // If the 3 most significant bits are 110, then 2 byte character
                cp_index_data.push_back(data_index);
                data_index += 2;
            } else if (cp_length == 3) {
                // If the 4 most significant bits are 1110, then 3 byte character
                cp_index_data.push_back(data_index);
                data_index += 3;
            } else if (cp_length == 4) {
                // If the 5 most significant bits are 11110, then 4 byte character
                cp_index_data.push_back(data_index);
                data_index += 4;
            } else {
                data_index++;
            }
        }

    }

    void utf_str::String::debug_print() const {
        for (uint8_t byte: data) {
            std::cout << static_cast<char>(byte); // Output as char
        }
        std::cout << std::endl;
    }

    void String::debug_print_hex() const {
        for (uint8_t byte: data) {
            std::cout << std::hex << static_cast<int>(byte) << " " << std::dec;
        }

        std::cout << std::endl;
    }

    size_t String::size_byte() const {
        return data.size() - 1; // -1 to compensate for null terminator
    }

    size_t String::length() const {
        return cp_index_data.size() - 1; // -1 to compensate for the null terminator
    }

    bool String::empty() const {
        return data.empty();
    }

    void String::clear() {
        data.clear();
        update_cp_index_vector();
    }

    void String::erase_byte(size_t start, size_t count) {
        if ((start + count) < data.size()) {
            data.erase(data.begin() + static_cast<uint8_t>(start), data.begin() + static_cast<uint8_t>(count));
        } else {
            throw std::out_of_range("Erasing out of bounds of string");
        }

        update_cp_index_vector();
    }

    void String::erase(size_t cp_start, size_t cp_count) {

        if ((cp_start + cp_count) <= this->length()) {
            data.erase(data.begin() + static_cast<uint8_t>(cp_index_data[cp_start]),
                       data.begin() + static_cast<uint8_t>(cp_index_data[cp_count]));
        } else {
            throw std::out_of_range("Erasing out of bounds of string");
        }

        update_cp_index_vector();
    }


    code_point_ref String::operator[](size_t cp_index) {

        if (cp_index >= this->length()) { // because [0] is the 1st element, so 4 elements (size of 4) means max [3]
            throw std::out_of_range("Index out of bounds");
        } else {
            auto cp_length = get_codepoint_length(data[cp_index]);

            return code_point_ref{&data[cp_index_data[cp_index]], cp_length};
        }

    }


    String String::subtr_byte(size_t start, size_t count) const {
        String temp{}; // Holds the substring

        if ((start + count) < data.size()) {
            for (auto i = start; i < start + count; i++) {
                temp.data_push_back_null(data[i]);
            }
        } else {
            throw std::runtime_error("Substring out of bounds of string");
        }

        return temp;
    }

    String String::subtr(size_t cp_start, size_t cp_count) const {
        String temp{}; // Holds the substring

        if ((cp_start + cp_count) <= this->length()) {
            // Copy substring into temp
            for (auto i = cp_start; i < cp_start + cp_count; i++) {
                // Get the real data index of the codepoint index
                auto index = cp_index_data[i];

                // Length of the codepoint is the distance between its beginning byte and the beginning byte of the next cp_index location
                // No out of bound issues here from the i+1 because we are always searching < cp_start + cp_count
                // The i+1 will then be on the null terminator thus not out of bounds
                auto cp_length = cp_index_data[i+1] - cp_index_data[i];

                // Push all bytes of the codepoint into temp
                for (size_t j{}; j < cp_length; j++) {
                    temp.data_push_back_null(data[index + j]);
                }

            }
        } else {
            throw std::runtime_error("Substring out of bounds of string");
        }

        return temp;
    }

    std::ostream &operator<<(std::ostream &os, const String &string) {
        for (uint8_t byte: string.data) {
            os << static_cast<char>(byte);
        }
        return os;
    }

    std::istream &operator>>(std::istream &is, String &string) {
        std::string temp;
        is >> temp;
        string = String(temp.c_str());
        return is;
    }

    const uint8_t *String::c_str() const {
        // Returns pointer to the underlying character storage.
        if (data.empty()) {
            return nullptr;
        }

        return &data[0];
    }

    void String::data_push_back_null(uint8_t c) {
        // Ensures that the last character is always '\0'
        if (!data.empty() && data.back() == '\0') {
            // If the last element is '\0' replace it with the new character
            data.back() = c;
            data.push_back('\0');
        } else {
            // If empty, push the data and push '\0' behind it.
            data.push_back(c);
            data.push_back('\0');
        }
    }

    String operator+(const String &lhs, const String &rhs) {
        String result = lhs;

        for (int i{}; i < rhs.data.size(); i++) {
            result.push_back(rhs.data[i]);
        }

        return result;
    }

    code_point_ref String::front() {
        // Length of the codepoint is the distance between its beginning byte and the beginning byte of the next cp_index location
        auto cp_length = cp_index_data[1] - cp_index_data[0];

        return code_point_ref{&data[cp_index_data[0]], cp_length};
    }

    code_point_ref String::back() {
        // The length is the first byte of the last codepoint to the very last byte in the data
        // This is last_element->second to data.size();

        // cp_index_data.size() - 1 is the last element in the cp_index_data vector, since the key is element number n+1.
        // However, the last element is a '\0', therefore we need to use length() that already compensates for the length without null terminator

        // length() is the number of items in the cp_index_data vector without the null terminator
        // We do length()-1 because the index 0 is element 1 (the first element)

        auto last_element_key = this->length() - 1; // length()-1 because the index 0 is element 1 (the first element)
        auto last_element_value = cp_index_data[last_element_key];

        auto last_byte = this->size_byte(); // size_byte because we don't count the null terminator, which is the true last byte

        auto cp_length = last_byte - last_element_value;

        return code_point_ref{&data[last_element_value], cp_length};
    }

    size_t String::get_codepoint_length(uint8_t first_byte) const {

        /* Check how many bytes the character is by checking the marker bits in the first byte */
        if ((first_byte & 0b10000000) == 0b00000000) {
            // If the most significant bit is 0 it's a single-byte character
            return 1;
        } else if ((first_byte & 0b11100000) == 0b11000000) {
            // If the 3 most significant bits are 110, then 2 byte character
            return 2;
        } else if ((first_byte & 0b11110000) == 0b11100000) {
            // If the 4 most significant bits are 1110, then 3 byte character
            return 3;
        } else if ((first_byte & 0b11111000) == 0b11110000) {
            // If the 5 most significant bits are 11110, then 4 byte character
            return 4;
        } else {
            // Invalid UTF-8 sequence
            return 0;
        }
    }

}