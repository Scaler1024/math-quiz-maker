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
////���ʽ��׼����δ����
//using namespace std;
//
//class DuplicateChecker {
//private:
//    set<string> generatedExpressions;
//
//    // ���ʽ��׼��������ɽ����Ĳ�����
//    string standardizeExpression(const Expression& expr) {
//        // ʵ�ֱ��ʽ�ı�׼����ʾ
//        // ���ɽ����Ĳ�������+�͡����Ĳ��������ֵ�������
//        // ����Ҫ�������ʽ������������
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

    // ��׼�����ʽΪΨһ�ַ�����ʾ
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