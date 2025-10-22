#define _CRT_SECURE_NO_WARNINGS  1
#pragma warning(disable:6031)
#include <gtest/gtest.h>
#include "Fraction.h"
#include "Expression.h"

// ����Fraction��Ļ�������
TEST(FractionTest, BasicOperations) {
    // ����1�������ӷ�
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    EXPECT_EQ((f1 + f2).toString(), "5/6");

    // ����2����������
    Fraction f3(3, 4);
    Fraction f4(1, 4);
    EXPECT_EQ((f3 - f4).toString(), "1/2");

    // ����3�������˷�
    Fraction f5(2, 3);
    Fraction f6(3, 4);
    EXPECT_EQ((f5 * f6).toString(), "1/2");

    // ����4����������
    Fraction f7(1, 2);
    Fraction f8(3, 4);
    EXPECT_EQ((f7 / f8).toString(), "2/3");

    // ����5��������ת��
    Fraction f9("1'1/2");
    EXPECT_EQ(f9.getNumerator(), 3);
    EXPECT_EQ(f9.getDenominator(), 2);
}

// ����Expression��ı��ʽ����
TEST(ExpressionTest, Evaluation) {
    // ����6�����������ʽ
    ExpressionNode* node1 = new ExpressionNode('+', new ExpressionNode(Fraction(2, 1)), new ExpressionNode(Fraction(3, 1)));
    Expression expr1(node1);
    EXPECT_EQ(expr1.evaluate().toString(), "5");

    // ����7���������ʽ
    ExpressionNode* node2 = new ExpressionNode('*', new ExpressionNode(Fraction(1, 2)), new ExpressionNode(Fraction(2, 3)));
    Expression expr2(node2);
    EXPECT_EQ(expr2.evaluate().toString(), "1/3");

    // ����8���������
    ExpressionNode* node3 = new ExpressionNode('+',
        new ExpressionNode('*', new ExpressionNode(Fraction(1, 2)), new ExpressionNode(Fraction(2, 3))),
        new ExpressionNode(Fraction(1, 3))
    );
    Expression expr3(node3);
    EXPECT_EQ(expr3.evaluate().toString(), "2/3");

    // ����9�����ʽ��׼����ȥ���ź�����
    ExpressionNode* node4 = new ExpressionNode('+', new ExpressionNode(Fraction(3, 1)), new ExpressionNode(Fraction(2, 1)));
    Expression expr4(node4);
    EXPECT_EQ(expr4.standardize().toString(), "2 + 3 = ");

    // ����10�����ʽ�Ϸ��Լ��
    ExpressionNode* node5 = new ExpressionNode('-', new ExpressionNode(Fraction(1, 1)), new ExpressionNode(Fraction(2, 1)));
    Expression expr5(node5);
    EXPECT_FALSE(expr5.isValid(10)); // ���Ϊ�������Ϸ�
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}