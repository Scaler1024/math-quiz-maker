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
//�ļ���������������Ŀ�ı���ʹ𰸵�����

class FileProcessor {
public:
    // ������Ŀ���ļ�
    static void saveExercises(const vector<Expression>& exercises, const string& filename) {
        ofstream file(filename);
        for (size_t i = 0; i < exercises.size(); i++) {
            file << i + 1 << ". " << exercises[i].toString() << endl;
        }
        file.close();
    }

    // ����𰸵��ļ�
    static void saveAnswers(const vector<Fraction>& answers, const string& filename) {
        ofstream file(filename);
        for (size_t i = 0; i < answers.size(); i++) {
            file << i + 1 << ". " << answers[i].toString() << endl;
        }
        file.close();
    }




    static void gradeAnswers(const string& exerciseFile, const string& answerFile, const string& gradeFile) {
        ifstream exFile(exerciseFile);
        ifstream ansFile(answerFile);
        ofstream grdFile(gradeFile);

        // ����ļ��Ƿ�ɹ��򿪣�������һ�ļ���ʧ�������������Ϣ������
        if (!exFile.is_open() || !ansFile.is_open() || !grdFile.is_open()) {
            cerr << "�޷����ļ��������ļ�·���Ƿ���ȷ" << endl;
            return;
        }

        vector<int> correct, wrong; // �洢��ȷ��������Ŀ������б�
        string exLine, ansLine; // ���ڶ�ȡ��ϰ���ļ���һ������  // ���ڶ�ȡ���ļ���һ������
        int questionNum = 0;    // ��Ŀ����������¼��ǰ�������ţ�

        // �����������������ʽ�ַ���ΪExpression����
        auto parseExpressionStr = [](const string& exprStr) -> Expression {
            vector<string> tokens;
            int n = exprStr.size();
            int i = 0;

            // �ʷ����������token
            while (i < n) {
                if (isspace(exprStr[i])) {
                    i++;
                    continue;
                } // ��������������ţ�+��-��*��/��(��)��
                if (exprStr[i] == '+' || exprStr[i] == '-' ||
                    exprStr[i] == '*' || exprStr[i] == '/' ||
                    exprStr[i] == '(' || exprStr[i] == ')') {
                    tokens.push_back(string(1, exprStr[i]));
                    i++;
                }// �������ֺͷ�����֧��������������"3/4"����������"1'2/3"��
                else if (isdigit(exprStr[i]) || exprStr[i] == '\'') {
                    int j = i;
                    while (j < n && (isdigit(exprStr[j]) || exprStr[j] == '\'' || exprStr[j] == '/')) {
                        j++;
                    }
                    tokens.push_back(exprStr.substr(i, j - i));
                    i = j;
                }
                else {
                    i++;
                }
            }

            // �ݹ��½�������
           // �����������������ӣ����֡����Ű����ı��ʽ��
            function<ExpressionNode* (int&)> parseFactor;
            // ��������Ӽ�ĳ˳����㣩
            function<ExpressionNode* (int&)> parseTerm;
            // �������ʽ�����ļӼ����㣩
            function<ExpressionNode* (int&)> parseExpression;


            // �������ӣ��������֡������������������Ű����ı��ʽ
            parseFactor = [&](int& index) -> ExpressionNode* {
                if (index >= tokens.size()) return nullptr;
                string token = tokens[index];
                // ����ǰtoken�������ţ���ݹ���������ڵı��ʽ
                if (token == "(") {
                    index++;
                    auto node = parseExpression(index);
                    if (index < tokens.size() && tokens[index] == ")") {
                        index++;
                    }
                    return node;
                }
                else {// ������Ϊ����/������ת��ΪFraction���󲢴���Ҷ�ӽڵ�
                    Fraction f(token);
                    index++;
                    return new ExpressionNode(f);
                }
                };
            // ������������Ӽ�ĳ˷���*���ͳ�����/�����㣨���ȼ����ڼӼ���
            parseTerm = [&](int& index) -> ExpressionNode* {
                auto node = parseFactor(index);
                while (index < tokens.size()) {
                    string op = tokens[index];
                    if (op == "*" || op == "/") {
                        index++;
                        auto right = parseFactor(index);
                        node = new ExpressionNode(op[0], node, right);
                    }
                    else {
                        break;
                    }
                }
                return node;
                };

            // �������ʽ���������ļӷ���+���ͼ�����-�����㣨���ȼ����ڳ˳���
            parseExpression = [&](int& index) -> ExpressionNode* {
                auto node = parseTerm(index);
                while (index < tokens.size()) {
                    string op = tokens[index];
                    if (op == "+" || op == "-") {
                        index++;
                        auto right = parseTerm(index);
                        node = new ExpressionNode(op[0], node, right);
                    }
                    else {
                        break;
                    }
                }
                return node;
                };

            int index = 0;
            ExpressionNode* root = parseExpression(index);
            return Expression(root);
            };

        // ���д�����ϰ��ʹ�
        while (getline(exFile, exLine) && getline(ansFile, ansLine)) {
            questionNum++;

            // ������ϰ���еı��ʽ
            size_t dotPosEx = exLine.find('.');
            size_t eqPos = exLine.find(" = ");
            if (dotPosEx == string::npos || eqPos == string::npos || dotPosEx >= eqPos) {
                wrong.push_back(questionNum);
                continue;
            }

            string exprStr = exLine.substr(dotPosEx + 2, eqPos - dotPosEx - 2);
            Expression expr;
            try {// ���Խ������ʽ�ַ���
                expr = parseExpressionStr(exprStr);
            }
            catch (...) {
                wrong.push_back(questionNum);
                continue;
            }
            Fraction correctAns = expr.evaluate();

            // �����û���
            size_t dotPosAns = ansLine.find('.');
            if (dotPosAns == string::npos) {
                wrong.push_back(questionNum);
                continue;
            }
            string ansStr = ansLine.substr(dotPosAns + 2);
            Fraction userAns;
            try {
                userAns = Fraction(ansStr);
            }
            catch (...) {
                wrong.push_back(questionNum);
                continue;
            }

            // �Աȴ�
            if (userAns == correctAns) {
                correct.push_back(questionNum);
            }
            else {
                wrong.push_back(questionNum);
            }
        }

        // �����ļ�������ƥ������
        while (getline(exFile, exLine)) {
            questionNum++;
            wrong.push_back(questionNum);
        }
        while (getline(ansFile, ansLine)) {
            questionNum++;
            wrong.push_back(questionNum);
        }

        // д�����ֽ��
        grdFile << "Correct: " << correct.size() << " (";
        for (size_t i = 0; i < correct.size(); i++) {
            grdFile << correct[i];
            if (i < correct.size() - 1) grdFile << ", ";
        }
        grdFile << ")" << endl;

        grdFile << "Wrong: " << wrong.size() << " (";
        for (size_t i = 0; i < wrong.size(); i++) {
            grdFile << wrong[i];
            if (i < wrong.size() - 1) grdFile << ", ";
        }
        grdFile << ")" << endl;
        // �ر����д򿪵��ļ�
        exFile.close();
        ansFile.close();
        grdFile.close();
    }
    //// ���Ĵ�
    // 
    //static void gradeAnswers(const string& exerciseFile, const string& answerFile, const string& gradeFile) {
    //    ifstream exFile(exerciseFile);
    //    ifstream ansFile(answerFile);
    //    ofstream grdFile(gradeFile);

