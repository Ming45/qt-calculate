#include "calcu.h"

template<class T>
void Stack<T>::push(const T& val) {//进栈
    Node<T>* node = new Node<T>(val);//创建一个新结点（node指向新创建的动态内存）
    node->m_next = m_top;//将结点node压栈,将node指向的结点的m_next指向m_top
    m_top = node;//修改栈顶指针，将m_top指向node结点
}

template<class T>
void Stack<T>::pop() {//出栈
    Node<T>* p = m_top;//将栈顶元素地址存放到指针p中
    m_top = m_top->m_next;//修改栈顶指针，将m_top指向m_top指向的下一个结点
    delete p;//释放释放指针p指向的内存，（若不这样操作，会造成内存泄露，出栈的那个内存未处理）
}

template<class T>
void Stack<T>::clear() {//清空栈
    Node<T>* p = nullptr;//工具指针，用于协助出栈,同上
    while (m_top != nullptr) {
        p = m_top;
        m_top = m_top->m_next;
        delete p;
    }
}

int Calculator::precedence(const QChar& s)const {//获取运算符优先级（不可对运算符和函数体进行写操作）
    switch (s.unicode()) {
    case'=':return 0;
    case'#':return 1;
    case'+':case'-':return 2;
    case'*':case'/':case'%':return 3;
    case'^':return 4;
    case's':case'c':case't':case'r':return -1;//在后面进行判断是否为sin cos tan sqrt
    case'S':case'C':case'T':case'R':return 5;
    case'(':case')':return 6;
    }
    return 0;
}

double Calculator::readNum(QString::const_iterator& it) {//读取操作数
    QString t;
    while (isNum(it) || (*it == 'p'))//判断是否为数字，如果是，迭代器指针加一继续扫描，直到遇到运算符
    if (*it == 'p') {//判断是不是pi
        ++it;
        if (*it == 'i') {
            ++it;
            return (atan(1.0) * 4);
        }
        else {
            //如果不是，报错
            qDebug() << "表达式出错，下面结果-1代表报错" << endl;
            return -1;
        }
    }
    else//如果不是pi就正常获取数
        t += *it++;
    return t.toDouble();//将数字字符串转换为double类型
}

void Calculator::calculator() {//取出运算符和操作数进行计算
    double b = m_num.top();//取出右操作数
    double a=0;//定义a用于取左操作数
    m_num.pop();//右操作数出栈
    if ((m_opr.top() != 'S')&& (m_opr.top() != 'C') && (m_opr.top() != 'T') && (m_opr.top() != 'R')) {//不是sin,cos,tan,sqrt单目运算符就取出做操作符
        a = m_num.top();//取出左操作数
        m_num.pop();//左操作数出栈
    }
    if (m_opr.top() == '+')//若为加，则将(a + b)压栈
        m_num.push(a + b);
    else if (m_opr.top() == '-')//若为减，则将(a - b)压栈
        m_num.push(a - b);
    else if (m_opr.top() == '*')//若为乘，则将(a * b)压栈
        m_num.push(a * b);
    else if (m_opr.top() == '/')//若为除，则将(a / b)压栈
        m_num.push(a / b);
    else if (m_opr.top() == '%')//若为求余，则将(a % b)压栈
        m_num.push(fmod(a, b));
    else if (m_opr.top() == '^')//若为求幂，则将a^b压栈
        m_num.push(pow(a,b));
    else if (m_opr.top() == 'S') {//若为sin，则将sin(b)压栈
        m_num.push(sin(b));
    }
    else if (m_opr.top() == 'C') {//若为cos，则将cos(b)压栈
        m_num.push(cos(b));
    }
    else if (m_opr.top() == 'T') {//若为tan，则将tan(b)压栈
        m_num.push(tan(b));
    }
    else if (m_opr.top() == 'R') {//若为sqrt，则将sqrt(b)压栈
        m_num.push(sqrt(b));
    }
    m_opr.pop();//当前运算结束，运算符出栈
}

