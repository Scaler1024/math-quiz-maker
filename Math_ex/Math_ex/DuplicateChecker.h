#pragma once
#include <iostream>
#include <string>
#include <set>
#include "Expression.h"

using namespace std;

class DuplicateChecker {
private:
    // �洢�����ɵı�׼�����ʽ�ַ��������ڿ��ٲ���
    set<string> generatedExpressions;

    // ��׼�����ʽΪΨһ�ַ�����ʾ
    string standardizeExpression(const Expression& expr) {
        
        // ����Expression��ı�׼��������ȡ�淶�����ʽ
        Expression normalized = expr.standardize();
        //����׼�����ַ���ת��Ϊ�ַ���
        return normalized.toString();
    }

public:
    bool isDuplicate(const Expression& expr) {
        //�Ƚ����ʽת��Ϊ��׼�ַ���
        string standardized = standardizeExpression(expr);
        //�����ַ����Ƿ��Ѿ��ڼ����д���
        if (generatedExpressions.find(standardized) != generatedExpressions.end()) {
            return true;
        }
        generatedExpressions.insert(standardized);
        return false;
    }

    void clear() {
        generatedExpressions.clear();//����Ѿ���������б��ʽ��¼
    }
};