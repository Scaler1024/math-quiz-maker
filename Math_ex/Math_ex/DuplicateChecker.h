#pragma once
#include <iostream>
#include <string>
#include <set>
#include "Expression.h"

using namespace std;

class DuplicateChecker {
private:
    // 存储已生成的标准化表达式字符串，用于快速查重
    set<string> generatedExpressions;

    // 标准化表达式为唯一字符串表示
    string standardizeExpression(const Expression& expr) {
        
        // 调用Expression类的标准化方法获取规范化表达式
        Expression normalized = expr.standardize();
        //将标准话的字符串转换为字符串
        return normalized.toString();
    }

public:
    bool isDuplicate(const Expression& expr) {
        //先将表达式转换为标准字符串
        string standardized = standardizeExpression(expr);
        //检查该字符串是否已经在集合中存在
        if (generatedExpressions.find(standardized) != generatedExpressions.end()) {
            return true;
        }
        generatedExpressions.insert(standardized);
        return false;
    }

    void clear() {
        generatedExpressions.clear();//清空已经储存的所有表达式记录
    }
};