#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<fstream>

using namespace std;

class Node
{
public:
    char token[512];
    double Value;
    Node* Left;
    Node* Right;
    Node()
    {
        Left=nullptr;
        Right=nullptr;
        token[0]=0;
    }
    Node(int input)
    {
        Value=input;
        Left=nullptr;
        Right=nullptr;
    }
};


class Element
{
public:
    Element* Next;
    Node* Key;
    Element* Prev;
    Element(Node* newkey): Key(newkey), Next(nullptr), Prev(nullptr) {}
};

class Stack
{
public:
    Element* Last;
    Element* First;
    Stack(): Last(nullptr), First(nullptr) {}
    void Push(Node* newkey)
    {
        if(Last==nullptr)
            First=Last=new Element(newkey);

        else
        {
            Element* aux= Last;
            Last->Next=new Element(newkey);
            Last=Last->Next;
            Last->Prev=aux;
        }
    }

    Node* Pop()
    {
        if (Last!=nullptr)
        {
            Node* aux= Last->Key;
            if( Last!=First && Last)
            {
                Last=Last->Prev;
                delete Last->Next;
            }
            else
            {
                if(First)
                    delete First;
                First=Last=nullptr;
            }
            return aux;
        }
        return NULL;
    }

    ~Stack()
    {
        if(Last!=nullptr)
        {
            Element *Current= Last->Prev;
            while(Current!=nullptr)
            {
                delete Current->Next;
                Current=Current->Prev;
            }
            delete First;
        }
    }
};

bool isOperator(char* p )
{
    if ((p[0]=='*' || p[0]=='+' || p[0]== '-' || p[0]=='/' )&& p[1]==0)
        return true;
    else
        return false;
}


bool isNumber(char*p)
{
    bool ok=0;
    for(int i=0; p[i]; ++i)
    {
        if(p[i]=='.')
        {
            if(!ok)
                ok=1;
            else
                return false;
        }
        else if(p[i]<'0' || p[i]> '9')
            return false;
    }
    return true;
}


class Tree
{
public:
    Node *Root;
    void Delete(Node* currentNode)
    {
        if(currentNode!=nullptr)
        {
            Delete(currentNode->Left);
            Delete(currentNode->Right);
            delete currentNode;
        }
    }
    Tree(char *exp)
    {
        Root= new Node();
        Stack NodeStack;
        Node *Current=Root;
        char *p;
        p=strtok(exp, " ");
        while(p)
        {
            if( isOperator(p))
            {
                strcpy(Current->token, p);
                Current->Left=new Node();
                Current->Right=new Node();
                NodeStack.Push(Current->Right);
                Current=Current->Left;
            }
            else if(isNumber(p))
            {
                strcpy(Current->token, p);
                Current->Value=atof(p);
                Current=NodeStack.Pop();
            }
            else if(p[0]!='(' && p[0]!=')')
            {
                cout<<"Unknown symbol!\n";
                Delete(Root);
                Root=nullptr;
                break;
            }
            p=strtok(NULL, " ");
        }
    }

    double Compute(Node * Current)
    {
        if(isOperator(Current->token))
        {
            if(Current->token[0]=='+')
                return Compute(Current->Left)+Compute(Current->Right);
            else if(Current->token[0]=='-')
                return Compute(Current->Left)-Compute(Current->Right);
            else if(Current->token[0]=='*')
                return Compute(Current->Left)*Compute(Current->Right);
            else if(Current->token[0]=='/')
                return Compute(Current->Left)/Compute(Current->Right);
        }
        else return Current->Value;
    }
};

int main()
{
    ifstream fin("input.txt");
    char input[512];
    for( int i=1; i <= 10; ++i)
    {
        fin.getline(input,512);
        Tree T(input);
        cout<<"The value of expression number "<<i<<" is "<< T.Compute(T.Root)<< "."<<endl;
    }
    return 0;
}
