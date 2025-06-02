#pragma once

template<class INF, class FRIEND>
class ListNode
{
private:
    INF value;
    ListNode *next;

    ListNode() { next = nullptr; }

    ListNode(INF _value) {
        value = _value;
        next = nullptr;
    }

    friend FRIEND;
};


template<class INF>
class MyStack {
    typedef class ListNode<INF, MyStack<INF>> Node;
    Node *top;
public:
    MyStack() {
        top = nullptr;
    }

    MyStack(const MyStack &st) {
        Node *tmp = st.top;
        MyStack tmp_st;
        while (tmp) {
            tmp_st.push(tmp->value);
            tmp = tmp->next;
        }
        top = nullptr;
        while (!tmp_st.empty()) {
            push(tmp_st.top_inf());
            tmp_st.pop();
        }
    }

    MyStack& operator = (const MyStack &st) {
        Node *tmp = st.top;
        MyStack tmp_st;
        while (tmp) {
            tmp_st.push(tmp->value);
            tmp = tmp->next;
        }
        top = nullptr;
        while (!tmp_st.empty()) {
            push(tmp_st.top_inf());
            tmp_st.pop();
        }
        return *this;
    }

    ~MyStack() {
        Node *tmp;
        while (top) {
            tmp = top->next;
            delete top;
            top = tmp;
        }
    }

    bool empty() {
        return (!top);
    }

    bool push(INF n) {
        Node *tmp = new Node(n);
        tmp->next = top;
        top = tmp;
        return true;
    }

    bool pop() {
        if (empty()) return false;
        Node *tmp = top->next;
        delete top;
        top = tmp;
        return true;
    }

    INF top_inf() {
        return top->value;
    }
};
