#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//
typedef struct _T_Node {
    int ASCII;
    unsigned long long count;
    struct _T_Node * left;
    struct _T_Node * right;
} Tnode;

typedef struct _Tree {
    struct _T_Node * root;
} Tree;

typedef struct _Leaf_Node {
    struct _Tree * data;
    struct _Leaf_Node * next;
} LeafNode;

typedef struct _LinkedList {
    struct _Leaf_Node * start;
} LinkedList;

//
void PQ_enqueue(LinkedList * dataList, LeafNode * node);
void PQ_dequeue(LinkedList * dataList, LeafNode * node);
FILE * openFile(char * filename);
LinkedList * countCharacters(FILE * file);
Tree * convertToBST(LinkedList * list);

void printList(LinkedList * dataList);






void printList(LinkedList * dataList) {
    LeafNode * start = dataList->start;
    while(start != NULL)
    {
        printf("%c %llu\n", start->data->root->ASCII, start->data->root->count);
        start = start -> next;
    }
}

int getBit(int position)
{
    return position % 8 ? position % 8 : 8;
}

void printBits(FILE * fp, char * preorder, int * writePosition, unsigned char * writingBit, int useTrigger) {
    int k = strlen(preorder);
    int trigger [2] = {0};
    int i = 0;
    
    for(i = 0; i < k; i++) {
        char data = preorder[i];
        if((useTrigger && (!trigger[0] || !trigger[1])) || (!useTrigger && (data == '1' || data == '0'))) {
            if(data == '1') {
                trigger[1] = trigger[0];
                trigger[0] = 1;
                *writingBit = *writingBit | 1;
            } else {
                trigger[1] = 0; trigger[0] = 0;
            }
            (*writePosition)++;
            if(*writePosition == 8) {
                *writePosition = 0;
                fwrite(writingBit, sizeof(char), 1, fp);
            }
            *writingBit = *writingBit << 1;
        } else {
            trigger[1] = 0; trigger[0] = 0;
            *writingBit = *writingBit << (7 - *writePosition);
            char toBit = data >> *writePosition;
            *writingBit = *writingBit | toBit;
            fwrite(writingBit, sizeof(char), 1, fp);
            *writingBit = data << 1;
        }
    }
}

void traverseTree(struct _T_Node * node, char ** table, char * arr, char * total, FILE * fp) {
    int len = strlen(arr);
    if(node->left == NULL && node->right == NULL) {
        table[node->ASCII] = malloc(sizeof(char) * 37);
        strcpy(table[node->ASCII], arr);
        
        int totalLen = strlen(total);
        total[totalLen] = '1';
        total[totalLen + 1] = '1';
        total[totalLen + 2] = node->ASCII;
        total[totalLen + 3] = '\0';
        return;
    }
    
    int totalLen = strlen(total);
    total[totalLen] = '0';
    total[totalLen + 1] = '\0';
    
    if(node->left != NULL) {
        arr[len] = '0';
        arr[len + 1] = '\0';
        traverseTree(node->left, table, arr, total, fp);
    }
    if(node->right != NULL) {
        arr[len] = '1';
        arr[len + 1] = '\0';
        traverseTree(node->right, table, arr, total, fp);
    }
}


void _destroy_treenode(Tnode* tree)
{
    if(tree == NULL)
    {
        return;
    }
    _destroy_treenode(tree -> left);
    _destroy_treenode(tree -> right);
    free(tree);
}




