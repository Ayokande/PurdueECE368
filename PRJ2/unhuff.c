#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _Tree_Node {
    int ASCII;
    unsigned long long count;
    struct _Tree_Node * left;
    struct _Tree_Node * right;
} Tnode;

typedef struct _Tree {
    struct _Tree_Node * root;
} Tree;

typedef struct _Leaf_Node {
    struct _Tree * data;
    struct _Leaf_Node * next;
} LeafNode;

typedef struct _LinkedList {
    struct _Leaf_Node * start;
} LinkedList;

void incrementChar(FILE* input, int * move, unsigned char * c, unsigned char * n) {
    if(++(*move) == 8) {
        *move = 0;
        *c = *n;
        *n = fgetc(input);
    } else {
        *c = (*c << 1);
    }
}

Tnode * buildTree(char * data, int * k) {
    if(data[*k] == '0') {
        ++(*k);
        Tnode * left = buildTree(data, k);
        ++(*k);
        Tnode * right = buildTree(data, k);
        
        Tnode * root = malloc(sizeof(Tnode));
        root->left = left;
        root->right = right;
        return root;
        
    } else if(data[*k] == '1') {
        (*k)++;
        Tnode * leaf = malloc(sizeof(Tnode));
        leaf->left = NULL;
        leaf->right = NULL;
        leaf->ASCII = data[*k];
        return leaf;
    } else {
        return NULL;
    }
}

void buildData(FILE * input, char * str) {
    
    char * preorder = malloc(sizeof(char) * 1024);
    
    unsigned char c = fgetc(input);
    unsigned char nextChar = fgetc(input);
    int move = 0;
    int i = 0;
    
    while(1) {
        if(c >> 7 == 0) {
            
            preorder[i] = '0';
            preorder[i + 1] = '\0';
            i++;
            
            incrementChar(input, &move, &c, &nextChar);
        } else {
            if(c >> 6 == 2) {
                incrementChar(input, &move, &c, &nextChar);
                incrementChar(input, &move, &c, &nextChar);
                break;
            }
            
            incrementChar(input, &move, &c, &nextChar);
            incrementChar(input, &move, &c, &nextChar);
            c = c | (nextChar >> (8 - move));
            
            preorder[i] = '1';
            preorder[i + 1] = c;
            preorder[i + 2] = '\0';
            i += 2;
            
            c = nextChar << move;
            nextChar = fgetc(input);
            c += nextChar >> (8 - move);
        }
    }
    int len = 0;
    Tnode * head = buildTree(preorder, &len);
    Tnode * currNode = head;
    
    char * filename = malloc(sizeof(char) * (strlen(str) + 7));
    strcpy(filename, str);
    strcpy(&(filename[strlen(str)]), ".unhuff");
    
    FILE * output = fopen(filename, "w");
    
    if(c >> 7 == 0) {
        return;
    } else {
        incrementChar(input, &move, &c, &nextChar);
    }
    
    while(1) {
        
        if(currNode->left == NULL && currNode->right == NULL)
        {
            fprintf(output, "%c", currNode->ASCII);
            currNode = head;
            if(c >> 7 == 0) {
                break;
            } else {
                incrementChar(input, &move, &c, &nextChar);
            }
        }
        if(c >> 7 == 0)
        {
            currNode = currNode->left;
        }
        else
        {
            currNode = currNode->right;
        }
        incrementChar(input, &move, &c, &nextChar);
    }
    
    fclose(output);
}

int main(int argc, char ** argv) {
    
    if(argc != 2) {
        return EXIT_FAILURE;
    }
    
    FILE * input = fopen(argv[1], "r");
    buildData(input, argv[1]);
    
    return EXIT_SUCCESS;
}
