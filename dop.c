#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "mylib.h"

#pragma pack(push, 1)
typedef struct {
    uint32_t timestamp_ms;
    float lat_rad;
    float lon_rad;
    float alt_m;
} record_t;
#pragma pack(pop)

int main(int argc, char *argv[]) {
    // проверка аргументов
    if (argc < 2 || argc > 3) {
        return 1;
    }

    const char *inputFilename = argv[1];
    const char *outputFilename = (argc == 3) ? argv[2] : "output.txt";

    // открытие файлов
    FILE *fin = fopen(inputFilename, "rb");
    FILE *fout = fopen(outputFilename, "w");
    
    // ошибка открытия входного файла
    if (!fin) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", inputFilename);
        return 1;
    }
    if (!fout) {
        fprintf(stderr, "Error: Cannot create output file '%s'\n", outputFilename);
        fclose(fin);
        return 1;
    }

    // чтение данных в список
    Node* list = NULL;
    record_t r;
    
    while (fread(&r, sizeof(record_t), 1, fin) == 1) {
        appendList(&list, r.timestamp_ms, r.lat_rad, r.lon_rad, r.alt_m);
    }
    
    // если файл пустой
    if (!list) {
        printf("No data found in file '%s'\n", inputFilename);
        fclose(fin);
        fclose(fout);
        return 1;
    }
    
    // построение дерева
    TreeNode* root = NULL;
    Node* cur = list;
    while (cur) {
        root = insertTree(root, cur->alt);
        cur = cur->next;
    }
    
    float min_h = findMin(root);
    float max_h = findMax(root);
    
    // расчет статистики
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
    
    // вывод в консоль
    printf("Input file: %s\n", inputFilename);
    printf("Number of points: %d\n", count);
    printf("Flight duration: %u ms\n", duration);
    printf("Max altitude: %.2f m\n", max_h);
    printf("Min altitude: %.2f m\n", min_h);
    
    // запись в файл
    fprintf(fout, "Input file: %s\n", inputFilename);
    fprintf(fout, "Number of points: %d\n", count);
    fprintf(fout, "Flight duration: %u ms\n", duration);
    fprintf(fout, "Max altitude: %.2f m\n", max_h);
    fprintf(fout, "Min altitude: %.2f m\n", min_h);
    
    // завершение
    fclose(fin);
    fclose(fout);
    freeList(list);
    freeTree(root);
    
    return 0;
}