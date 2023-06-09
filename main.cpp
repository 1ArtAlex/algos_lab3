#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

struct Node {
    int data;
    Node* next;
};

class Stack {
private:
    Node* top;

public:
    Stack() : top(nullptr) {}

    void push(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = top;
        top = newNode;
    }

    int pop() {
        if (isEmpty()) {
            cerr << "Стек пуст" << endl;
            return 0;
        }
        int value = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        return value;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    Node* getTop() {
        return top;
    }
};

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

bool highPrecedence(char op1, char op2) {
    int op1Precedence = precedence(op1);
    int op2Precedence = precedence(op2);
    return op1Precedence >= op2Precedence;
}

string infixToPostfix(const string& expression) {
    string postfix;
    Stack operators;

    for (char ch : expression) {
        if (ch == ' ') {
            continue;
        }

        if (isdigit(ch)) {
            postfix += ch;
        } else if (isOperator(ch)) {
            while (!operators.isEmpty() && operators.getTop()->data != '(' && highPrecedence(operators.getTop()->data, ch)) {
                postfix += operators.pop();
            }
            operators.push(ch);
        } else if (ch == '(') {
            operators.push(ch);
        } else if (ch == ')') {
            while (!operators.isEmpty() && operators.getTop()->data != '(') {
                postfix += operators.pop();
            }
            if (!operators.isEmpty() && operators.getTop()->data == '(') {
                operators.pop();
            }
        }
    }

    while (!operators.isEmpty()) {
        postfix += operators.pop();
    }

    return postfix;
}

int evaluatePostfix(const string& postfix) {
    Stack operands;

    for (char ch : postfix) {
        if (isdigit(ch)) {
            operands.push(ch - '0');
        } else if (isOperator(ch)) {

            int operand2 = operands.pop();
            int operand1 = operands.pop();
            int result;

            switch (ch) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 != 0) {
                        result = operand1 / operand2;
                    } else {
                        cerr << "Деление на ноль" << endl;
                        exit(1);
                    }
                    break;
            }

            operands.push(result);
        }
    }

    if (!operands.isEmpty()) {
        return operands.pop();
    } else {
        cerr << "Недопустимое постфиксное выражение" << endl;
        exit(1);
    }
}

bool isValidExpression(const string& expression) {
    int numOpenParentheses = 0;
    int numCloseParentheses = 0;
    bool isOperandExpected = true;

    for (char ch : expression) {
        if (ch == ' ') {
            continue;
        }

        if (isdigit(ch)) {
            if (!isOperandExpected) {
                return false;
            }
            isOperandExpected = false;
        } else if (ch == '(') {
            numOpenParentheses++;
            isOperandExpected = true;
        } else if (ch == ')') {
            numCloseParentheses++;
            if (isOperandExpected || numCloseParentheses > numOpenParentheses) {
                return false;
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (isOperandExpected) {
                return false;
            }
            isOperandExpected = true;
        } else {
            return false;
        }
    }
    return numOpenParentheses == numCloseParentheses && !isOperandExpected;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    string expression;
    cout << "Введите инфиксное выражение: ";
    getline(cin, expression);

    if (!isValidExpression(expression)) {
        cerr << "Неверное инфиксное выражение" << endl;
        return 1;
    }
    string postfix = infixToPostfix(expression);
    if (postfix.empty()) {
        cerr << "Неверное инфиксное выражение" << endl;
        return 1;
    }

    cout << "Постфиксное выражение: " << postfix << endl;
    int result = evaluatePostfix(postfix);
    cout << "Ответ: " << result << endl;

    return 0;
}