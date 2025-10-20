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

// ������
class Fraction {
private:
    int numerator;   // ����
    int denominator; // ��ĸ

    // �����Լ��
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    // �������
    void simplify() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }

        int g = gcd(abs(numerator), abs(denominator));
        if (g != 0) {
            numerator /= g;
            denominator /= g;
        }

        if (denominator == 0) {
            denominator = 1; // ��ֹ�������
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        simplify();
    }

    // ���ַ����������
    Fraction(const string& str) {
        size_t apostrophe = str.find("'");
        size_t slash = str.find("/");

        if (apostrophe != string::npos) {
            // ��������ʽ: a'b/c
            int whole = stoi(str.substr(0, apostrophe));
            int num = stoi(str.substr(apostrophe + 1, slash - apostrophe - 1));
            int den = stoi(str.substr(slash + 1));

            numerator = whole * den + (whole >= 0 ? num : -num);
            denominator = den;
        }
        else if (slash != string::npos) {
            // �������ʽ: a/b
            numerator = stoi(str.substr(0, slash));
            denominator = stoi(str.substr(slash + 1));
        }
        else {
            // ������ʽ
            numerator = stoi(str);
            denominator = 1;
        }
        simplify();
    }

    // ת��Ϊ�ַ���
    string toString() const {
        if (denominator == 1) {
            return to_string(numerator);
        }

        if (abs(numerator) > denominator) {
            int whole = numerator / denominator;
            int num = abs(numerator) % denominator;
            return to_string(whole) + "'" + to_string(num) + "/" + to_string(denominator);
        }

        return to_string(numerator) + "/" + to_string(denominator);
    }

    // ������������
    Fraction operator+(const Fraction& other) const {
        return Fraction(numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }

    Fraction operator/(const Fraction& other) const {
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }

    // �Ƚ������
    bool operator==(const Fraction& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }

    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }

    bool operator>(const Fraction& other) const {
        return numerator * other.denominator > other.numerator * denominator;
    }

    bool operator<=(const Fraction& other) const {
        return !(*this > other);
    }

    // ��ȡ���ӷ�ĸ
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // �ж��Ƿ�Ϊ�Ǹ�����������֤��ĿҪ��
    bool isNonNegative() const {
        return numerator >= 0;
    }

    // �ж��Ƿ�Ϊ�����
    bool isProperFraction() const {
        return abs(numerator) < denominator;
    }
};


// ���ʽ�ڵ�����
enum NodeType { NUMBER, OPERATOR };

// ���ʽ���ڵ�
struct ExpressionNode {
    NodeType type;
    Fraction value;
    char op;
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(Fraction val) : type(NUMBER), value(val), op(0), left(nullptr), right(nullptr) {}
    ExpressionNode(char operation, ExpressionNode* l, ExpressionNode* r) :
        type(OPERATOR), value(0), op(operation), left(l), right(r) {}
};

// ���ʽ��
class Expression {
private:
    ExpressionNode* root;

    // ��¡����
    ExpressionNode* clone(ExpressionNode* node) {
        if (!node) return nullptr;
        if (node->type == NUMBER) {
            return new ExpressionNode(node->value);
        }
        return new ExpressionNode(node->op, clone(node->left), clone(node->right));
    }

    // ɾ������
    void clear(ExpressionNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // ������ʽ��
    Fraction evaluate(ExpressionNode* node) const {
        if (node->type == NUMBER) {
            return node->value;
        }

        Fraction leftVal = evaluate(node->left);
        Fraction rightVal = evaluate(node->right);

        switch (node->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return leftVal / rightVal;
        default: return Fraction(0, 1);
        }
    }

    // �����ַ�����ʾ
    string toString(ExpressionNode* node, bool needParentheses = false) const {
        if (node->type == NUMBER) {
            return node->value.toString();
        }

        string leftStr = toString(node->left, node->op == '*' || node->op == '/');
        string rightStr = toString(node->right, true);

        string result;
        if (needParentheses) {
            result = "(" + leftStr + " " + string(1, node->op) + " " + rightStr + ")";
        }
        else {
            result = leftStr + " " + string(1, node->op) + " " + rightStr;
        }

        return result;
    }

public:
    Expression() : root(nullptr) {}
    Expression(ExpressionNode* r) : root(r) {}
    Expression(const Expression& other) : root(clone(other.root)) {}

    Expression& operator=(const Expression& other) {
        if (this != &other) {
            clear(root);
            root = clone(other.root);
        }
        return *this;
    }

    ~Expression() {
        clear(root);
    }

    // ���ø��ڵ�
    void setRoot(ExpressionNode* r) {
        clear(root);
        root = r;
    }

    // ������ʽֵ
    Fraction evaluate() const {
        if (!root) return Fraction(0, 1);
        return evaluate(root);
    }

    // ��ȡ�ַ�����ʾ
    string toString() const {
        if (!root) return "";
        return toString(root, false) + " = ";
    }

    // ��ȡ���ʽ�����ڵ�
    ExpressionNode* getRoot() const { return root; }

    // �жϱ��ʽ�Ƿ���Ч��������ĿҪ��
    bool isValid(int maxRange) const {
        // �����ֵ��Χ
        function<bool(ExpressionNode*)> checkRange = [&](ExpressionNode* node) {
            if (node->type == NUMBER) {
                Fraction val = node->value;
                if (val.getNumerator() < 0 || val.getNumerator() >= maxRange ||
                    val.getDenominator() <= 0 || val.getDenominator() >= maxRange) {
                    return false;
                }
                return true;
            }
            return checkRange(node->left) && checkRange(node->right);
            };

        if (!checkRange(root)) return false;

        // ����������Ǹ�
        function<bool(ExpressionNode*)> checkNonNegative = [&](ExpressionNode* node) {
            if (node->type == OPERATOR && node->op == '-') {
                Fraction leftVal = evaluate(node->left);
                Fraction rightVal = evaluate(node->right);
                if (leftVal < rightVal) {
                    return false;
                }
            }

            if (node->left && !checkNonNegative(node->left)) return false;
            if (node->right && !checkNonNegative(node->right)) return false;
            return true;
            };

        if (!checkNonNegative(root)) return false;

        // ���������Ϊ�����
        function<bool(ExpressionNode*)> checkDivision = [&](ExpressionNode* node) {
            if (node->type == OPERATOR && node->op == '/') {
                Fraction result = evaluate(node);
                if (!result.isProperFraction()) {
                    return false;
                }
            }

            if (node->left && !checkDivision(node->left)) return false;
            if (node->right && !checkDivision(node->right)) return false;
            return true;
            };

        if (!checkDivision(root)) return false;

        return true;
    }
};

class ExpressionGenerator {
private:
    int maxRange;
    int maxOperators;

