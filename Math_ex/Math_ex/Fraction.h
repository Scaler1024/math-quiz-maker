#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <functional>

using namespace std;

// 分数类
class Fraction {
private:
    int numerator;   // 分子
    int denominator; // 分母

    // 求最大公约数
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    // 化简分数
    void simplify() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }

        int g = gcd(abs(numerator), abs(denominator));
        if (g != 0) {
            numerator /= g;
            denominator /= g;
        }

        if (denominator == 0) {
            denominator = 1; // 防止除零错误
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        simplify();
    }

    // 从字符串构造分数
    Fraction(const string& str) {
        size_t apostrophe = str.find("'");
        size_t slash = str.find("/");

        if (apostrophe != string::npos) {
            // 带分数格式: a'b/c
            int whole = stoi(str.substr(0, apostrophe));
            int num = stoi(str.substr(apostrophe + 1, slash - apostrophe - 1));
            int den = stoi(str.substr(slash + 1));

            numerator = whole * den + (whole >= 0 ? num : -num);
            denominator = den;
        }
        else if (slash != string::npos) {
            // 真分数格式: a/b
            numerator = stoi(str.substr(0, slash));
            denominator = stoi(str.substr(slash + 1));
        }
        else {
            // 整数格式
            numerator = stoi(str);
            denominator = 1;
        }
        simplify();
    }

    // 转换为字符串
    string toString() const {
        if (denominator == 1) {
            return to_string(numerator);
        }

        if (abs(numerator) > denominator) {
            int whole = numerator / denominator;
            int num = abs(numerator) % denominator;
            return to_string(whole) + "'" + to_string(num) + "/" + to_string(denominator);
        }

        return to_string(numerator) + "/" + to_string(denominator);
    }

    // 四则运算重载
    Fraction operator+(const Fraction& other) const {
        return Fraction(numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }

    Fraction operator/(const Fraction& other) const {
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }

    // 比较运算符
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }

    bool operator>(const Fraction& other) const {
        return numerator * other.denominator > other.numerator * denominator;
    }

    bool operator<=(const Fraction& other) const {
        return !(*this > other);
    }

    // 获取分子分母
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // 判断是否为非负数（用于验证题目要求）
    bool isNonNegative() const {
        return numerator >= 0;
    }

    // 判断是否为真分数
    bool isProperFraction() const {
        return abs(numerator) < denominator;
    }
};
