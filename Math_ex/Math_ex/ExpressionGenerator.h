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
        const int MAX_ATTEMPTS = 100;//最大尝试次数

        while (attempts < MAX_ATTEMPTS) {
            ExpressionNode* root = buildExpressionTree(operatorCount);
            expr.setRoot(root);

            if (expr.isValid(maxRange)) {
                return expr;
            }

            attempts++;
        }
        //如果多次失败则返回一个简单的表达式
        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
    }

    // 标准化表达式（用于去重）
    Expression normalize(const Expression& expr) {
        return expr.standardize();
    }
};