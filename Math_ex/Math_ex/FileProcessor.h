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
//文件处理器，处理题目的保存和答案的批改

class FileProcessor {
public:
    // 保存题目到文件
    static void saveExercises(const vector<Expression>& exercises, const string& filename) {
        ofstream file(filename);
        for (size_t i = 0; i < exercises.size(); i++) {
            file << i + 1 << ". " << exercises[i].toString() << endl;
        }
        file.close();
    }

    // 保存答案到文件
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

        // 检查文件是否成功打开，若有任一文件打开失败则输出错误信息并返回
        if (!exFile.is_open() || !ansFile.is_open() || !grdFile.is_open()) {
            cerr << "无法打开文件，请检查文件路径是否正确" << endl;
            return;
        }

        vector<int> correct, wrong; // 存储正确、错误题目的题号列表
        string exLine, ansLine; // 用于读取练习题文件的一行内容  // 用于读取答案文件的一行内容
        int questionNum = 0;    // 题目计数器（记录当前处理的题号）

        // 辅助函数：解析表达式字符串为Expression对象
        auto parseExpressionStr = [](const string& exprStr) -> Expression {
            vector<string> tokens;
            int n = exprStr.size();
            int i = 0;

            // 词法分析：拆分token
            while (i < n) {
                if (isspace(exprStr[i])) {
                    i++;
                    continue;
                } // 处理运算符和括号（+、-、*、/、(、)）
                if (exprStr[i] == '+' || exprStr[i] == '-' ||
                    exprStr[i] == '*' || exprStr[i] == '/' ||
                    exprStr[i] == '(' || exprStr[i] == ')') {
                    tokens.push_back(string(1, exprStr[i]));
                    i++;
                }// 处理数字和分数（支持整数、分数如"3/4"、带分数如"1'2/3"）
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

            // 递归下降解析器
           // 函数声明：解析因子（数字、括号包裹的表达式）
            function<ExpressionNode* (int&)> parseFactor;
            // 解析项（因子间的乘除运算）
            function<ExpressionNode* (int&)> parseTerm;
            // 解析表达式（项间的加减运算）
            function<ExpressionNode* (int&)> parseExpression;


            // 解析因子：处理数字、带分数、分数或括号包裹的表达式
            parseFactor = [&](int& index) -> ExpressionNode* {
                if (index >= tokens.size()) return nullptr;
                string token = tokens[index];
                // 若当前token是左括号，则递归解析括号内的表达式
                if (token == "(") {
                    index++;
                    auto node = parseExpression(index);
                    if (index < tokens.size() && tokens[index] == ")") {
                        index++;
                    }
                    return node;
                }
                else {// 否则视为数字/分数，转换为Fraction对象并创建叶子节点
                    Fraction f(token);
                    index++;
                    return new ExpressionNode(f);
                }
                };
            // 解析项：处理因子间的乘法（*）和除法（/）运算（优先级高于加减）
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

            // 解析表达式：处理项间的加法（+）和减法（-）运算（优先级低于乘除）
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

        // 逐行处理练习题和答案
        while (getline(exFile, exLine) && getline(ansFile, ansLine)) {
            questionNum++;

            // 解析练习题中的表达式
            size_t dotPosEx = exLine.find('.');
            size_t eqPos = exLine.find(" = ");
            if (dotPosEx == string::npos || eqPos == string::npos || dotPosEx >= eqPos) {
                wrong.push_back(questionNum);
                continue;
            }

            string exprStr = exLine.substr(dotPosEx + 2, eqPos - dotPosEx - 2);
            Expression expr;
            try {// 尝试解析表达式字符串
                expr = parseExpressionStr(exprStr);
            }
            catch (...) {
                wrong.push_back(questionNum);
                continue;
            }
            Fraction correctAns = expr.evaluate();

            // 解析用户答案
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

            // 对比答案
            if (userAns == correctAns) {
                correct.push_back(questionNum);
            }
            else {
                wrong.push_back(questionNum);
            }
        }

        // 处理文件行数不匹配的情况
        while (getline(exFile, exLine)) {
            questionNum++;
            wrong.push_back(questionNum);
        }
        while (getline(ansFile, ansLine)) {
            questionNum++;
            wrong.push_back(questionNum);
        }

        // 写入评分结果
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
        // 关闭所有打开的文件
        exFile.close();
        ansFile.close();
        grdFile.close();
    }
    //// 批改答案
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

    //        // 提取答案部分（简化处理，实际需要更复杂的解析）
    //        size_t dotPos = ansLine.find('.');
    //        if (dotPos != string::npos) {
    //            string ansStr = ansLine.substr(dotPos + 2); // 跳过序号和空格

    //            // 这里需要实现答案验证逻辑
    //            correct.push_back(questionNum);
    //        }
    //    }

    //    // 输出统计结果
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
