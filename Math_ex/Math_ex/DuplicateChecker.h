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
////表达式标准化还未完善
//using namespace std;
//
//class DuplicateChecker {
//private:
//    set<string> generatedExpressions;
//
//    // 表达式标准化：排序可交换的操作数
//    string standardizeExpression(const Expression& expr) {
//        // 实现表达式的标准化表示
//        // 将可交换的操作符（+和×）的操作数按字典序排序
//        // 这需要遍历表达式树并重新排列
//
//        return expr.toString();
//    }
//
//public:
//    bool isDuplicate(const Expression& expr) {
//        string standardized = standardizeExpression(expr);
//        if (generatedExpressions.find(standardized) != generatedExpressions.end()) {
//            return true;
//        }
//        generatedExpressions.insert(standardized);
//        return false;
//    }
//
//    void clear() {
//        generatedExpressions.clear();
//    }
//};

#pragma once
#include <iostream>
#include <string>
#include <set>
#include "Expression.h"

using namespace std;

class DuplicateChecker {
private:
    set<string> generatedExpressions;

    // 标准化表达式为唯一字符串表示
    string standardizeExpression(const Expression& expr) {
        Expression normalized = expr.standardize();
        return normalized.toString();
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