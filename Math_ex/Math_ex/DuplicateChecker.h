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
//表达式标准化还未完善
using namespace std;

class DuplicateChecker {
private:
    set<string> generatedExpressions;

    // 表达式标准化：排序可交换的操作数
    string standardizeExpression(const Expression& expr) {
        // 实现表达式的标准化表示
        // 将可交换的操作符（+和×）的操作数按字典序排序
        // 这需要遍历表达式树并重新排列

        return expr.toString();
    }

public:
    bool isDuplicate(const Expression& expr) {
        string standardized = standardizeExpression(expr);
        if (generatedExpressions.find(standardized) != generatedExpressions.end()) {
            return true;
        }
        generatedExpressions.insert(standardized);
        return false;
    }

    void clear() {
        generatedExpressions.clear();
    }
};


//
////题目重复性检测,这是满足题目不重复要求的关键部分
//class DuplicateChecker {
//private:
//    std::set<std::string> generatedExpressions;
//
//    // 核心方法：获取表达式的规范化字符串表示
//    std::string getNormalizedExpression(const Expression& expr) {
//        return normalizeExpressionTree(expr.getRoot());
//    }
//
//    // 递归规范化表达式树（核心算法）
//    std::string normalizeExpressionTree(ExpressionNode* node) {
//        if (node == nullptr) return "";
//
//        if (node->type == NUMBER) {
//            return node->value.toString();
//        }
//
//        std::string leftStr = normalizeExpressionTree(node->left);
//        std::string rightStr = normalizeExpressionTree(node->right);
//
//        // 关键：对可交换的操作符（+和×）的操作数进行排序
//        if (node->op == '+' || node->op == '*') {
//            if (leftStr > rightStr) {
//                std::swap(leftStr, rightStr);
//            }
//        }
//
//        return "(" + leftStr + node->op + rightStr + ")";
//    }
//
//    // 检查两个表达式是否数学等价
//    bool areExpressionsEquivalent(const Expression& expr1, const Expression& expr2) {
//        // 1. 先比较规范化字符串
//        std::string norm1 = getNormalizedExpression(expr1);
//        std::string norm2 = getNormalizedExpression(expr2);
//
//        if (norm1 == norm2) {
//            return true;
//        }
//
//        // 2. 如果规范化字符串不同，但计算结果相同，进一步检查
//        if (expr1.evaluate() == expr2.evaluate()) {
//            // 检查运算符组合是否相同
//            return haveSameOperatorCombination(expr1, expr2);
//        }
//
//        return false;
//    }
//
//    // 检查运算符组合是否相同
//    bool haveSameOperatorCombination(const Expression& expr1, const Expression& expr2) {
//        std::vector<char> ops1 = collectAllOperators(expr1.getRoot());
//        std::vector<char> ops2 = collectAllOperators(expr2.getRoot());
//
//        if (ops1.size() != ops2.size()) {
//            return false;
//        }
//
//        // 排序后比较
//        std::sort(ops1.begin(), ops1.end());
//        std::sort(ops2.begin(), ops2.end());
//
//        return ops1 == ops2;
//    }
//
//    // 收集所有运算符
//    std::vector<char> collectAllOperators(ExpressionNode* node) {
//        std::vector<char> operators;
//        if (node == nullptr) return operators;
//
//        if (node->type == OPERATOR) {
//            operators.push_back(node->op);
//            auto leftOps = collectAllOperators(node->left);
//            auto rightOps = collectAllOperators(node->right);
//            operators.insert(operators.end(), leftOps.begin(), leftOps.end());
//            operators.insert(operators.end(), rightOps.begin(), rightOps.end());
//        }
//
//        return operators;
//    }
//
//public:
//    // 主要的重复检测方法
//    bool isDuplicate(const Expression& expr, const std::vector<Expression>& existingExpressions) {
//        // 快速检查：规范化字符串匹配
//        std::string normalized = getNormalizedExpression(expr);
//        if (generatedExpressions.find(normalized) != generatedExpressions.end()) {
//            return true;
//        }
//
//        // 详细检查：与每个已有表达式比较
//        for (const auto& existingExpr : existingExpressions) {
//            if (areExpressionsEquivalent(expr, existingExpr)) {
//                return true;
//            }
//        }
//
//        // 不是重复的，添加到已生成集合
//        generatedExpressions.insert(normalized);
//        return false;
//    }
//
//    void clear() {
//        generatedExpressions.clear();
//    }
//};
//