    // ������ɷ���
    Fraction generateFraction() {
        int denominator = rand() % (maxRange - 1) + 2; // ��ĸ��2��maxRange-1
        int numerator = rand() % denominator; // ����С�ڷ�ĸ�����������
        return Fraction(numerator, denominator);
    }

    // ����������֣������������
    ExpressionNode* generateNumber() {
        // 50%��������������50%�������������
        if (rand() % 2 == 0) {
            int value = rand() % maxRange;
            return new ExpressionNode(Fraction(value, 1));
        }
        else {
            return new ExpressionNode(generateFraction());
        }
    }

    // ������������
    char generateOperator() {
        char operators[] = { '+', '-', '*', '/' };
        return operators[rand() % 4];
    }

    // �������ʽ��
    ExpressionNode* buildExpressionTree(int operatorCount) {
        if (operatorCount == 0) {
            return generateNumber();
        }

        // ��������������������������
        int leftCount = rand() % operatorCount;
        int rightCount = operatorCount - leftCount - 1;

        ExpressionNode* left = buildExpressionTree(leftCount);
        ExpressionNode* right = buildExpressionTree(rightCount);

        return new ExpressionNode(generateOperator(), left, right);
    }

public:
    ExpressionGenerator(int range, int maxOps = 3) : maxRange(range), maxOperators(maxOps) {
        srand(time(nullptr));
    }

    // ���ɱ��ʽ
    Expression generate() {
        int operatorCount = rand() % maxOperators + 1; // 1��maxOperators�������

        Expression expr;
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;

        while (attempts < MAX_ATTEMPTS) {
            ExpressionNode* root = buildExpressionTree(operatorCount);
            expr.setRoot(root);

            if (expr.isValid(maxRange)) {
                return expr;
            }

            attempts++;
        }

        // �����γ���ʧ�ܣ�����һ���򵥵ı��ʽ
        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
    }

    // ��׼�����ʽ�����ڼ���ظ���
    Expression normalize(const Expression& expr) {
        // ʵ�ֱ��ʽ��׼�������ڼ���ظ���Ŀ
        // ������Ҫʵ�ֱ��ʽ�Ľ����ɺͽ���ɱ�׼��

        return expr;
    }
};

class DuplicateChecker {
private:
    set<string> generatedExpressions;

    // ���ʽ��׼��������ɽ����Ĳ�����
    string standardizeExpression(const Expression& expr) {
        // ʵ�ֱ��ʽ�ı�׼����ʾ
        // ���ɽ����Ĳ�������+�͡����Ĳ��������ֵ�������
        // ����Ҫ�������ʽ������������

        return expr.toString();
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

    // ���Ĵ�
    static void gradeAnswers(const string& exerciseFile, const string& answerFile, const string& gradeFile) {
        ifstream exFile(exerciseFile);
        ifstream ansFile(answerFile);
        ofstream grdFile(gradeFile);

        vector<int> correct, wrong;
        string exLine, ansLine;
        int questionNum = 0;

        while (getline(exFile, exLine) && getline(ansFile, ansLine)) {
            questionNum++;

            // ��ȡ�𰸲��֣��򻯴���ʵ����Ҫ�����ӵĽ�����
            size_t dotPos = ansLine.find('.');
            if (dotPos != string::npos) {
                string ansStr = ansLine.substr(dotPos + 2); // ������źͿո�

                // ������Ҫʵ�ִ���֤�߼�
                correct.push_back(questionNum);
            }
        }

        // ���ͳ�ƽ��
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

        exFile.close();
        ansFile.close();
        grdFile.close();
    }
};

#include <unordered_map>

class CommandLineParser {
private:
    unordered_map<string, string> arguments;

public:
    void parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];

            if (arg == "-n" && i + 1 < argc) {
                arguments["-n"] = argv[++i];
            }
            else if (arg == "-r" && i + 1 < argc) {
                arguments["-r"] = argv[++i];
            }
            else if (arg == "-e" && i + 1 < argc) {
                arguments["-e"] = argv[++i];
            }
            else if (arg == "-a" && i + 1 < argc) {
                arguments["-a"] = argv[++i];
            }
        }
    }

    bool has(const string& key) const {
        return arguments.find(key) != arguments.end();
    }

    string get(const string& key, const string& defaultValue = "") const {
        auto it = arguments.find(key);
        if (it != arguments.end()) {
            return it->second;
        }
        return defaultValue;
    }
};

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
        FileProcessor::saveExercises(exercises, "Exercises.txt");
        FileProcessor::saveAnswers(answers, "Answers.txt");

        cout << "������ " << exercises.size() << " ����Ŀ�� Exercises.txt" << endl;
        cout << "���ѱ��浽 Answers.txt" << endl;
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