double  Calculator::doIt(const QString& exp) {//表达式求值（不可对表达式进行写操作）
    m_num.clear();//保证同一个对象再次调用doIt时，数据栈为空

    for (auto it = exp.begin(); it != exp.end();) {//初始化，迭代器it指向表达式的第一个，判断句为：it没有指向表达式最后一个
        if (isNum(it)||(*it == 'p')) {//当遇到操作数时,或遇到p时，可能是pi
            m_num.push(readNum(it));//读取操作数，将其入栈
        }
        else {
            while ((precedence(*it) <= precedence(m_opr.top())) || (*it == ')')|| (precedence(*it)==-1)) {//遇到运算符时，判断现在指向的运算符优先级和运算符栈顶的优先级，若小于等于则进入循环(优先级低的先进去，高压低)
                                                                                 //或者当it指向‘）’时，即要开始处理括号内东西
                                                                                 //或者当优先级为-1时，即要进行判断是否为sin,cos tan,sqrt
                if ((m_opr.top() == '(') && (*it != ')')&& (*it != 's') && (*it != 'c') && (*it != 't')) {//如果此时栈顶是'('，且it值的不是‘）’，则无视优先级，将其后运算符入栈，(通过循环外面那个if语句入栈)
                    break;                                                                                //同时不是(sin)这样的
                }
                if ((m_opr.top() == '(') && (*it == ')')) {//如果此时栈顶是'('，且it值的是‘）’，则括号内处理完毕，
                    m_opr.pop();//将‘（’弹出去
                    break;
                }

                if (*it == 's') {//判断是否为sin
                    ++it;
                    if (*it == 'i') {
                        ++it;
                        if (*it == 'n') {//若是sin，则将S压栈，代表sin,并break结束
                            m_opr.push('S');
                            break;
                        }
                        else {//如果不是，报错
                            qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                            return -1;
                        }
                    }
                    else {//如果不是，it退回去,有可能是sqrt
                        --it;
                    }
                }

                if (*it == 'c') {//判断是否为cos
                    ++it;
                    if (*it == 'o') {
                        ++it;
                        if (*it == 's') {//若是cos，则将C压栈，代表cos,并break结束
                            m_opr.push('C');
                            break;
                        }
                        else {//如果不是，报错
                            qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                            return -1;
                        }
                    }
                    else {//如果不是，报错
                        qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                        return -1;
                    }
                }

                if (*it == 't') {//判断是否为tan
                    ++it;
                    if (*it == 'a') {
                        ++it;
                        if (*it == 'n') {//若是tan，则将T压栈，代表tan,并break结束
                            m_opr.push('T');
                            break;
                        }
                        else {//如果不是，报错
                            qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                            return -1;
                        }
                    }
                    else {//如果不是，报错
                        qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                        return -1;
                    }
                }

                if (*it == 's') {//判断是否为sqrt
                    ++it;
                    if (*it == 'q') {
                        ++it;
                        if (*it == 'r') {
                                ++it;
                            if (*it == 't') {//若是sqrt，则将R压栈，代表sqrt,并break结束
                                m_opr.push('R');
                                break;
                            }
                            else {//如果不是，报错
                                qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                                return -1;
                            }
                        }
                        else {//如果不是，报错
                            qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                            return -1;
                        }
                    }
                    else {//如果不是，报错(前面判断了不是sin，若这里还不是sqrt的话就是单个s，报错)
                        qDebug() << "表达式出错，下面结果-1代表报错" << endl;
                        return -1;
                    }
                }
                if (m_opr.top() == '#')//若运算符栈顶只剩下#，则计算完毕，直接break
                    break;
                calculator();//执行栈顶运算符（现在运算符/优先级 <= 运算符栈顶的优先级，且栈顶运算符不是#，或者现在运算符是‘）’）
            }
            if ((*it != '=')&&(*it != 'n') && (*it != 't') && (*it != 's')) {//如果it指向不是=，即没完成,且不是n,t,s（前面已经完成指定符的压栈，这里不需要再压栈）
                char work= it->unicode();
                m_opr.push(work);//则将运算符入栈（现在运算符优先级 > 运算符栈顶的优先级）
                if (*it == ')')//将栈顶的')'弹出
                    m_opr.pop();
            }
            ++it;//并继续扫描
        }
    }
    return m_num.top();//返回计算结果，注意数据栈此时非空
}
