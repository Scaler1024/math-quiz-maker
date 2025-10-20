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





// ������
int main(int argc, char* argv[]) {
    CommandLineParser parser;
    parser.parse(argc, argv);


    // ������Ŀģʽ
    if (parser.has("-n")) {
        int count = stoi(parser.get("-n"));

        if (count <= 0 || count > 10000) {
            cout << "������Ŀ����������1��10000֮��" << endl;
            return 1;
        }
        // ���-r�����Ƿ��ṩ
        if (!parser.has("-r")) {
            cout << "���󣺱���ʹ�� -r ����ָ����ֵ��Χ" << endl;
            cout << "�÷���" << endl;
            cout << "������Ŀ: " << argv[0] << " -n <��Ŀ����> -r <��ֵ��Χ>" << endl;
            cout << "���Ĵ�: " << argv[0] << " -e <��Ŀ�ļ�> -a <���ļ�>" << endl;
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
                cout << "���棺�޷����ɸ��಻�ظ�����Ŀ�������� " << exercises.size() << " ����Ŀ" << endl;
                break;
            }
        }

        // ������Ŀ�ʹ�
        FileProcessor::saveExercises(exercises, "Exercises.txt");//��һ��
        FileProcessor::saveAnswers(answers, "Answers.txt");

        cout << "������ " << exercises.size() << " ����Ŀ�� "<< argv[0] << "\Exercises.txt" << endl;
        cout << "���ѱ��浽 " << argv[0] << "\Answers.txt" << endl;
    }
    // ���Ĵ�ģʽ
    else if (parser.has("-e") && parser.has("-a")) {
        string exerciseFile = parser.get("-e");
        string answerFile = parser.get("-a");

        FileProcessor::gradeAnswers(exerciseFile, answerFile, "Grade.txt");
        cout << "���Ľ���ѱ��浽 Grade.txt" << endl;
    }
    else {
        cout << "���󣺲���������" << endl;
        cout << "�÷���" << endl;
        cout << "������Ŀ: " << argv[0] << " -n <��Ŀ����> -r <��ֵ��Χ>" << endl;
        cout << "���Ĵ�: " << argv[0] << " -e <��Ŀ�ļ�> -a <���ļ�>" << endl;
        return 1;
    }

    return 0;
}