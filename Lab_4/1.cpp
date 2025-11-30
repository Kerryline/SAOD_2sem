#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

struct tLE {
    tLE* next;
    int data;
};

struct Queue {
    tLE* head;
    tLE* tail;
};

void splitList(tLE* S, tLE*& a, tLE*& b, int& n) {
    if (S == NULL) {
        a = b = NULL;
        n = 0;
        return;
    }
    
    a = S;
    b = S->next;
    n = 1;
    
    tLE* k = a;
    tLE* p = b;
    
    while (p != NULL) {
        n++;
        k->next = p->next;
        k = p;
        p = p->next;
    }
}

void enqueue(Queue& q, tLE* element) {
    if (q.head == NULL) {
        q.head = q.tail = element;
    } else {
        q.tail->next = element;
        q.tail = element;
    }
}

void mergeSeries(tLE*& a, int q, tLE*& b, int r, Queue& c, int& comparisons, int& moves) {
    while (q > 0 && r > 0) {
        comparisons++;
        
        if (a->data <= b->data) {
            tLE* temp = a;
            a = a->next;
            temp->next = NULL;
            enqueue(c, temp);
            moves++;
            q--;
        } else {
            tLE* temp = b;
            b = b->next;
            temp->next = NULL;
            enqueue(c, temp);
            moves++;
            r--;
        }
    }
    
    while (q > 0) {
        tLE* temp = a;
        a = a->next;
        temp->next = NULL;
        enqueue(c, temp);
        moves++;
        q--;
    }
    
    while (r > 0) {
        tLE* temp = b;
        b = b->next;
        temp->next = NULL;
        enqueue(c, temp);
        moves++;
        r--;
    }
}

void mergeSort(tLE*& S, int& comparisons, int& moves) {
    if (S == NULL || S->next == NULL) return;
    
    tLE *a, *b;
    int n;
    
    splitList(S, a, b, n);
    
    int p = 1;
    
    Queue c[2];
    
    while (p < n) {
        c[0].head = c[0].tail = NULL;
        c[1].head = c[1].tail = NULL;
        
        int i = 0;
        int m = n;
        
        tLE* currentA = a;
        tLE* currentB = b;
        
        while (m > 0) {
            int q = (m >= p) ? p : m;
            m -= q;
            
            int r = (m >= p) ? p : m;
            m -= r;
            
            mergeSeries(currentA, q, currentB, r, c[i], comparisons, moves);
            
            i = 1 - i;
        }
        
        a = c[0].head;
        b = c[1].head;
        p *= 2;
    }
    
    if (c[0].tail != NULL) {
        c[0].tail->next = NULL;
    }
    S = c[0].head;
}

tLE* createNode(int value) {
    tLE* newNode = new tLE;
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void printList(tLE* head, const char* name) {
    cout << name << ": ";
    tLE* current = head;
    int count = 0;
    while (current != NULL && count < 20) {
        cout << current->data << " ";
        current = current->next;
        count++;
    }
    if (current != NULL) cout << "...";
    cout << endl;
}

int countElements(tLE* head) {
    int count = 0;
    tLE* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

tLE* createList(int n, int type) {
    if (n <= 0) return NULL;
    
    tLE* head = NULL;
    tLE* current = NULL;
    
    for (int i = 0; i < n; i++) {
        int value;
        switch (type) {
            case 0:
                value = i + 1;
                break;
            case 1:
                value = n - i;
                break;
            case 2:
                value = rand() % 100 + 1;
                break;
        }
        
        tLE* newNode = createNode(value);
        if (head == NULL) {
            head = current = newNode;
        } else {
            current->next = newNode;
            current = newNode;
        }
    }
    
    return head;
}

void freeList(tLE* head) {
    while (head != NULL) {
        tLE* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    srand(time(0));
    
    tLE* original = createList(25, 2);
    printList(original, "Исходный список");
    cout << "Количество элементов: " << countElements(original) << endl;
    
    tLE *a, *b;
    int n;
    splitList(original, a, b, n);
    
    printList(a, "Список A");
    printList(b, "Список B");
    cout << "Элементов в A: " << countElements(a) << endl;
    cout << "Элементов в B: " << countElements(b) << endl;
    
    tLE* testList = createList(20, 2);
    int comparisons = 0, moves = 0;
    
    cout << "До сортировки: ";
    printList(testList, "Тестовый список");
    
    mergeSort(testList, comparisons, moves);
    
    cout << "После сортировки: ";
    printList(testList, "Тестовый список");
    cout << "Фактические: C=" << comparisons << ", M=" << moves << ", M+C=" << (comparisons + moves) << endl;
    
    cout << "Трудоемкость сортировки прямого слияния" << endl;
    cout << "| N   | M+C теоретич. | Возр.   | Убыв.   | Случ.   |" << endl;
    cout << "|-----|---------------|---------|---------|---------|" << endl;
    
    for (int size = 100; size <= 500; size += 100) {
        double log2size = log2(size);
        int theory_total = 2 * size * ceil(log2size) + size;
        
        int res1, res2, res3;
        
        tLE* list1 = createList(size, 0);
        int comp1 = 0, mov1 = 0;
        mergeSort(list1, comp1, mov1);
        res1 = comp1 + mov1;
        freeList(list1);
        
        tLE* list2 = createList(size, 1);
        int comp2 = 0, mov2 = 0;
        mergeSort(list2, comp2, mov2);
        res2 = comp2 + mov2;
        freeList(list2);
        
        tLE* list3 = createList(size, 2);
        int comp3 = 0, mov3 = 0;
        mergeSort(list3, comp3, mov3);
        res3 = comp3 + mov3;
        freeList(list3);
        
        cout << "| " << size << " | " << theory_total << "          | " 
             << res1 << "    | " << res2 << "    | " << res3 << "    |" << endl;
    }
    
    freeList(a);
    freeList(b);
    freeList(testList);
    
    return 0;
}