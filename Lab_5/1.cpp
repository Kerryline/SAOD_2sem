#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct tLE {
    tLE* next;
    union {
        int data;
        unsigned short sdata;
        unsigned char bytes[4];
    };
};

struct Queue {
    tLE* head;
    tLE* tail;
};

void digitalSort(tLE*& S, int byteCount, bool reverse, int& moves) {
    if (S == NULL) return;
    
    const int m = 256;
    Queue Q[m];
    
    for (int byteIndex = 0; byteIndex < byteCount; byteIndex++) {
        for (int i = 0; i < m; i++) {
            Q[i].head = Q[i].tail = NULL;
        }
        
        tLE* current = S;
        while (current != NULL) {
            unsigned char digit = current->bytes[byteIndex];
            
            if (Q[digit].head == NULL) {
                Q[digit].head = Q[digit].tail = current;
            } else {
                Q[digit].tail->next = current;
                Q[digit].tail = current;
            }
            moves++;
            
            tLE* next = current->next;
            current->next = NULL;
            current = next;
        }
        
        tLE* newHead = NULL;
        tLE* newTail = NULL;
        
        if (reverse) {
            for (int i = m - 1; i >= 0; i--) {
                if (Q[i].head != NULL) {
                    if (newHead == NULL) {
                        newHead = Q[i].head;
                        newTail = Q[i].tail;
                    } else {
                        newTail->next = Q[i].head;
                        newTail = Q[i].tail;
                    }
                    moves++;
                }
            }
        } else {
            for (int i = 0; i < m; i++) {
                if (Q[i].head != NULL) {
                    if (newHead == NULL) {
                        newHead = Q[i].head;
                        newTail = Q[i].tail;
                    } else {
                        newTail->next = Q[i].head;
                        newTail = Q[i].tail;
                    }
                    moves++;
                }
            }
        }
        
        S = newHead;
    }
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
    while (current != NULL && count < 10) {
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
                value = rand() % 1000 + 1;
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

int checkSum(tLE* head) {
    int sum = 0;
    tLE* current = head;
    while (current != NULL) {
        sum += current->data;
        current = current->next;
    }
    return sum;
}

int countSeries(tLE* head, bool ascending) {
    if (head == NULL) return 0;
    
    int series = 1;
    tLE* current = head;
    
    while (current->next != NULL) {
        if (ascending) {
            if (current->data > current->next->data) {
                series++;
            }
        } else {
            if (current->data < current->next->data) {
                series++;
            }
        }
        current = current->next;
    }
    
    return series;
}

int main() {
    srand(time(0));
    
    tLE* original = createList(25, 2);
    printList(original, "Исходный список");
    cout << "Количество элементов: " << countElements(original) << endl;
    
    tLE *a, *b;
    int n;
    
    tLE* testList = createList(20, 2);
    int comparisons = 0, moves = 0;
    
    cout << "До сортировки: ";
    printList(testList, "Тестовый список");
    
    digitalSort(testList, 4, false, moves);
    
    cout << "После сортировки: ";
    printList(testList, "Тестовый список");
    cout << "Фактические: M=" << moves << endl;
    
    cout << "Трудоемкость цифровой сортировки DigitalSort" << endl;
    cout << "| N   | M теоретич. | Убыв. | Случ. | Возр. |" << endl;
    cout << "|-----|-------------|-------|-------|-------|" << endl;
    
    for (int size = 100; size <= 500; size += 100) {
        int L = 4;
        int theory_moves = 2 * L * size;
        
        int moves_desc, moves_rand, moves_asc;
        
        tLE* list_desc = createList(size, 1);
        moves_desc = 0;
        digitalSort(list_desc, 4, false, moves_desc);
        freeList(list_desc);
        
        tLE* list_rand = createList(size, 2);
        moves_rand = 0;
        digitalSort(list_rand, 4, false, moves_rand);
        freeList(list_rand);
        
        tLE* list_asc = createList(size, 0);
        moves_asc = 0;
        digitalSort(list_asc, 4, false, moves_asc);
        freeList(list_asc);
        
        cout << "| " << size << " | " << theory_moves << "          | " 
             << moves_desc << "    | " << moves_rand << "    | " << moves_asc << "    |" << endl;
    }
    
    freeList(testList);
    
    return 0;
}