#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    Node* tail;
    int size;
} List;

// 初始化链表
void init_list(List* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// 创建新节点
Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->prev = new_node->next = NULL;
    return new_node;
}

// 按升序插入（保持有序）
void insert_sorted(List* list, int data) {
    Node* new_node = create_node(data);
    if (!list->head) {
        list->head = list->tail = new_node;
        list->size++;
        return;
    }

    Node* current = list->head;
    while (current && current->data < data) {
        current = current->next;
    }

    if (!current) { // 插入尾部
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    } else if (current == list->head) { // 插入头部
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    } else { // 插入中间
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;
    }

    list->size++;
}

// 查找节点
Node* findNode(List* list, int data) {
    Node* current = list->head;
    while (current) {
        if (current->data == data)
            return current;
        else if (current->data > data)
            break; // 因为有序
        current = current->next;
    }
    return NULL;
}

// 删除节点
void deleteNode(List* list, Node* node) {
    if (!node) return;
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node);
    list->size--;
}

// 释放链表
void freeList(List* list) {
    Node* cur = list->head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

int main() {
    List list;
    init_list(&list);
    char buffer[2000000];

    // 读取第一行图书ID（初始馆藏）
    if (fgets(buffer, sizeof(buffer), stdin)) {
        char* token = strtok(buffer, " \n");
        while (token) {
            int id = atoi(token);
            insert_sorted(&list, id);
            token = strtok(NULL, " \n");
        }
    }

    int m;
    scanf("%d", &m);

    char result[1000005];
    int idx = 0;

    for (int i = 0; i < m; ++i) {
        int op, id;
        scanf("%d %d", &op, &id);

        Node* found = findNode(&list, id);

        if (op == 1) { // 查询
            result[idx++] = found ? '1' : '0';
        } else if (op == 2) { // 借出
            if (found) {
                deleteNode(&list, found);
                result[idx++] = '1';
            } else {
                result[idx++] = '0';
            }
        } else if (op == 3) { // 归还
            if (!found) {
                insert_sorted(&list, id);
                result[idx++] = '1';
            } else {
                result[idx++] = '0';
            }
        } else {
            result[idx++] = '0'; // 非法操作
        }
    }

    result[idx] = '\0';
    printf("%s\n", result);

    freeList(&list);
    return 0;
}
