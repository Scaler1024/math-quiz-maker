//#pragma once
//#include <iostream>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <set>
//#include <map>
//#include <fstream>
//#include <algorithm>
//#include <cstdlib>
//#include <ctime>
//#include <cmath>
//#include <functional>
//#include"Expression.h"
//#include"DuplicateChecker.h"
//
//using namespace std;
//
//// ��׼�����ʽ�����ڼ���ظ������ʽ��׼����δ����
////���ʽ����
//
//class ExpressionGenerator {
//private:
//    int maxRange;
//    int maxOperators;
//
//    // ������ɷ���
//    Fraction generateFraction() {
//        int denominator = rand() % (maxRange - 1) + 2; // ��ĸ��2��maxRange-1
//        int numerator = rand() % denominator; // ����С�ڷ�ĸ�����������
//        return Fraction(numerator, denominator);
//    }
//
//    // ����������֣������������
//    ExpressionNode* generateNumber() {
//        // 50%��������������50%�������������
//        if (rand() % 2 == 0) {
//            int value = rand() % maxRange;
//            return new ExpressionNode(Fraction(value, 1));
//        }
//        else {
//            return new ExpressionNode(generateFraction());
//        }
//    }
//
//    // ������������
//    char generateOperator() {
//        char operators[] = { '+', '-', '*', '/' };
//        return operators[rand() % 4];
//    }
//
//    // �������ʽ��
//    ExpressionNode* buildExpressionTree(int operatorCount) {
//        if (operatorCount == 0) {
//            return generateNumber();
//        }
//
//        // ��������������������������
//        int leftCount = rand() % operatorCount;
//        int rightCount = operatorCount - leftCount - 1;
//
//        ExpressionNode* left = buildExpressionTree(leftCount);
//        ExpressionNode* right = buildExpressionTree(rightCount);
//
//        return new ExpressionNode(generateOperator(), left, right);
//    }
//public:
//    ExpressionGenerator(int range, int maxOps = 3) : maxRange(range), maxOperators(maxOps) {
//        srand(time(nullptr));
//    }
//
//    // ���ɱ��ʽ
//    Expression generate() {
//        int operatorCount = rand() % maxOperators + 1; // 1��maxOperators�������
//
//        Expression expr;
//        int attempts = 0;
//        const int MAX_ATTEMPTS = 100;
//
//        while (attempts < MAX_ATTEMPTS) {
//            ExpressionNode* root = buildExpressionTree(operatorCount);
//            expr.setRoot(root);
//
//            if (expr.isValid(maxRange)) {
//                return expr;
//            }
//
//            attempts++;
//        }
//
//        // �����γ���ʧ�ܣ�����һ���򵥵ı��ʽ
//        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
//    }
//
//
//
//    // ��׼�����ʽ�����ڼ���ظ���
//    Expression normalize(const Expression& expr) {
//        // ʵ�ֱ��ʽ��׼�������ڼ���ظ���Ŀ
//        // ������Ҫʵ�ֱ��ʽ�Ľ����ɺͽ���ɱ�׼��
//
//
//        //return expr;
//    }
//};
//
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Expression.h"
#include "DuplicateChecker.h"

using namespace std;

class ExpressionGenerator {
private:
    int maxRange;
    int maxOperators;

    // �����������
    Fraction generateFraction() {
        int denominator = rand() % (maxRange - 1) + 2;
        int numerator = rand() % denominator;
        return Fraction(numerator, denominator);
    }

    // �������ֽڵ�
    ExpressionNode* generateNumber() {
        if (rand() % 2 == 0) {
            int value = rand() % maxRange;
            return new ExpressionNode(Fraction(value, 1));
        }
        else {
            return new ExpressionNode(generateFraction());
        }
    }

    // ���������
    char generateOperator() {
        char operators[] = { '+', '-', '*', '/' };
        return operators[rand() % 4];
    }

    // �������ʽ��
    ExpressionNode* buildExpressionTree(int operatorCount) {
        if (operatorCount == 0) {
            return generateNumber();
        }

        int leftCount = rand() % operatorCount;
        int rightCount = operatorCount - leftCount - 1;

        ExpressionNode* left = buildExpressionTree(leftCount);
        ExpressionNode* right = buildExpressionTree(rightCount);

        return new ExpressionNode(generateOperator(), left, right);
    }

public:
    ExpressionGenerator(int range, int maxOps = 3) : maxRange(range), maxOperators(maxOps) {
        srand(time(nullptr));
    }

    // ���ɱ��ʽ
    Expression generate() {
        int operatorCount = rand() % maxOperators + 1;

        Expression expr;
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;

        while (attempts < MAX_ATTEMPTS) {
            ExpressionNode* root = buildExpressionTree(operatorCount);
            expr.setRoot(root);

            if (expr.isValid(maxRange)) {
                return expr;
            }

            attempts++;
        }

        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
    }

    // ��׼�����ʽ������ȥ�أ�
    Expression normalize(const Expression& expr) {
        return expr.standardize();
    }
};