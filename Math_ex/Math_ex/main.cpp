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
#include <shlwapi.h>  // ���� PathRemoveFileSpecA ����
#include <string>

using namespace std;
// ���� shlwapi �⣨VS �п�ֱ��ʹ�� #pragma���������������ֶ����ӣ�
#pragma comment(lib, "shlwapi.lib")

// ��ȡ��ִ���ļ�����Ŀ¼
string getExecutableDirectory() {
    char exePath[MAX_PATH];
    // ��ȡ��ǰ��ִ���ļ�����·��
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    // �Ƴ��ļ���������Ŀ¼·��������� "C:\test\app.exe" �õ� "C:\test"��
    PathRemoveFileSpecA(exePath);
    // ת��Ϊ�ַ�������
    return string(exePath);
}

// ƴ��Ŀ¼���ļ���������·���ָ�����
string combinePath(const string& dir, const string& filename) {
    if (dir.empty()) return filename;
    // ���Ŀ¼ĩβ�Ƿ����зָ�����û�������
    char lastChar = dir.back();
    if (lastChar != '\\' && lastChar != '/') {
        return dir + "\\" + filename;  //Windows ������ʹ��\
    
    }
        return dir + filename;
}



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

        //��ȡ��ִ���ļ�Ŀ¼
        string execDir = getExecutableDirectory();
        // ƴ��·��
        string exercisesPath = combinePath(execDir, "Exercises.txt");
        string answersPath = combinePath(execDir, "Answers.txt");
        FileProcessor::saveExercises(exercises,exercisesPath);
        FileProcessor::saveAnswers(answers, answersPath);

        cout << "������ " << exercises.size() << " ����Ŀ��" << exercisesPath << endl;
        cout << "���ѱ��浽" << answersPath << endl;
    }
    // ���Ĵ�ģʽ
    else if (parser.has("-e") && parser.has("-a")) {
        string exerciseFile = parser.get("-e");
        string answerFile = parser.get("-a");


    // ��ȡ��ִ���ļ�Ŀ¼
    string execDir = getExecutableDirectory();
    string gradePath = combinePath(execDir, "Grade.txt");
    FileProcessor::gradeAnswers(exerciseFile, answerFile, gradePath);
        cout << "���ֽ���ѱ��浽 " << gradePath << endl;
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