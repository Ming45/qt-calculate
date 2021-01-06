#ifndef CALCU_H
#define CALCU_H
#include<iostream>
#include<QString>
#include<QChar>
#include<vector>
#include <cmath>
#include<QByteArray>
#include <QDebug>
using namespace std;

template<typename T>class Stack;//声明

template<typename T>
class Node {//利用模板定义一个结点
    friend class Stack<T>;//要将Stack声明为有元
    T m_data;//数据域
    Node* m_next = nullptr;//指向下一个结点的指针（指向node类型的指针）（类允许包含指向其自身类型的指针 或引用）
public:
    Node(const T& val) :m_data(val) {}//构造函数，对数据域初始化
    const T& data()const { return m_data; }//const 版本，读操作用（对T&data限定，对函数体内限定）
    T& data() { return m_data; }//非const版本，写操作用
    Node* next() { return m_next; }//返回指向下一个指针
};

template<typename T>
class Stack {
    Node<T>* m_top = nullptr;//头指针
public:
    Stack() = default;//默认构造函数
    Stack(const Stack&) = delete;//禁止复制
    Stack& operator =(const Stack&) = delete;//禁止赋值
    ~Stack()= default;
    void clear();//清空栈
    void push(const T& val);//进栈
    void pop();//出栈
    bool empty()const { return m_top == nullptr; }//是否为空
    const T& top() { return m_top->m_data; }//取栈顶元素（对栈顶元素只能进行读操作）
};

class Calculator {//计算器类
private:
    Stack<double>m_num;//操作数栈
    Stack<char>m_opr;//运算符栈
    int precedence(const QChar& s)const;//获取运算符优先级（不可对运算符和函数体进行写操作）
    double readNum(QString::const_iterator& it);//读取操作数
    void calculator();//取出运算符和操作数进行计算
    bool isNum(QString::const_iterator& c)const {//内联函数，判断是否为数字（不可对函数体进行写操作）
        return ((*c >= '0') && (*c <= '9')) || (*c == '.');//必须为0到9，可以有.，用于小数
    }
public:
    Calculator() { m_opr.push('#'); }//运算符栈初始化,放优先级最低的#进入
    double doIt(const QString& exp);//表达式求值（不可对表达式进行写操作）
};

#endif // CALCU_H
