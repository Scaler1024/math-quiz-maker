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
//���ʽ��׼����δ����
using namespace std;

class DuplicateChecker {
private:
    set<string> generatedExpressions;

    // ���ʽ��׼��������ɽ����Ĳ�����
    string standardizeExpression(const Expression& expr) {
        // ʵ�ֱ��ʽ�ı�׼����ʾ
        // ���ɽ����Ĳ�������+�͡����Ĳ��������ֵ�������
        // ����Ҫ�������ʽ������������

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
////��Ŀ�ظ��Լ��,����������Ŀ���ظ�Ҫ��Ĺؼ�����
//class DuplicateChecker {
//private:
//    std::set<std::string> generatedExpressions;
//
//    // ���ķ�������ȡ���ʽ�Ĺ淶���ַ�����ʾ
//    std::string getNormalizedExpression(const Expression& expr) {
//        return normalizeExpressionTree(expr.getRoot());
//    }
//
//    // �ݹ�淶�����ʽ���������㷨��
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
//        // �ؼ����Կɽ����Ĳ�������+�͡����Ĳ�������������
//        if (node->op == '+' || node->op == '*') {
//            if (leftStr > rightStr) {
//                std::swap(leftStr, rightStr);
//            }
//        }
//
//        return "(" + leftStr + node->op + rightStr + ")";
//    }
//
//    // ����������ʽ�Ƿ���ѧ�ȼ�
//    bool areExpressionsEquivalent(const Expression& expr1, const Expression& expr2) {
//        // 1. �ȱȽϹ淶���ַ���
//        std::string norm1 = getNormalizedExpression(expr1);
//        std::string norm2 = getNormalizedExpression(expr2);
//
//        if (norm1 == norm2) {
//            return true;
//        }
//
//        // 2. ����淶���ַ�����ͬ������������ͬ����һ�����
//        if (expr1.evaluate() == expr2.evaluate()) {
//            // ������������Ƿ���ͬ
//            return haveSameOperatorCombination(expr1, expr2);
//        }
//
//        return false;
//    }
//
//    // ������������Ƿ���ͬ
//    bool haveSameOperatorCombination(const Expression& expr1, const Expression& expr2) {
//        std::vector<char> ops1 = collectAllOperators(expr1.getRoot());
//        std::vector<char> ops2 = collectAllOperators(expr2.getRoot());
//
//        if (ops1.size() != ops2.size()) {
//            return false;
//        }
//
//        // �����Ƚ�
//        std::sort(ops1.begin(), ops1.end());
//        std::sort(ops2.begin(), ops2.end());
//
//        return ops1 == ops2;
//    }
//
//    // �ռ����������
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
//    // ��Ҫ���ظ���ⷽ��
//    bool isDuplicate(const Expression& expr, const std::vector<Expression>& existingExpressions) {
//        // ���ټ�飺�淶���ַ���ƥ��
//        std::string normalized = getNormalizedExpression(expr);
//        if (generatedExpressions.find(normalized) != generatedExpressions.end()) {
//            return true;
//        }
//
//        // ��ϸ��飺��ÿ�����б��ʽ�Ƚ�
//        for (const auto& existingExpr : existingExpressions) {
//            if (areExpressionsEquivalent(expr, existingExpr)) {
//                return true;
//            }
//        }
//
//        // �����ظ��ģ���ӵ������ɼ���
//        generatedExpressions.insert(normalized);
//        return false;
//    }
//
//    void clear() {
//        generatedExpressions.clear();
//    }
//};
//
