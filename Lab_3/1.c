#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Счётчики для измерений
long long comparisons = 0;
long long moves = 0;

// Функция обмена двух элементов местами
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    moves += 3;
}

// Находим средний из трёх элементов (медиану)
int find_median(int arr[], int left, int right) {
    int mid = left + (right - left) / 2;
    
    comparisons += 2;
    
    // Сравниваем три элемента и находим средний
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

// Основная функция разделения массива
int partition(int arr[], int left, int right) {
    // Выбираем опорный элемент как медиану трёх
    int pivot_index = find_median(arr, left, right);
    int pivot_value = arr[pivot_index];
    
    // Перемещаем медиану в конец для удобства
    if (pivot_index != right) {
        swap(&arr[pivot_index], &arr[right]);
    }
    
    int i = left;  // Указатель на конец левой части (элементы ≤ pivot)
    
    // Проходим по всем элементам
    for (int j = left; j < right; j++) {
        comparisons++;  // Считаем сравнение
        if (arr[j] <= pivot_value) {
            // Если элемент меньше или равен pivot, перемещаем его в левую часть
            if (i != j) {
                swap(&arr[i], &arr[j]);
            }
            i++;  // Сдвигаем границу левой части
        }
    }
    
    // Ставим pivot на правильное место
    swap(&arr[i], &arr[right]);
    
    return i;  // Возвращаем позицию pivot
}

// QuickSort (основная версия)
void quick_sort(int arr[], int left, int right) {
    if (left < right) {
        // Разделяем массив и получаем позицию pivot
        int pivot_pos = partition(arr, left, right);
        
        // Рекурсивно сортируем левую и правую части
        quick_sort(arr, left, pivot_pos - 1);
        quick_sort(arr, pivot_pos + 1, right);
    }
}

// Проверяем правильность сортировки
void check_sort(int arr[], int n, int* series, long long* sum) {
    *series = 1;    // Начинаем с одной серии
    *sum = 0;       // Обнуляем сумму
    
    for (int i = 0; i < n; i++) {
        *sum += arr[i];  // Считаем сумму всех элементов
        
        // Если текущий элемент меньше предыдущего - это новая серия
        if (i > 0 && arr[i] < arr[i - 1]) {
            (*series)++;
        }
    }
}

// Создаём убывающий массив
void make_decreasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // Первый элемент = n, последний = 1
    }
}

// Создаём возрастающий массив  
void make_increasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;  // Первый элемент = 1, последний = n
    }
}

// Создаём случайный массив
void make_random(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Случайные числа от 0 до 999
    }
}

// Копируем массив
void copy_array(int from[], int to[], int n) {
    for (int i = 0; i < n; i++) {
        to[i] = from[i];  // Копируем каждый элемент
    }
}

// Обнуляем счётчики перед каждым тестом
void reset_counters() {
    comparisons = 0;
    moves = 0;
}

int main() {
    srand(time(NULL));  // Инициализируем генератор случайных чисел
    
    // Размеры массивов для тестирования
    int sizes[] = {100, 200, 300, 400, 500};
    int num_sizes = 5;
    
    printf("=== ИССЛЕДОВАНИЕ БЫСТРОЙ СОРТИРОВКИ ===\n\n");
    
    // Таблица: Трудоёмкость (Mф + Сф)
    printf("Таблица: Сумма пересылок и сравнений (Mф + Сф)\n");
    printf("+------+----------+----------+----------+\n");
    printf("| Размер | Убывающий | Возрастающий | Случайный |\n");
    printf("+------+----------+----------+----------+\n");
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        
        // Выделяем память для массивов
        int* original = (int*)malloc(n * sizeof(int));
        int* test_arr = (int*)malloc(n * sizeof(int));
        
        printf("| %4d ", n);
        
        // Тестируем на трёх типах массивов
        for (int type = 0; type < 3; type++) {
            // Создаём массив нужного типа
            if (type == 0) make_decreasing(original, n);
            else if (type == 1) make_increasing(original, n);
            else make_random(original, n);
            
            // Копируем для тестирования
            copy_array(original, test_arr, n);
            reset_counters();  // Обнуляем счётчики
            
            // Сортируем
            quick_sort(test_arr, 0, n - 1);
            
            // Проверяем правильность
            int series;
            long long checksum;
            check_sort(test_arr, n, &series, &checksum);
            
            // Выводим общую трудоёмкость
            printf("| %8lld ", comparisons + moves);
        }
        printf("|\n");
        
        // Освобождаем память
        free(original);
        free(test_arr);
    }
    printf("+------+----------+----------+----------+\n\n");
    
    return 0;
}