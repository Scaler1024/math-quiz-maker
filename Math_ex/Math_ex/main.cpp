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
using namespace std;





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
        FileProcessor::saveExercises(exercises, "Exercises.txt");//改一下
        FileProcessor::saveAnswers(answers, "Answers.txt");

        cout << "已生成 " << exercises.size() << " 道题目到 "<< argv[0] << "\Exercises.txt" << endl;
        cout << "答案已保存到 " << argv[0] << "\Answers.txt" << endl;
    }
    // 批改答案模式
    else if (parser.has("-e") && parser.has("-a")) {
        string exerciseFile = parser.get("-e");
        string answerFile = parser.get("-a");

        FileProcessor::gradeAnswers(exerciseFile, answerFile, "Grade.txt");
        cout << "批改结果已保存到 Grade.txt" << endl;
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