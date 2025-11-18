#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

int M, C;

// Восстановление свойств пирамиды
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    C++;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    C++;
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        M += 3; // 3 пересылки при обмене
        
        heapify(arr, n, largest);
    }
}

// Построение пирамиды из массива
void buildHeap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// Пирамидальная сортировка
void heapSort(int arr[], int n) {
    M = 0; C = 0;
    
    // Построение пирамиды
    buildHeap(arr, n);
    
    // Сортировка
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        M += 3; // 3 пересылки при обмене
        
        heapify(arr, i, 0);
    }
}

// Проверка корректности сортировки
void checkSort(int arr[], int n, int& sum, int& series) {
    sum = 0;
    series = 1;
    
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        if (i > 0 && arr[i] < arr[i - 1]) {
            series++;
        }
    }
}

// Генерация массивов разного типа
void generateArray(int arr[], int n, int type) {
    switch (type) {
        case 0: // Убывающий
            for (int i = 0; i < n; i++) {
                arr[i] = n - i;
            }
            break;
        case 1: // Случайный
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 1000;
            }
            break;
        case 2: // Возрастающий
            for (int i = 0; i < n; i++) {
                arr[i] = i + 1;
            }
            break;
    }
}

// Теоретическая оценка трудоемкости построения пирамиды
void theoreticalComplexity(int n, double& theory) {
    theory = 0;
    for (int L = n/2; L >= 1; L--) {
        double k = log2((double)n / L);
        theory += 3 * k + 2; // C = 2k, M = k + 2
    }
}

int main() {
    srand(time(0));
    int sizes[] = {100, 200, 300, 400, 500};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    
    std::cout << "Трудоемкость построения пирамиды\n";
    std::cout << "N\tM+C теор.\tMф+Сф убыв.\tMф+Сф случ.\tMф+Сф возр.\n";
    
    // исследование построения пирамиды
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        double theory;
        theoreticalComplexity(n, theory);
        
        int M_ubyv, C_ubyv, M_sluch, C_sluch, M_vozr, C_vozr;
        
        // Убывающий массив
        int* arr1 = new int[n];
        generateArray(arr1, n, 0);
        M = 0; C = 0;
        buildHeap(arr1, n);
        M_ubyv = M; C_ubyv = C;
        
        // Случайный массив
        int* arr2 = new int[n];
        generateArray(arr2, n, 1);
        M = 0; C = 0;
        buildHeap(arr2, n);
        M_sluch = M; C_sluch = C;
        
        // Возрастающий массив
        int* arr3 = new int[n];
        generateArray(arr3, n, 2);
        M = 0; C = 0;
        buildHeap(arr3, n);
        M_vozr = M; C_vozr = C;
        
        std::cout << n << "\t" << theory << "\t\t" 
                  << M_ubyv + C_ubyv << "\t\t" << M_sluch + C_sluch 
                  << "\t\t" << M_vozr + C_vozr << std::endl;
        
        delete[] arr1;
        delete[] arr2;
        delete[] arr3;
    }
    
    std::cout << "\nТрудоемкость пирамидальной сортировки\n";
    std::cout << "N\tУбыв.\tВозр.\tСлуч.\n";
    
    // исследование пирамидальной сортировки
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        
        // Убывающий массив
        int* arr1 = new int[n];
        generateArray(arr1, n, 0);
        heapSort(arr1, n);
        int sum1, series1;
        checkSort(arr1, n, sum1, series1);
        int res1 = M + C;
        
        // Возрастающий массив  
        int* arr2 = new int[n];
        generateArray(arr2, n, 2);
        heapSort(arr2, n);
        int sum2, series2;
        checkSort(arr2, n, sum2, series2);
        int res2 = M + C;
        
        // Случайный массив
        int* arr3 = new int[n];
        generateArray(arr3, n, 1);
        heapSort(arr3, n);
        int sum3, series3;
        checkSort(arr3, n, sum3, series3);
        int res3 = M + C;
        
        std::cout << n << "\t" << res1 << "\t" << res2 << "\t" << res3 << std::endl;
        
        delete[] arr1;
        delete[] arr2;
        delete[] arr3;
    }
    
    return 0;
}