#ifndef UNICODE_UNICODE_H
#define UNICODE_UNICODE_H

#include <iostream>
#include <vector>
#include <cstdint> // Guaranteed sizes

namespace utf_str {

    // Struct that holds a pointer to the first byte of a code point and the length of the code point
    struct code_point_ref {
        code_point_ref(unsigned char* ptr, size_t length);
        unsigned char *ptr{}; // Pointer to the first byte of the code_point
        size_t length{}; // Length in bytes of code point

        std::vector<uint8_t> to_vec() const; // Convert pointer and its length to a vector with the codepoint's contents
        friend std::ostream& operator<<(std::ostream& os, const code_point_ref& cp_ref);
    };

    class String {
    private:
        std::vector<uint8_t> data; // Vector of bytes stored in the string
        std::vector<size_t> cp_index_data; // Contains the first byte of each character's codepoint in data, sequentially from left to right.

        void data_push_back_null(uint8_t c); // Push back but ensure the last byte is always '\0' for c string compatibility
        void update_cp_index_vector(); // Update cp_index_map with the new indexes of data
        size_t get_codepoint_length(uint8_t first_byte) const; // returns the number of bytes in a code point

    public:
        // Constructors
        String() = default;
        explicit String(const char* str);

        size_t size_byte() const; // Size of the string in bytes, ignoring null terminator
        size_t length() const; // Length of the string in code points, ignoring null terminator
        bool empty() const; // Return true if empty

        // Modifiers, all of these call update_cp_index_vector()
        void clear(); // Clear entire string
        void push_back(const char* c); // Push back a unicode character.
        void push_back(uint32_t code_point); // Push back a code point (hexadecimal)
        void erase_byte(size_t start, size_t count); // Erase bytes from index start to count
        void erase(size_t cp_start, size_t cp_count); // Erase codepoint characters from index start to count.

        // Accessors
        code_point_ref operator[](size_t cp_index); // Get character at codepoint index, returns it as code_point_ref
        String subtr_byte(size_t start, size_t count) const; // Return substring from byte index start to count
        String subtr(size_t cp_start, size_t cp_count) const; // Return substring from cp character index start to count
        code_point_ref front(); // Return code_point_ref to first character
        code_point_ref back(); // Return code_point_ref to last character, ignoring the null terminator

        const uint8_t* c_str() const; // Returns pointer to the underlying character storage.

        friend String operator+(const String& lhs, const String& rhs); // Concatenate 2 strings
        friend std::ostream& operator<<(std::ostream& os, const String& string); // Output stream operator
        friend std::istream& operator>>(std::istream& is, String& string); // Input stream operator

        void debug_print() const; // Print contents of string
        void debug_print_hex() const; // Print contents of string as hexadecimals

        ~String() = default;
    };

}

#endif //UNICODE_UNICODE_H