int main(int argc, char ** argv) {
    
    if(argc != 2) {
        return EXIT_FAILURE;
    }
    
    FILE * inputFile = openFile(argv[1]);
    LinkedList * list = countCharacters(inputFile);
    Tree * tree = convertToBST(list);
    
    char * filename = malloc(sizeof(char) * (strlen(argv[1]) + 5));
    strcpy(filename, argv[1]);
    strcpy(&(filename[strlen(argv[1])]), ".huff");
    
    FILE * fp = fopen(filename, "w");
    char ** arr = malloc(sizeof(char*) * 256);
    char * preorderTraversal = malloc(sizeof(char) * 1024);
    char * dump = malloc(sizeof(char) * 37);
    dump[0] = '\0';
    preorderTraversal[0] = '\0';
    
    unsigned char writingBit = 0;
    int writingPosition = 0;
    traverseTree(tree->root, arr, dump, preorderTraversal, fp);
    printBits(fp, preorderTraversal, &writingPosition, &writingBit, 1);
    printBits(fp, "10", &writingPosition, &writingBit, 0);
    
    fseek(inputFile, 0, SEEK_SET);
    char data [2];
    data[1] = '\0';
    while((data[0] = fgetc(inputFile)) != EOF) {
        printBits(fp, "1", &writingPosition, &writingBit, 0);
        printBits(fp, arr[(int) data[0]], &writingPosition, &writingBit, 0);
    }
    
    printBits(fp, "0", &writingPosition, &writingBit, 0);
    
    writingBit = writingBit << (7 - writingPosition);
    fwrite(&writingBit, sizeof(char), 1, fp);
    fclose(fp);
    
    free(preorderTraversal);
    free(arr);
    free(dump);
  //  Tnode* temp_free = tree -> root;
    
   
    /*
    LeafNode* temp_free_ = list -> start;
    
    while( temp_free -> next != NULL)
    {
        LeafNode* temp2 = temp_free;
        temp_free = temp_free -> next;
        free(temp2);
    }
    */
    
    
    return EXIT_SUCCESS;
}

void PQ_enqueue(LinkedList * dataList, LeafNode * node) {
    
    LeafNode * start = dataList->start;
    if(start == NULL || start->data->root->count > node->data->root->count) {
        dataList->start = node;
        node->next = start;
    } else {
        while(1) {
            if(start->next == NULL || start->next->data->root->count > node->data->root->count) {
                node->next = start->next;
                start->next = node;
                break;
            }
            start = start->next;
        }
    }
}

void PQ_dequeue(LinkedList * dataList, LeafNode * node) {
    LeafNode * start = dataList->start;
    if(dataList->start == node) {
        dataList->start = node->next;
    } else {
        while(start->next != NULL) {
            if(start->next == node) {
                start->next = node->next;
                break;
            }
        }
    }
}

Tree * convertToBST(LinkedList * list) {
    LeafNode * currNode = list->start;
    Tnode* temp = NULL;
    while(list->start->next != NULL)
    {
        Tree * node_left = currNode->data;
        Tree * node_right = currNode->next->data;
        
        Tree * newTree = malloc(sizeof(Tree));
        Tnode * root = malloc(sizeof(Tnode));
        temp = root;
        root->ASCII = -1;
        root->count = node_left->root->count + node_right->root->count;
        root->left = node_left->root;
        root->right = node_right->root;
        newTree->root = root;
        
        LeafNode * LeafNode = malloc(sizeof(LeafNode));
        LeafNode->data = newTree;
        LeafNode->next = NULL;
        
        PQ_enqueue(list, LeafNode);
        
        PQ_dequeue(list, currNode);
        PQ_dequeue(list, currNode->next);
        
        currNode = list->start;
    }
    

    
    return list->start->data;
}

FILE * openFile(char * filename) {
    return fopen(filename, "r");
}

LinkedList * countCharacters(FILE * file) {
    fseek(file, 0, SEEK_SET);
    unsigned long long dataArray [256] = {0};
    char data;
    int i = 0;
    
    while((data = fgetc(file)) != EOF) {
        dataArray[(int) data]++;
    }
    
    LinkedList * dataList = malloc(sizeof(LinkedList));
    dataList->start = NULL;
    
    for(i = 0; i < 256; i++) {
        if(dataArray[i]) {
            
            Tnode * node = malloc(sizeof(Tnode));
            node->ASCII = i;
            node->count = dataArray[i];
            node->left = NULL;
            node->right = NULL;
            
            Tree * tree = malloc(sizeof(Tree));
            tree->root = node;
            
            LeafNode * LeafNode = malloc(sizeof(LeafNode));
            LeafNode->data = tree;
            LeafNode->next = NULL;
            
            PQ_enqueue(dataList, LeafNode);
        }
    }
    return dataList;
}
