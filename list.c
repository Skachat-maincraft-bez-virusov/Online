#include <stdlib.h>
#include "mylib.h"
Node* createNode(uint32_t time, float lat, float lon, float alt) {
    Node* newNode = malloc(sizeof(Node));
    newNode->time = time;
    newNode->lat = lat;
    newNode->lon = lon;
    newNode->alt = alt;
    newNode->next = NULL;
    return newNode;
}
void appendList(Node** head, uint32_t time, float lat, float lon, float alt) {
    Node* newNode = createNode(time, lat, lon, alt);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = newNode;
}
void freeList(Node* head) {
    while (head) {
        Node* tmp = head;
        head = head->next;
        free(tmp);
    }
}