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
#include"Fraction.h"
#include"Expression.h"
#include"ExpressionGenerator.h"
#include"DuplicateChecker.h"
#include"FileProcessor.h"
#include"CommandLineParser.h"
#include <windows.h>
#include <shlwapi.h>  // 用于 PathRemoveFileSpecA 函数
#include <string>

using namespace std;
// 链接 shlwapi 库（VS 中可直接使用 #pragma，其他编译器需手动链接）
#pragma comment(lib, "shlwapi.lib")

// 获取可执行文件所在目录
string getExecutableDirectory() {
    char exePath[MAX_PATH];
    // 获取当前可执行文件完整路径
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    // 移除文件名，保留目录路径（例如从 "C:\test\app.exe" 得到 "C:\test"）
    PathRemoveFileSpecA(exePath);
    // 转换为字符串返回
    return string(exePath);
}

// 拼接目录和文件名（处理路径分隔符）
string combinePath(const string& dir, const string& filename) {
    if (dir.empty()) return filename;
    // 检查目录末尾是否已有分隔符，没有则添加
    char lastChar = dir.back();
    if (lastChar != '\\' && lastChar != '/') {
        return dir + "\\" + filename;  //Windows 下优先使用\
    
    }
        return dir + filename;
}



// 主程序
int main(int argc, char* argv[]) {
    CommandLineParser parser;
    parser.parse(argc, argv);


    // 生成题目模式
    if (parser.has("-n")) {
        int count = stoi(parser.get("-n"));

        if (count <= 0 || count > 10000) {
            cout << "错误：题目数量必须在1到10000之间" << endl;
            return 1;
        }
        // 检查-r参数是否提供
        if (!parser.has("-r")) {
            cout << "错误：必须使用 -r 参数指定数值范围" << endl;
            cout << "用法：" << endl;
            cout << "生成题目: " << argv[0] << " -n <题目数量> -r <数值范围>" << endl;
            cout << "批改答案: " << argv[0] << " -e <题目文件> -a <答案文件>" << endl;
            return 1;
        }

        int range = stoi(parser.get("-r"));


        ExpressionGenerator generator(range);
        DuplicateChecker checker;
        vector<Expression> exercises;
        vector<Fraction> answers;

        for (int i = 0; i < count; i++) {
            int attempts = 0;
            const int MAX_ATTEMPTS = 1000;

            while (attempts < MAX_ATTEMPTS) {
                Expression expr = generator.generate();

                if (!checker.isDuplicate(expr)) {
                    exercises.push_back(expr);
                    answers.push_back(expr.evaluate());
                    break;
                }

                attempts++;
            }

            if (attempts >= MAX_ATTEMPTS) {
                cout << "警告：无法生成更多不重复的题目，已生成 " << exercises.size() << " 道题目" << endl;
                break;
            }
        }

        // 保存题目和答案

        //获取可执行文件目录
        string execDir = getExecutableDirectory();
        // 拼接路径
        string exercisesPath = combinePath(execDir, "Exercises.txt");
        string answersPath = combinePath(execDir, "Answers.txt");
        FileProcessor::saveExercises(exercises,exercisesPath);
        FileProcessor::saveAnswers(answers, answersPath);

        cout << "已生成 " << exercises.size() << " 道题目到" << exercisesPath << endl;
        cout << "答案已保存到" << answersPath << endl;
    }
    // 批改答案模式
    else if (parser.has("-e") && parser.has("-a")) {
        string exerciseFile = parser.get("-e");
        string answerFile = parser.get("-a");


    // 获取可执行文件目录
    string execDir = getExecutableDirectory();
    string gradePath = combinePath(execDir, "Grade.txt");
    FileProcessor::gradeAnswers(exerciseFile, answerFile, gradePath);
        cout << "评分结果已保存到 " << gradePath << endl;
    }
    else {
        cout << "错误：参数不完整" << endl;
        cout << "用法：" << endl;
        cout << "生成题目: " << argv[0] << " -n <题目数量> -r <数值范围>" << endl;
        cout << "批改答案: " << argv[0] << " -e <题目文件> -a <答案文件>" << endl;
        return 1;
    }

    return 0;
}