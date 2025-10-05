#include <iostream>
#include <string>
using namespace std;

class TextEditor {
public:
    void insertChar(char c);
    void deleteChar();
    void moveLeft();
    void moveRight();
    string getTextWithCursor() const;
    virtual ~TextEditor() {}
    TextEditor() {}
};

class Node {
public:
    char data;
    Node* prev;
    Node* next;
    Node(char c) {
        data = c;
        prev = NULL;
        next = NULL;
    }
};

class EditorData {
public:
    Node* head;
    Node* tail;
    Node* cursor;
    EditorData() {
        head = NULL;
        tail = NULL;
        cursor = NULL;
    }
};

static EditorData ed;

void TextEditor::insertChar(char c) {
    Node* newNode = new Node(c);
    if (ed.head == NULL) {
        ed.head = newNode;
        ed.tail = newNode;
        ed.cursor = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    } else if (ed.cursor == NULL) {
        newNode->next = ed.head;
        ed.head->prev = newNode;
        ed.head = newNode;
        ed.cursor = newNode;
    } else {
        newNode->prev = ed.cursor;
        newNode->next = ed.cursor->next;
        if (ed.cursor->next != NULL)
            ed.cursor->next->prev = newNode;
        else
            ed.tail = newNode;
        ed.cursor->next = newNode;
        ed.cursor = newNode;
    }
}

void TextEditor::deleteChar() {
    if (ed.cursor == NULL)
        return;
    Node* toDelete = ed.cursor;
    if (toDelete->prev != NULL)
        toDelete->prev->next = toDelete->next;
    else
        ed.head = toDelete->next;
    if (toDelete->next != NULL)
        toDelete->next->prev = toDelete->prev;
    else
        ed.tail = toDelete->prev;
    ed.cursor = toDelete->prev;
    delete toDelete;
}

void TextEditor::moveLeft() {
    if (ed.cursor != NULL)
        ed.cursor = ed.cursor->prev;
}

void TextEditor::moveRight() {
    if (ed.cursor == NULL)
        ed.cursor = ed.head;
    else if (ed.cursor->next != NULL)
        ed.cursor = ed.cursor->next;
}

string TextEditor::getTextWithCursor() const {
    string result = "";
    Node* temp = ed.head;
    while (temp != NULL) {
        if (temp == ed.cursor)
            result += '|';
        result += temp->data;
        temp = temp->next;
    }
    if (ed.cursor == NULL || ed.cursor == ed.tail)
        result += '|';
    return result;
}

int main() {
    TextEditor ed;

    ed.insertChar('a');
    cout << "After insert 'a': " << ed.getTextWithCursor() << endl;

    ed.insertChar('b');
    cout << "After insert 'b': " << ed.getTextWithCursor() << endl;

    ed.moveLeft();
    cout << "After move left: " << ed.getTextWithCursor() << endl;

    ed.insertChar('c');
    cout << "After insert 'c': " << ed.getTextWithCursor() << endl;

    ed.deleteChar();
    cout << "After delete: " << ed.getTextWithCursor() << endl;

    ed.moveLeft();
    ed.moveLeft();
    cout << "After move left twice: " << ed.getTextWithCursor() << endl;

    ed.moveRight();
    ed.moveRight();
    cout << "After move right twice: " << ed.getTextWithCursor() << endl;

    return 0;
}
