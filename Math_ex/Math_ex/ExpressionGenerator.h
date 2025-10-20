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
//// 标准化表达式（用于检测重复）表达式标准化还未完善
////表达式生成
//
//class ExpressionGenerator {
//private:
//    int maxRange;
//    int maxOperators;
//
//    // 随机生成分数
//    Fraction generateFraction() {
//        int denominator = rand() % (maxRange - 1) + 2; // 分母从2到maxRange-1
//        int numerator = rand() % denominator; // 分子小于分母，生成真分数
//        return Fraction(numerator, denominator);
//    }
//
//    // 随机生成数字（整数或分数）
//    ExpressionNode* generateNumber() {
//        // 50%概率生成整数，50%概率生成真分数
//        if (rand() % 2 == 0) {
//            int value = rand() % maxRange;
//            return new ExpressionNode(Fraction(value, 1));
//        }
//        else {
//            return new ExpressionNode(generateFraction());
//        }
//    }
//
//    // 随机生成运算符
//    char generateOperator() {
//        char operators[] = { '+', '-', '*', '/' };
//        return operators[rand() % 4];
//    }
//
//    // 构建表达式树
//    ExpressionNode* buildExpressionTree(int operatorCount) {
//        if (operatorCount == 0) {
//            return generateNumber();
//        }
//
//        // 随机决定左右子树的运算符数量
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
//    // 生成表达式
//    Expression generate() {
//        int operatorCount = rand() % maxOperators + 1; // 1到maxOperators个运算符
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
//        // 如果多次尝试失败，返回一个简单的表达式
//        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
//    }
//
//
//
//    // 标准化表达式（用于检测重复）
//    Expression normalize(const Expression& expr) {
//        // 实现表达式标准化，便于检测重复题目
//        // 这里需要实现表达式的交换律和结合律标准化
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

    // 生成随机分数
    Fraction generateFraction() {
        int denominator = rand() % (maxRange - 1) + 2;
        int numerator = rand() % denominator;
        return Fraction(numerator, denominator);
    }

    // 生成数字节点
    ExpressionNode* generateNumber() {
        if (rand() % 2 == 0) {
            int value = rand() % maxRange;
            return new ExpressionNode(Fraction(value, 1));
        }
        else {
            return new ExpressionNode(generateFraction());
        }
    }

    // 生成运算符
    char generateOperator() {
        char operators[] = { '+', '-', '*', '/' };
        return operators[rand() % 4];
    }

    // 构建表达式树
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

    // 生成表达式
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

    // 标准化表达式（用于去重）
    Expression normalize(const Expression& expr) {
        return expr.standardize();
    }
};