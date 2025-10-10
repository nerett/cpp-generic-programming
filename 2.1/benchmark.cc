#include "benchmark.hh"
#include "cow_string.hh"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>


namespace {

class Timer {
public:
    Timer(const std::string& name) : m_name(name), m_start(std::chrono::high_resolution_clock::now()) {}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - m_start;
        std::cout << "[" << m_name << "] Duration: " << duration.count() << " ms\n";
    }
private:
    std::string m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

} // namespace

void demonstrate_cow_logic() {
    std::cout << "--- Simple demo (CoW logic) ---\n";
    nerett::CowString<char> s1 = "Hello, World!";
    std::cout << "s1: \"" << s1 << "\" (use_count: " << s1.use_count() << ")\n";

    nerett::CowString<char> s2 = s1;
    std::cout << "s2 = s1;\n";
    std::cout << "s1: \"" << s1 << "\" (use_count: " << s1.use_count() << ")\n";
    std::cout << "s2: \"" << s2 << "\" (use_count: " << s2.use_count() << ")\n\n";
    
    std::cout << "Modifying s2[7] = 'C';\n";
    s2[7] = 'C';
    
    std::cout << "s1: \"" << s1 << "\" (use_count: " << s1.use_count() << ")\n";
    std::cout << "s2: \"" << s2 << "\" (use_count: " << s2.use_count() << ")\n\n";
}

void benchmark_copying() {
    std::cout << "--- Benchmark (copying large string) ---\n";
    const int num_copies = 500;
    const int string_size = 2 * 1024 * 1024; // 2 MB

    std::string base_std_string(string_size, 'x');
    nerett::CowString<char> base_cow_string(base_std_string.c_str());

    {
        Timer t("std::string");
        std::vector<std::string> strings;
        strings.reserve(num_copies);
        for (int i = 0; i < num_copies; ++i) {
            strings.push_back(base_std_string);
        }
    }

    {
        Timer t("CowString");
        std::vector<nerett::CowString<char>> cow_strings;
        cow_strings.reserve(num_copies);
        for (int i = 0; i < num_copies; ++i) {
            cow_strings.push_back(base_cow_string);
        }
    }
}

void test_comparisons() {
    nerett::CowString s1 = "apple";
    nerett::CowString s2 = "apply";
    std::cout << "\n--- Comparisons ---\n";
    std::cout << "s1 ('apple') < s2 ('apply'): " << std::boolalpha << (s1 < s2) << std::endl;
    std::cout << "s1 ('apple') == s1: " << (s1 == s1) << std::endl;
}
