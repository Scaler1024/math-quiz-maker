#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include <gtest/gtest.h>
#include "Fraction.h"
#include "Expression.h"

// 测试Fraction类的基本运算
TEST(FractionTest, BasicOperations) {
    // 测试1：分数加法
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    EXPECT_EQ((f1 + f2).toString(), "5/6");

    // 测试2：分数减法
    Fraction f3(3, 4);
    Fraction f4(1, 4);
    EXPECT_EQ((f3 - f4).toString(), "1/2");

    // 测试3：分数乘法
    Fraction f5(2, 3);
    Fraction f6(3, 4);
    EXPECT_EQ((f5 * f6).toString(), "1/2");

    // 测试4：分数除法
    Fraction f7(1, 2);
    Fraction f8(3, 4);
    EXPECT_EQ((f7 / f8).toString(), "2/3");

    // 测试5：带分数转换
    Fraction f9("1'1/2");
    EXPECT_EQ(f9.getNumerator(), 3);
    EXPECT_EQ(f9.getDenominator(), 2);
}

// 测试Expression类的表达式计算
TEST(ExpressionTest, Evaluation) {
    // 测试6：简单整数表达式
    ExpressionNode* node1 = new ExpressionNode('+', new ExpressionNode(Fraction(2, 1)), new ExpressionNode(Fraction(3, 1)));
    Expression expr1(node1);
    EXPECT_EQ(expr1.evaluate().toString(), "5");

    // 测试7：分数表达式
    ExpressionNode* node2 = new ExpressionNode('*', new ExpressionNode(Fraction(1, 2)), new ExpressionNode(Fraction(2, 3)));
    Expression expr2(node2);
    EXPECT_EQ(expr2.evaluate().toString(), "1/3");

    // 测试8：混合运算
    ExpressionNode* node3 = new ExpressionNode('+',
        new ExpressionNode('*', new ExpressionNode(Fraction(1, 2)), new ExpressionNode(Fraction(2, 3))),
        new ExpressionNode(Fraction(1, 3))
    );
    Expression expr3(node3);
    EXPECT_EQ(expr3.evaluate().toString(), "2/3");

    // 测试9：表达式标准化（去括号和排序）
    ExpressionNode* node4 = new ExpressionNode('+', new ExpressionNode(Fraction(3, 1)), new ExpressionNode(Fraction(2, 1)));
    Expression expr4(node4);
    EXPECT_EQ(expr4.standardize().toString(), "2 + 3 = ");

    // 测试10：表达式合法性检查
    ExpressionNode* node5 = new ExpressionNode('-', new ExpressionNode(Fraction(1, 1)), new ExpressionNode(Fraction(2, 1)));
    Expression expr5(node5);
    EXPECT_FALSE(expr5.isValid(10)); // 结果为负，不合法
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}