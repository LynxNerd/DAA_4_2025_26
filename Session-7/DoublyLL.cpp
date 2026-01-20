#include<iostream>
using namespace std;

class Node {
    public :
    int data;
    Node *Next;
    Node *Prev;          

    Node(int val) {
        data = val;
        Next = NULL;
        Prev = NULL;     
    }
};

Node *Head;

class List {
    public:
    List() {
        Head = NULL;
    }

    void InsertEnd(int val) {
        Node *newnode  = new Node(val);

        if(Head == NULL) {
            Head = newnode;
        } else {
            Node *temp = Head;
            while(temp->Next != NULL) {
                temp = temp->Next;
            }
            temp->Next = newnode;
            newnode->Prev = temp;   
        }
    }

    void deleteVal(int val) {
        if(Head == NULL){
            cout<<"Empty List";
            return;
        }

        if(Head->data == val) {
            Node *temp = Head;
            Head = Head->Next;

            if(Head != NULL) {
                Head->Prev = NULL;  
            }

            delete temp;
            return;
        } 

        Node *temp = Head;
        while(temp->Next != NULL && temp->Next->data != val) {
            temp = temp->Next;
        }

        if(temp->Next == NULL) {
            cout<<"Value Not Found";
            return;
        } 

        Node *temp2 = temp->Next;
        temp->Next = temp2->Next;

        if(temp2->Next != NULL) {
            temp2->Next->Prev = temp;  
        }

        delete temp2;
    }
};

int main() {
    List l;
    l.InsertEnd(5);
    l.InsertEnd(10);
    l.InsertEnd(15);
    l.deleteVal(10);

    Node *temp = Head;
    while(temp != NULL) {
        cout<<temp->data<<" ";
        temp = temp->Next;
    }
}