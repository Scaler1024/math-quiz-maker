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

// 分数类
class Fraction {
private:
    int numerator;   // 分子
    int denominator; // 分母

    // 求最大公约数
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    // 化简分数
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
            denominator = 1; // 防止除零错误
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        simplify();
    }

    // 从字符串构造分数
    Fraction(const string& str) {
        size_t apostrophe = str.find("'");
        size_t slash = str.find("/");

        if (apostrophe != string::npos) {
            // 带分数格式: a'b/c
            int whole = stoi(str.substr(0, apostrophe));
            int num = stoi(str.substr(apostrophe + 1, slash - apostrophe - 1));
            int den = stoi(str.substr(slash + 1));

            numerator = whole * den + (whole >= 0 ? num : -num);
            denominator = den;
        }
        else if (slash != string::npos) {
            // 真分数格式: a/b
            numerator = stoi(str.substr(0, slash));
            denominator = stoi(str.substr(slash + 1));
        }
        else {
            // 整数格式
            numerator = stoi(str);
            denominator = 1;
        }
        simplify();
    }

    // 转换为字符串
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

    // 四则运算重载
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

    // 比较运算符
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

    // 获取分子分母
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }

    // 判断是否为非负数（用于验证题目要求）
    bool isNonNegative() const {
        return numerator >= 0;
    }

    // 判断是否为真分数
    bool isProperFraction() const {
        return abs(numerator) < denominator;
    }
};


// 表达式节点类型
enum NodeType { NUMBER, OPERATOR };

// 表达式树节点
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

// 表达式类
class Expression {
private:
    ExpressionNode* root;

    // 克隆子树
    ExpressionNode* clone(ExpressionNode* node) {
        if (!node) return nullptr;
        if (node->type == NUMBER) {
            return new ExpressionNode(node->value);
        }
        return new ExpressionNode(node->op, clone(node->left), clone(node->right));
    }

    // 删除子树
    void clear(ExpressionNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // 计算表达式树
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

    // 生成字符串表示
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

    // 设置根节点
    void setRoot(ExpressionNode* r) {
        clear(root);
        root = r;
    }

    // 计算表达式值
    Fraction evaluate() const {
        if (!root) return Fraction(0, 1);
        return evaluate(root);
    }

    // 获取字符串表示
    string toString() const {
        if (!root) return "";
        return toString(root, false) + " = ";
    }

    // 获取表达式树根节点
    ExpressionNode* getRoot() const { return root; }

    // 判断表达式是否有效（符合题目要求）
    bool isValid(int maxRange) const {
        // 检查数值范围
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

        // 检查减法结果非负
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

        // 检查除法结果为真分数
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

    // 随机生成分数
    Fraction generateFraction() {
        int denominator = rand() % (maxRange - 1) + 2; // 分母从2到maxRange-1
        int numerator = rand() % denominator; // 分子小于分母，生成真分数
        return Fraction(numerator, denominator);
    }

    // 随机生成数字（整数或分数）
    ExpressionNode* generateNumber() {
        // 50%概率生成整数，50%概率生成真分数
        if (rand() % 2 == 0) {
            int value = rand() % maxRange;
            return new ExpressionNode(Fraction(value, 1));
        }
        else {
            return new ExpressionNode(generateFraction());
        }
    }

    // 随机生成运算符
    char generateOperator() {
        char operators[] = { '+', '-', '*', '/' };
        return operators[rand() % 4];
    }

    // 构建表达式树
    ExpressionNode* buildExpressionTree(int operatorCount) {
        if (operatorCount == 0) {
            return generateNumber();
        }

        // 随机决定左右子树的运算符数量
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

    // 生成表达式
    Expression generate() {
        int operatorCount = rand() % maxOperators + 1; // 1到maxOperators个运算符

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

        // 如果多次尝试失败，返回一个简单的表达式
        return Expression(new ExpressionNode('+', generateNumber(), generateNumber()));
    }

    // 标准化表达式（用于检测重复）
    Expression normalize(const Expression& expr) {
        // 实现表达式标准化，便于检测重复题目
        // 这里需要实现表达式的交换律和结合律标准化

        return expr;
    }
};

class DuplicateChecker {
private:
    set<string> generatedExpressions;

    // 表达式标准化：排序可交换的操作数
    string standardizeExpression(const Expression& expr) {
        // 实现表达式的标准化表示
        // 将可交换的操作符（+和×）的操作数按字典序排序
        // 这需要遍历表达式树并重新排列

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

    // 批改答案
    static void gradeAnswers(const string& exerciseFile, const string& answerFile, const string& gradeFile) {
        ifstream exFile(exerciseFile);
        ifstream ansFile(answerFile);
        ofstream grdFile(gradeFile);

        vector<int> correct, wrong;
        string exLine, ansLine;
        int questionNum = 0;

        while (getline(exFile, exLine) && getline(ansFile, ansLine)) {
            questionNum++;

            // 提取答案部分（简化处理，实际需要更复杂的解析）
            size_t dotPos = ansLine.find('.');
            if (dotPos != string::npos) {
                string ansStr = ansLine.substr(dotPos + 2); // 跳过序号和空格

                // 这里需要实现答案验证逻辑
                correct.push_back(questionNum);
            }
        }

        // 输出统计结果
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
        FileProcessor::saveExercises(exercises, "Exercises.txt");
        FileProcessor::saveAnswers(answers, "Answers.txt");

        cout << "已生成 " << exercises.size() << " 道题目到 Exercises.txt" << endl;
        cout << "答案已保存到 Answers.txt" << endl;
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