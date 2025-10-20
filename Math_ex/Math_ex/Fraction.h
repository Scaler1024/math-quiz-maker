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

// ������
class Fraction {
private:
    int numerator;   // ����
    int denominator; // ��ĸ

    // �����Լ��
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    // �������
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
            denominator = 1; // ��ֹ�������
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        simplify();
    }

    // ���ַ����������
    Fraction(const string& str) {
        size_t apostrophe = str.find("'");
        size_t slash = str.find("/");

        if (apostrophe != string::npos) {
            // ��������ʽ: a'b/c
            int whole = stoi(str.substr(0, apostrophe));
            int num = stoi(str.substr(apostrophe + 1, slash - apostrophe - 1));
            int den = stoi(str.substr(slash + 1));

            numerator = whole * den + (whole >= 0 ? num : -num);
            denominator = den;
        }
        else if (slash != string::npos) {
            // �������ʽ: a/b
            numerator = stoi(str.substr(0, slash));
            denominator = stoi(str.substr(slash + 1));
        }
        else {
            // ������ʽ
            numerator = stoi(str);
            denominator = 1;
        }
        simplify();
    }

    // ת��Ϊ�ַ���
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

    // ������������
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

    // �Ƚ������
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

    // ��ȡ���ӷ�ĸ
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // �ж��Ƿ�Ϊ�Ǹ�����������֤��ĿҪ��
    bool isNonNegative() const {
        return numerator >= 0;
    }

    // �ж��Ƿ�Ϊ�����
    bool isProperFraction() const {
        return abs(numerator) < denominator;
    }
};
