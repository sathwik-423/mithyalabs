#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LENGTH 50

struct Node {
    char data[MAX_DATA_LENGTH];
    struct Node* child;
    struct Node* sibling;
};

struct Node* createNode(char* data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode != NULL) {
        strcpy(newNode->data, data);
        newNode->child = NULL;
        newNode->sibling = NULL;
    }
    return newNode;
}

void addChild(struct Node* parent, struct Node* child) {
    if (parent == NULL || child == NULL) return;
    if (parent->child == NULL) {
        parent->child = child;
    } else {
        struct Node* temp = parent->child;
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

void deleteNode(struct Node* node) {
    if (node == NULL) return;
    deleteNode(node->child);
    deleteNode(node->sibling);
    free(node);
}

void getPath(struct Node* root, struct Node* node, char path[]) {
    if (root == NULL || node == NULL) return;
    if (root == node) {
        strcat(path, node->data);
        return;
    }
    struct Node* temp = root->child;
    while (temp != NULL) {
        char tempPath[MAX_DATA_LENGTH];
        strcpy(tempPath, path);
        getPath(temp, node, tempPath);
        if (strlen(tempPath) > strlen(path)) {
            strcpy(path, tempPath);
            strcat(path, "/");
            strcat(path, node->data);
            return;
        }
        temp = temp->sibling;
    }
}

int countNodes(struct Node* node) {
    if (node == NULL) return 0;
    int count = 1;
    count += countNodes(node->child);
    count += countNodes(node->sibling);
    return count;
}

void reorderChildren(struct Node* node, int order[]) {
    if (node == NULL || node->child == NULL) return;
    struct Node* temp = node->child;
    struct Node* prev = NULL;
    while (temp != NULL) {
        struct Node* next = temp->sibling;
        temp->sibling = prev;
        prev = temp;
        temp = next;
    }
    node->child = prev;
}

void moveChild(struct Node* oldParent, struct Node* newParent, struct Node* child) {
    if (oldParent == NULL || newParent == NULL || child == NULL) return;
    if (oldParent->child == child) {
        oldParent->child = child->sibling;
    } else {
        struct Node* temp = oldParent->child;
        while (temp != NULL && temp->sibling != child) {
            temp = temp->sibling;
        }
        if (temp != NULL) {
            temp->sibling = child->sibling;
        }
    }
    child->sibling = NULL;
    addChild(newParent, child);
}

struct Node* searchNode(struct Node* root, char* search) {
    if (root == NULL) return NULL;
    if (strcmp(root->data, search) == 0) return root;
    struct Node* temp = root->child;
    while (temp != NULL) {
        struct Node* result = searchNode(temp, search);
        if (result != NULL) return result;
        temp = temp->sibling;
    }
    return NULL;
}

void displayTree(struct Node* root, int level) {
    if (root == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", root->data);
    displayTree(root->child, level + 1);
    displayTree(root->sibling, level);
}

void performOperations(struct Node* root) {
    int choice;
    do {
        printf("\nOperations Menu:\n");
        printf("1. Add a child node\n");
        printf("2. Delete a node\n");
        printf("3. Get the path of a node\n");
        printf("4. Get the count of nodes in a subtree\n");
        printf("5. Reorder children of a node\n");
        printf("6. Move a child to another parent\n");
        printf("7. Search for a node\n");
        printf("8. Display tree\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char data[MAX_DATA_LENGTH];
                printf("Enter data for the new node: ");
                scanf("%s", data);
                struct Node* newNode = createNode(data);
                addChild(root, newNode);
                printf("Node added successfully!\n");
                break;
            }
            case 2: {
                char data[MAX_DATA_LENGTH];
                printf("Enter data of the node to delete: ");
                scanf("%s", data);
                struct Node* nodeToDelete = searchNode(root, data);
                if (nodeToDelete != NULL) {
                    deleteNode(nodeToDelete);
                    printf("Node deleted successfully!\n");
                } else {
                    printf("Node not found!\n");
                }
                break;
            }
            case 3: {
                char data[MAX_DATA_LENGTH];
                printf("Enter data of the node to get path: ");
                scanf("%s", data);
                struct Node* nodeToFind = searchNode(root, data);
                if (nodeToFind != NULL) {
                    char path[MAX_DATA_LENGTH * 100];
                    strcpy(path, "");
                    getPath(root, nodeToFind, path);
                    printf("Path of the node: %s\n", path);
                } else {
                    printf("Node not found!\n");
                }
                break;
            }
            case 4: {
                int count = countNodes(root);
                printf("Count of nodes in subtree: %d\n", count);
                break;
            }
            case 5: {
                int order[MAX_DATA_LENGTH];
                printf("Enter the new order of children: ");
                for (int i = 0; i < MAX_DATA_LENGTH; i++) {
                    scanf("%d", &order[i]);
                    if (order[i] == 0) break;
                }
                reorderChildren(root, order);
                printf("Children reordered successfully!\n");
                break;
            }
            case 6: {
                char oldParentData[MAX_DATA_LENGTH], newParentData[MAX_DATA_LENGTH], childData[MAX_DATA_LENGTH];
                printf("Enter data of old parent node: ");
                scanf("%s", oldParentData);
                printf("Enter data of new parent node: ");
                scanf("%s", newParentData);
                printf("Enter data of the child node: ");
                scanf("%s", childData);
                struct Node* oldParent = searchNode(root, oldParentData);
                struct Node* newParent = searchNode(root, newParentData);
                struct Node* child = searchNode(root, childData);
                if (oldParent != NULL && newParent != NULL && child != NULL) {
                    moveChild(oldParent, newParent, child);
                    printf("Child moved successfully!\n");
                } else {
                    printf("Nodes not found!\n");
                }
                break;
            }
            case 7: {
                char search[MAX_DATA_LENGTH];
                printf("Enter data to search: ");
                scanf("%s", search);
                struct Node* result = searchNode(root, search);
                if (result != NULL) {
                    printf("Node found!\n");
                } else {
                    printf("Node not found!\n");
                }
                break;
            }
            case 8: {
                printf("Tree:\n");
                displayTree(root, 0);
                break;
            }
            case 0: {
                printf("Exiting...\n");
                break;
            }
            default: {
                printf("Invalid choice! Please try again.\n");
                break;
            }
        }
    } while (choice != 0);
}

struct Node* createDummyTree() {
    struct Node* root = createNode("Root");
    struct Node* math = createNode("Math");
    struct Node* science = createNode("Science");
    struct Node* history = createNode("History");
    struct Node* algebra = createNode("Algebra");
    struct Node* calculus = createNode("Calculus");
    struct Node* physics = createNode("Physics");
    struct Node* chemistry = createNode("Chemistry");
    struct Node* worldHistory = createNode("World History");
    struct Node* usHistory = createNode("US History");
    struct Node* geometry = createNode("Geometry");

    addChild(root, math);
    addChild(root, science);
    addChild(root, history);

    addChild(math, algebra);
    addChild(math, calculus);
    addChild(science, physics);
    addChild(science, chemistry);
    addChild(history, worldHistory);
    addChild(history, usHistory);

    addChild(algebra, geometry);

    return root;
}

int main() {
    struct Node* root = createDummyTree();
    performOperations(root);
    deleteNode(root);
    return 0;
}

