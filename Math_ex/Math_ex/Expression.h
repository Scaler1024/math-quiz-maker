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
// 表达式节点类型
enum NodeType { NUMBER, OPERATOR };

// 表达式树节点
struct ExpressionNode {
    NodeType type;
    Fraction value;
    char op;
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(Fraction val) : type(NUMBER), value(val), op(0), left(nullptr), right(nullptr) {}
    ExpressionNode(char operation, ExpressionNode* l, ExpressionNode* r) :
        type(OPERATOR), value(0), op(operation), left(l), right(r) {}
};

// 表达式类
class Expression {
private:
    ExpressionNode* root;

    // 克隆子树
    ExpressionNode* clone(ExpressionNode* node) {
        if (!node) return nullptr;
        if (node->type == NUMBER) {
            return new ExpressionNode(node->value);
        }
        return new ExpressionNode(node->op, clone(node->left), clone(node->right));
    }

    // 删除子树
    void clear(ExpressionNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // 计算表达式树
    Fraction evaluate(ExpressionNode* node) const {
        if (node->type == NUMBER) {
            return node->value;
        }

        Fraction leftVal = evaluate(node->left);
        Fraction rightVal = evaluate(node->right);

        switch (node->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return leftVal / rightVal;
        default: return Fraction(0, 1);
        }
    }

    // 生成字符串表示
    string toString(ExpressionNode* node, bool needParentheses = false) const {
        if (node->type == NUMBER) {
            return node->value.toString();
        }

        string leftStr = toString(node->left, node->op == '*' || node->op == '/');
        string rightStr = toString(node->right, true);

        string result;
        if (needParentheses) {
            result = "(" + leftStr + " " + string(1, node->op) + " " + rightStr + ")";
        }
        else {
            result = leftStr + " " + string(1, node->op) + " " + rightStr;
        }

        return result;
    }
    /////
private:
    // 标准化表达式节点
    ExpressionNode* standardizeNode(ExpressionNode* node) const {
        if (!node) return nullptr;

        if (node->type == NUMBER) {
            return new ExpressionNode(node->value);
        }

        if (node->op == '+') {
            vector<ExpressionNode*> operands;
            collectAddOperands(node, operands);

            sort(operands.begin(), operands.end(), [this](ExpressionNode* a, ExpressionNode* b) {
                return this->nodeToString(a) < this->nodeToString(b);
                });

            ExpressionNode* root = operands[0];
            for (size_t i = 1; i < operands.size(); ++i) {
                root = new ExpressionNode('+', root, operands[i]);
            }
            return root;
        }
        else if (node->op == '*') {
            vector<ExpressionNode*> operands;
            collectMulOperands(node, operands);

            sort(operands.begin(), operands.end(), [this](ExpressionNode* a, ExpressionNode* b) {
                return this->nodeToString(a) < this->nodeToString(b);
                });

            ExpressionNode* root = operands[0];
            for (size_t i = 1; i < operands.size(); ++i) {
                root = new ExpressionNode('*', root, operands[i]);
            }
            return root;
        }
        else {
            ExpressionNode* left = standardizeNode(node->left);
            ExpressionNode* right = standardizeNode(node->right);
            return new ExpressionNode(node->op, left, right);
        }
    }

    // 收集加法操作数
    void collectAddOperands(ExpressionNode* node, vector<ExpressionNode*>& operands) const {
        if (!node) return;

        if (node->type == NUMBER) {
            operands.push_back(new ExpressionNode(node->value));
            return;
        }

        if (node->op == '+') {
            collectAddOperands(node->left, operands);
            collectAddOperands(node->right, operands);
        }
        else {
            ExpressionNode* stdNode = standardizeNode(node);
            operands.push_back(stdNode);
        }
    }

    // 收集乘法操作数
    void collectMulOperands(ExpressionNode* node, vector<ExpressionNode*>& operands) const {
        if (!node) return;

        if (node->type == NUMBER) {
            operands.push_back(new ExpressionNode(node->value));
            return;
        }

        if (node->op == '*') {
            collectMulOperands(node->left, operands);
            collectMulOperands(node->right, operands);
        }
        else {
            ExpressionNode* stdNode = standardizeNode(node);
            operands.push_back(stdNode);
        }
    }

    // 节点转字符串（用于排序）
    string nodeToString(ExpressionNode* node) const {
        if (!node) return "";

        if (node->type == NUMBER) {
            return node->value.toString();
        }

        string leftStr = nodeToString(node->left);
        string rightStr = nodeToString(node->right);

        if (node->op == '+' || node->op == '*') {
            return "(" + min(leftStr, rightStr) + string(1, node->op) + max(leftStr, rightStr) + ")";
        }
        else {
            return "(" + leftStr + string(1, node->op) + rightStr + ")";
        }
    }

    /////

public:
    Expression() : root(nullptr) {}
    Expression(ExpressionNode* r) : root(r) {}
    Expression(const Expression& other) : root(clone(other.root)) {}

    Expression& operator=(const Expression& other) {
        if (this != &other) {
            clear(root);
            root = clone(other.root);
        }
        return *this;
    }

    ~Expression() {
        clear(root);
    }

    // 设置根节点
    void setRoot(ExpressionNode* r) {
        clear(root);
        root = r;
    }

    // 计算表达式值
    Fraction evaluate() const {
        if (!root) return Fraction(0, 1);
        return evaluate(root);
    }

    // 获取字符串表示
    string toString() const {
        if (!root) return "";
        return toString(root, false) + " = ";
    }

    // 获取表达式树根节点
    ExpressionNode* getRoot() const { return root; }

    // 判断表达式是否有效（符合题目要求）
    bool isValid(int maxRange) const {
        // 检查数值范围
        function<bool(ExpressionNode*)> checkRange = [&](ExpressionNode* node) {
            if (node->type == NUMBER) {
                Fraction val = node->value;
                if (val.getNumerator() < 0 || val.getNumerator() >= maxRange ||
                    val.getDenominator() <= 0 || val.getDenominator() >= maxRange) {
                    return false;
                }
                return true;
            }
            return checkRange(node->left) && checkRange(node->right);
            };

        if (!checkRange(root)) return false;

        // 检查减法结果非负
        function<bool(ExpressionNode*)> checkNonNegative = [&](ExpressionNode* node) {
            if (node->type == OPERATOR && node->op == '-') {
                Fraction leftVal = evaluate(node->left);
                Fraction rightVal = evaluate(node->right);
                if (leftVal < rightVal) {
                    return false;
                }
            }

            if (node->left && !checkNonNegative(node->left)) return false;
            if (node->right && !checkNonNegative(node->right)) return false;
            return true;
            };

        if (!checkNonNegative(root)) return false;

        // 检查除法结果为真分数
        function<bool(ExpressionNode*)> checkDivision = [&](ExpressionNode* node) {
            if (node->type == OPERATOR && node->op == '/') {
                Fraction result = evaluate(node);
                if (!result.isProperFraction()) {
                    return false;
                }
            }

            if (node->left && !checkDivision(node->left)) return false;
            if (node->right && !checkDivision(node->right)) return false;
            return true;
            };

        if (!checkDivision(root)) return false;

        return true;
    }
    // 获取标准化表达式
    Expression standardize() const {
        ExpressionNode* stdRoot = standardizeNode(root);
        return Expression(stdRoot);
    }

};
