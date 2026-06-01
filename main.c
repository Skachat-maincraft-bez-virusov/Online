#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "mylib.h"
#pragma pack(push, 1)
typedef struct {
    uint32_t timestamp_ms;
    float lat_rad;
    float lon_rad;
    float alt_m;
} record_t;
#pragma pack(pop)
int main() {
    FILE *fin = fopen("flight_data5.bin", "rb");
    FILE *fout = fopen("wwwwwwwwww.txt", "w");
    if (!fin || !fout) {
        printf("File open error\n");
        return 1;
    }
    Node* list = NULL;
    record_t r;
    // 1) Преобразование бинарного файла в односвязный список
    while (fread(&r, sizeof(record_t), 1, fin) == 1) {
        appendList(&list, r.timestamp_ms, r.lat_rad, r.lon_rad, r.alt_m);
    }
    if (!list) {
        printf("No data\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    // 2) Построение бинарного дерева по высоте
    TreeNode* root = NULL;
    Node* cur = list;
    while (cur) {
        root = insertTree(root, cur->alt);
        cur = cur->next;
    }
    float min_h = findMin(root);
    float max_h = findMax(root);
    // 3) Расчёт статистики
    int count = 0;         
    uint32_t start = UINT32_MAX;
    uint32_t end = 0;
    cur = list;
    while (cur) {
        count++;
        if (cur->time < start) start = cur->time;
        if (cur->time > end)   end   = cur->time;
        cur = cur->next;
    }
    uint32_t duration = end - start;
    // 4) Вывод статистики в консоль
    printf("Number of points: %d\n", count);
    printf("Flight duration: %u ms\n", duration);
    printf("Max altitude: %.2f\n", max_h);
    printf("Min altitude: %.2f\n", min_h);
    // 5) Запись статистики в текстовый файл
    fprintf(fout, "Number of points: %d\n", count);
    fprintf(fout, "Flight duration: %u ms\n", duration);
    fprintf(fout, "Max altitude: %.2f\n", max_h);
    fprintf(fout, "Min altitude: %.2f\n", min_h);
    fclose(fin);
    fclose(fout);
    freeList(list);
    freeTree(root);
    return 0;
}
