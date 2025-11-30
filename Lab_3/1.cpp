#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility> 

int M, C;

void reset_counters() {
    M = 0; C = 0;
}

// Медиана трёх
int medianOfThree(int arr[], int L, int mid, int R) {
    if (arr[L] > arr[mid]) { int temp = arr[L]; arr[L] = arr[mid]; arr[mid] = temp; M += 3; }
    if (arr[L] > arr[R]) { int temp = arr[L]; arr[L] = arr[R]; arr[R] = temp; M += 3; }
    if (arr[mid] > arr[R]) { int temp = arr[mid]; arr[mid] = arr[R]; arr[R] = temp; M += 3; }
    return arr[mid];  // Медиана
}

// Partition с медианой
int partition(int arr[], int L, int R) {
    int mid = L + (R - L) / 2;
    int x = medianOfThree(arr, L, mid, R); C += 6;  // Сравнения в median
    int i = L, j = R;
    while (i <= j) { C++;
        while (arr[i] < x) { C++; i++; } C++;
        while (arr[j] > x) { C++; j--; } C++;
        if (i <= j) { C++;
            int temp = arr[i]; M++; arr[i] = arr[j]; M++; arr[j] = temp; M++;
            i++; j--;
        }
    }
    return i;
}

// QuickSort вторая версия
void quickSort(int arr[], int L, int R) {
    while (L < R) {
        int m = partition(arr, L, R);
        if (m - L < R - m) {
            quickSort(arr, L, m - 1);
            L = m;
        } else {
            quickSort(arr, m, R);
            R = m - 1;
        }
    }
}

// Full
std::pair<int, int> fullQuickSort(int arr[], int n) {
    reset_counters();
    quickSort(arr, 0, n - 1);
    return {C, M};
}

// Theor худший
double theor_worst(int n) {
    return n * n / 2.0;
}

// Generate как раньше
void generateArray(int arr[], int n, int type) {
    if (type == 0) { for (int i = 0; i < n; i++) arr[i] = n - i; }
    else if (type == 1) { for (int i = 0; i < n; i++) arr[i] = rand() % 1000; }
    else { for (int i = 0; i < n; i++) arr[i] = i + 1; }
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    std::cout << "\nТрудоемкость QuickSort (M+C):\n";
    std::cout << "+-----+----------------+-------+-------+-------+\n";
    std::cout << "| N   | Theor (худший) | Убыв  | Возр  | Случ  |\n";
    std::cout << "+-----+----------------+-------+-------+-------+\n";
    for (int n : sizes) {
        int* arr1 = new int[n]; generateArray(arr1, n, 0); auto res1 = fullQuickSort(arr1, n); int mc1 = res1.first + res1.second; delete[] arr1;
        int* arr2 = new int[n]; generateArray(arr2, n, 2); auto res2 = fullQuickSort(arr2, n); int mc2 = res2.first + res2.second; delete[] arr2;
        int* arr3 = new int[n]; generateArray(arr3, n, 1); auto res3 = fullQuickSort(arr3, n); int mc3 = res3.first + res3.second; delete[] arr3;
        std::cout << "| " << n << " | " << theor_worst(n) << "           | " << mc1 << "  | " << mc2 << "  | " << mc3 << "  |\n";
    }
    std::cout << "+-----+----------------+-------+-------+-------+\n";
    return 0;
}