#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Глобальные счетчики
long long comparisons = 0;
long long moves = 0;
int max_recursion_depth = 0;

// Функция обмена элементов
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    moves += 3; // 3 пересылки при обмене
}

// Функция для нахождения медианы трех элементов
int median_of_three(int arr[], int left, int right) {
    int mid = left + (right - left) / 2;
    
    // Сравниваем три элемента и находим медиану
    comparisons += 2; // два сравнения для медианы
    if (arr[left] > arr[mid]) {
        comparisons++;
        if (arr[mid] > arr[right]) return mid;
        else if (arr[left] > arr[right]) return right;
        else return left;
    } else {
        comparisons++;
        if (arr[left] > arr[right]) return left;
        else if (arr[mid] > arr[right]) return right;
        else return mid;
    }
}

// Функция разделения массива
int partition(int arr[], int left, int right, int current_depth) {
    if (current_depth > max_recursion_depth) {
        max_recursion_depth = current_depth;
    }
    
    // Выбираем медиану в качестве опорного элемента
    int pivot_index = median_of_three(arr, left, right);
    int pivot = arr[pivot_index];
    
    // Перемещаем медиану в конец для удобства
    if (pivot_index != right) {
        swap(&arr[pivot_index], &arr[right]);
    }
    
    int i = left - 1;
    
    for (int j = left; j < right; j++) {
        comparisons++;
        if (arr[j] <= pivot) {
            i++;
            if (i != j) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
    
    // Возвращаем опорный элемент на правильную позицию
    swap(&arr[i + 1], &arr[right]);
    
    return i + 1;
}

// QuickSort (первая версия)
void quick_sort_v1(int arr[], int left, int right, int depth) {
    if (depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    
    comparisons++;
    if (left < right) {
        int pivot_index = partition(arr, left, right, depth + 1);
        
        // Рекурсивно сортируем левую и правую части
        quick_sort_v1(arr, left, pivot_index - 1, depth + 1);
        quick_sort_v1(arr, pivot_index + 1, right, depth + 1);
    }
}

// QuickSort (вторая версия) - с оптимизацией глубины рекурсии
void quick_sort_v2(int arr[], int left, int right, int depth) {
    if (depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    
    while (left < right) {
        comparisons++;
        
        int pivot_index = partition(arr, left, right, depth + 1);
        
        // Рекурсивно обрабатываем меньшую часть, итеративно - большую
        if (pivot_index - left < right - pivot_index) {
            quick_sort_v2(arr, left, pivot_index - 1, depth + 1);
            left = pivot_index + 1;
        } else {
            quick_sort_v2(arr, pivot_index + 1, right, depth + 1);
            right = pivot_index - 1;
        }
    }
}

// Функция для проверки правильности сортировки
void check_sorting(int arr[], int n, int* series_count, long long* checksum) {
    *series_count = 1;
    *checksum = 0;
    
    for (int i = 0; i < n; i++) {
        *checksum += arr[i];
        if (i > 0 && arr[i] < arr[i - 1]) {
            (*series_count)++;
        }
    }
}

// Функции для генерации массивов
void generate_decreasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void generate_increasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

void generate_random(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

// Функция для копирования массива
void copy_array(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

// Функция сброса счетчиков
void reset_counters() {
    comparisons = 0;
    moves = 0;
    max_recursion_depth = 0;
}

int main() {
    srand(time(NULL));
    
    int sizes[] = {100, 200, 300, 400, 500};
    int num_sizes = 5;
    
    printf("Исследование трудоемкости метода Хоара\n");
    printf("======================================\n\n");
    
    // Таблица для трудоемкости
    printf("Таблица 1: Трудоемкость QuickSort (Mф + Сф)\n");
    printf("+------+----------+----------+----------+\n");
    printf("|  N   | Убыв.    | Возр.    | Случ.    |\n");
    printf("+------+----------+----------+----------+\n");
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int* original = malloc(n * sizeof(int));
        int* test_arr = malloc(n * sizeof(int));
        
        printf("| %4d ", n);
        
        // Тестируем на трех типах массивов
        int types = 3;
        for (int type = 0; type < types; type++) {
            // Генерируем массив
            if (type == 0) generate_decreasing(original, n);
            else if (type == 1) generate_increasing(original, n);
            else generate_random(original, n);
            
            copy_array(original, test_arr, n);
            reset_counters();
            
            quick_sort_v1(test_arr, 0, n - 1, 1);
            
            // Проверяем корректность сортировки
            int series_count;
            long long checksum;
            check_sorting(test_arr, n, &series_count, &checksum);
            
            printf("| %8lld ", comparisons + moves);
        }
        printf("|\n");
        
        free(original);
        free(test_arr);
    }
    printf("+------+----------+----------+----------+\n\n");
    
    // Таблица для глубины рекурсии
    printf("Таблица 2: Глубина рекурсии\n");
    printf("+------+----------+----------+----------+----------+----------+----------+\n");
    printf("|  n   | QuickSort| QuickSort| QuickSort| QuickSort2|QuickSort2|QuickSort2|\n");
    printf("|      | Убыв.    | Случ.    | Возр.    | Убыв.     | Случ.    | Возр.    |\n");
    printf("+------+----------+----------+----------+-----------+----------+----------+\n");
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int* original = malloc(n * sizeof(int));
        int* test_arr = malloc(n * sizeof(int));
        
        printf("| %4d ", n);
        
        // Тестируем обе версии на трех типах массивов
        for (int version = 0; version < 2; version++) {
            for (int type = 0; type < 3; type++) {
                // Генерируем массив
                if (type == 0) generate_decreasing(original, n);
                else if (type == 1) generate_increasing(original, n);
                else generate_random(original, n);
                
                copy_array(original, test_arr, n);
                reset_counters();
                
                if (version == 0) {
                    quick_sort_v1(test_arr, 0, n - 1, 1);
                } else {
                    quick_sort_v2(test_arr, 0, n - 1, 1);
                }
                
                printf("| %8d ", max_recursion_depth);
            }
        }
        printf("|\n");
        
        free(original);
        free(test_arr);
    }
    printf("+------+----------+----------+----------+-----------+----------+----------+\n");
    
    return 0;
}