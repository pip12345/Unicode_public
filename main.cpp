#include <iostream>
#include <vector>
#include "unicode.h"

#ifdef _WIN64
#include <windows.h> // Because windows sucks and needs special treatment for UTF_8
#endif // _WIN64

int main() {
#ifdef _WIN64
    SetConsoleOutputCP(CP_UTF8); // Required to properly display UTF_8 in Windows console
#endif // _WIN64

    utf_str::String ascii{"Hello ASCII"};

    // push_back() codepoint tests and print hex test
    utf_str::String str_1;
    str_1.push_back(0x0048); // Unicode code point for 'H'
    str_1.push_back(0x0065); // Unicode code point for 'e'
    str_1.push_back(0x6C34); // Unicode code point for '水'
    str_1.push_back(0x2112); // Unicode code point for 'ℒ'
    str_1.push_back(0x262D); // Unicode code point for '☭'
    str_1.push_back("♫♫♫");
    str_1.debug_print(); // Output the UTF-8 encoded string
    str_1.debug_print_hex();


    // Basic utf8 character tests
    utf_str::String str_2{"利纳克斯"};
    std::cout << std::endl << str_2 << std::endl;

    utf_str::String str_3{"Ḽơᶉëᶆ ȋṕšᶙṁ ḍỡḽǭᵳ ʂǐť ӓṁệẗ, ĉṓɲṩḙċťᶒțûɾ ấɖḯƥĭṩčįɳġ ḝłįʈ, șếᶑ ᶁⱺ ẽḭŭŝḿꝋď ṫĕᶆᶈṓɍ ỉñḉīḑȋᵭṵńť ṷŧ ḹẩḇőꝛế éȶ đꝍꞎôꝛȇ ᵯáꞡᶇā ąⱡîɋṹẵ."};
    std::cout << std::endl << str_3 << std::endl;

    utf_str::String str_5{"Блин"};
    std::cout << std::endl << str_5 << std::endl;

    // operator[] test
    std::cout << "\noperator[] test on last character of str_2, expected '斯': " << str_2[3] << std::endl << std::endl;


    std::cout << "str_4: ";
    utf_str::String str_4{"На прошлых выходных я решил съездить в Санкт-Петербург"};
    std::cout << std::endl << str_4 << std::endl;

    // erase() and substr() tests
    str_4.erase(0, 3);
    auto str_4_subtr = str_4.subtr(0, 11);

    std::cout << "\nerase(0, 3), substr(0, 11) of str_4: ";
    std::cout << std::endl << str_4 << std::endl;
    std::cout << "string_4_substr: " << str_4_subtr << std::endl;


    // erase_byte() and substr_byte() tests
    str_4.erase_byte(1, 10);
    auto str_4_subtr_byte = str_4.subtr_byte(0, 20);

    std::cout << "\nerase_byte(1, 10), substr_byte(0, 20) of str_4: ";
    std::cout << std::endl << str_4 << std::endl;
    std::cout << "string_4_substr_byte: " << str_4_subtr_byte << std::endl;


//    // operator>>() test
//    utf_str::String string_cin;
//    utf_str::String string_cin2{"利"};
//    std::cout << "\nType in contents for string_cin: ";
//    std::cin >> string_cin;
//    std::cout << "Printing contents of string_cin: ";
//    std::cout << string_cin << std::endl;


    // push_back() and c_str() test.
    utf_str::String str_ctest{"На прошлых выходных я решил съездить в Санкт-Петербург"};
    str_ctest.push_back(" Cool stuff");
    std::cout << std::endl << "contents of str_ctest.c_str(): " << str_ctest.c_str() << std::endl;


    // size_byte() and length() test
    utf_str::String str_lengthtest1{"what's up"};
    std::cout << std::endl << str_lengthtest1 << std::endl;
    std::cout << "size_byte: " << str_lengthtest1.size_byte() << " length: " << str_lengthtest1.length() << std::endl;
    utf_str::String str_lengthtest2{"прошлых"};
    std::cout << std::endl << str_lengthtest2 << std::endl;
    std::cout << "size_byte: " << str_lengthtest2.size_byte() << " length: " << str_lengthtest2.length() << std::endl;


    // operator+() concatenation tests
    utf_str::String concat_1{"test"};
    utf_str::String concat_2{" weee"};
    utf_str::String concat_3 = concat_1 + concat_2;
    concat_3 = concat_3 + utf_str::String{" wooo"};
    std::cout << "\nConcatenation test of test + weee + wooo: " << concat_3 << std::endl;


    // front() and back() tests
    utf_str::String str_frontback{"利прошлых выходных я решил斯"};
    std::cout << std::endl << str_frontback << std::endl;
    std::cout << "front(), expected '利': " << str_frontback.front() << std::endl;
    std::cout << "back(), expected '斯': " << str_frontback.back() << std::endl;


    // Empty and clear test
    std::cout << std::endl << "str_frontback: " << str_frontback << std::endl;
    std::cout << "empty: " << str_frontback.empty() << std::endl;
    std::cout << "clearing..." << std::endl;
    str_frontback.clear();
    std::cout << "str_frontback: " << str_frontback << std::endl;
    std::cout << "empty: " << str_frontback.empty() << std::endl;

    std::cout << std::endl << std::endl;


    utf_str::String hello{"你好"};
    std::cout << hello << std::endl;

    hello.push_back("!");
    hello.push_back("!");
    hello.push_back("!");
    std::cout << hello << std::endl;

    hello.erase(0, 2);
    std::cout << hello << std::endl;

    std::cout << std::endl << std::endl;

    utf_str::String words_of_wisdom{"利纳克斯"};
    std::cout << words_of_wisdom.c_str();

    return 0;
}