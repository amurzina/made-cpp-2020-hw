#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

const double EPSILON = 1e-7;

namespace task {

    std::vector<double> operator+(const std::vector<double> &first, const std::vector<double> &second) {
        size_t size = std::max(first.size(), second.size());
        std::vector<double> result(size);

        std::transform(
                begin(first),
                begin(first) + first.size(),
                begin(second),
                begin(result),
                std::plus<double>()
        );

        return result;
    }

    std::vector<double> operator-(std::vector<double> first, std::vector<double> second) {
        size_t size = std::max(first.size(), second.size());
        std::vector<double> result(size);

        std::transform(
                begin(first),
                begin(first) + first.size(),
                begin(second),
                begin(result),
                std::minus<double>()
        );

        return result;
    }

    std::vector<double> operator+(const std::vector<double> &number) {
        return number;
    }

    std::vector<double> operator-(const std::vector<double> &number) {
        size_t size = number.size();
        std::vector<double> result;

        for (size_t i = 0; i < size; i++) {
            result.push_back(-number[i]);
        }

        return result;
    }

    double operator*(std::vector<double> first, std::vector<double> second) {
        double result = 0;
        size_t size = first.size();

        for (size_t i = 0; i < size; i++) {
            result += first[i] * second[i];
        }

        return result;
    }

    std::vector<double> operator%(std::vector<double> first, std::vector<double> second) {
        std::vector<double> result;

        result.push_back(first[1] * second[2] - first[2] * second[1]);
        result.push_back(first[2] * second[0] - first[0] * second[2]);
        result.push_back(first[0] * second[1] - first[1] * second[0]);

        return result;
    }

    bool operator||(std::vector<double> first, std::vector<double> second) {
        double d1 = 0., d2 = 0.;
        for (size_t i = 0; i < first.size(); ++i) {
            d1 += first[i] * first[i];
            d2 += second[i] * second[i];
        }
        d1 = sqrt(d1);
        d2 = sqrt(d2);
        if ((1. - (first * second) / (d1 * d2) < EPSILON) || (1. + (first * second) / (d1 * d2) < EPSILON))
            return true;
        return false;
    }

    bool operator&&(std::vector<double> first, std::vector<double> second) {
        return (first * second > 0) && (first || second);  // have collinearity and one direction
    }

    std::istream &operator>>(std::istream &in, std::vector<double> &number) {
        size_t size = 0;
        number.clear();

        in >> size;
        number.resize(size);

        for (size_t i = 0; i < size; i++) {
            in >> number[i];
        }

        return in;
    }

    std::ostream &operator<<(std::ostream &out, const std::vector<double> &number) {
        size_t size = number.size();

        for (size_t i = 0; i < size; i++) {
            out << number[i] << ' ';
        }
        out << '\n';

        return out;
    }

    void reverse(std::vector<double> &number) { // http://www.cplusplus.com/reference/algorithm/reverse/
        auto first = number.begin();
        auto last = number.end();

        while ((first != last) && (first != --last)) {
            std::iter_swap(first, last);
            ++first;
        }
    }

    std::vector<int> operator|(std::vector<int> first, std::vector<int> second) {
        std::vector<int> result;
        for (size_t i = 0; i < first.size(); ++i) {
            result.push_back(first[i] | second[i]);
        }
        return result;
    }

    std::vector<int> operator&(std::vector<int> first, std::vector<int> second) {
        std::vector<int> result;
        for (size_t i = 0; i < first.size(); ++i) {
            result.push_back(first[i] & second[i]);
        }
        return result;
    }

}  // namespace task