    //    vector<int> correct, wrong;
    //    string exLine, ansLine;
    //    int questionNum = 0;

    //    while (getline(exFile, exLine) && getline(ansFile, ansLine)) {
    //        questionNum++;

    //        // ��ȡ�𰸲��֣��򻯴���ʵ����Ҫ�����ӵĽ�����
    //        size_t dotPos = ansLine.find('.');
    //        if (dotPos != string::npos) {
    //            string ansStr = ansLine.substr(dotPos + 2); // ������źͿո�

    //            // ������Ҫʵ�ִ���֤�߼�
    //            correct.push_back(questionNum);
    //        }
    //    }

    //    // ���ͳ�ƽ��
    //    grdFile << "Correct: " << correct.size() << " (";
    //    for (size_t i = 0; i < correct.size(); i++) {
    //        grdFile << correct[i];
    //        if (i < correct.size() - 1) grdFile << ", ";
    //    }
    //    grdFile << ")" << endl;

    //    grdFile << "Wrong: " << wrong.size() << " (";
    //    for (size_t i = 0; i < wrong.size(); i++) {
    //        grdFile << wrong[i];
    //        if (i < wrong.size() - 1) grdFile << ", ";
    //    }
    //    grdFile << ")" << endl;

    //    exFile.close();
    //    ansFile.close();
    //    grdFile.close();
    //}
};
