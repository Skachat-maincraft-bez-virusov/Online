#ifndef MYLIB_H
#define MYLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node {
    uint32_t time;
    float lat;
    float lon;
    float alt;
    struct Node* next;
} Node;

Node* createNode(uint32_t time, float lat, float lon, float alt);
void appendList(Node** head, uint32_t time, float lat, float lon, float alt);
void freeList(Node* head);

typedef struct TreeNode {
    float data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createTreeNode(float data);
TreeNode* insertTree(TreeNode* root, float data);
float findMin(TreeNode* root);
float findMax(TreeNode* root);
void freeTree(TreeNode* root);
#endif