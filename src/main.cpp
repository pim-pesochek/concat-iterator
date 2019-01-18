#include <vector>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include "concat.hpp"

void test_concat_with_vector() {
    std::vector<int> v1 = { 1, 2, 3, 4, 5 };
    std::vector<int> v2 = { 6, 7, 8, 9, 10 };
    std::vector<int> v3 = { 11, 12, 13, 14 };

    int value = 1;

    for (auto &i : concat(v1, v2, v3)) {
        if (i != value) {
            throw std::domain_error("wrong value");
        }

        ++value;
    }
}

void test_concat_with_initialized_list() {
    auto v1 = { 2, 6, 18};
    auto v2 = { 54, 162 };

    int value = 2;

    for (auto &i : concat(v1, v2)) {
        if (i != value) {
            throw std::domain_error("wrong value");
        }

        value *= 3;
    }
}

void test_concat_with_map() {
    std::unordered_map<int, char> v1 = {
            {1, '1'},
            {2, '2'},
            {3, '3'}
    };

    std::unordered_map<int, char> v2 = {
            {4, '4'},
            {5, '5'},
            {6, '6'},
            {7, '7'}
    };

    std::unordered_map<int, char> result;

    for (auto &i : concat(v1, v2)) {
        result.emplace(i.first, i.second);
    }

    if (result.size() != v1.size() + v2.size()) {
        throw std::domain_error("result map contains wrong number of items");
    }

    int value = 1;
    char ch = '1';

    for (size_t i = 0; i != result.size(); ++i) {
        auto it = result.find(value);

        if (it == result.end()) {
            throw std::domain_error("result map does not contain source data");
        }

        if (it->second != ch) {
            throw std::domain_error("result map contains wrong data");
        }

        ++value;
        ++ch;
    }
}

void test_concat_with_const_vector() {
    const std::vector<int> v1 = {1, 2, 3};
    const std::vector<int> v2 = {4, 5, 6, 7};

    int value = 1;
    for (auto &i : concat(v1, v2)) {
        if (i != value) {
            throw std::domain_error("wrong value");
        }

        ++value;
    }
}

int main() {
    test_concat_with_vector();
    test_concat_with_initialized_list();
    test_concat_with_map();
    test_concat_with_const_vector();
}