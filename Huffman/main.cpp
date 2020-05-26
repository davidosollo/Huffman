//
//  main.cpp
//  Huffman
//
//  Created by David Osollo on 4/6/19.
//  Copyright © 2019 David Osollo. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<list>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 100
#define NUM_LETRAS  100
#define MAX_FILE_SIZE 10000

// A Huffman tree node
struct MinHeapNode {
    
    // One of the input characters
    char data;
    int  iState;
    
    // Frequency of the character
    unsigned freq;
    
    // Left and right child of this node
    struct MinHeapNode *left, *right;
};

// A Min Heap:  Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {
    
    // Current size of min heap
    unsigned size;
    
    // capacity of min heap
    unsigned capacity;
    
    // Array of minheap node pointers
    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned freq, int iState);
void replace(char *sStr, char cOld, char cNew);
struct MinHeap* createMinHeap(unsigned capacity);
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
char *readFile(char *fileName);
void minHeapify(struct MinHeap* minHeap, int idx);
int isSizeOne(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void printArr(int arr[], int n);
int isLeaf(struct MinHeapNode* root);
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);
void printSTCodes(struct MinHeapNode* root, int arr[], int top);
void printCodes(struct MinHeapNode* root, int arr[], int top);
int getCodes(struct MinHeapNode* root, char cNodeId, int arr[], int top);
void HuffmanCodes(char data[], int freq[], int size);
int read_freq_file(const char *sFileName,char arr[],int freq[]);
void readFile(const char *fileName,char *code);
void WriteHuffFile(const char *fileName, char *code);
void DecompressHuffFile(const char *fileName);

struct MinHeapNode* gRoot;

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq, int iState)
{
    struct MinHeapNode* temp
    = (struct MinHeapNode*)malloc
    (sizeof(struct MinHeapNode));
    
    temp->left   = temp->right = NULL;
    temp->data   = data;
    temp->iState = iState;
    temp->freq   = freq;
    
    return temp;
}

////////////////////////////////////////////////////////
// Function to replace chars
void replace(char *sStr, char cOld, char cNew)
{
    char *str = sStr;
    str=strchr(str,cOld);
    while (str!=NULL)
    {
        *str=cNew;
        str=strchr(str+1,cOld);
    }
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    
    // current size is 0
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->
                                   capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
    
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < minHeap->size && minHeap->array[left]->
        freq < minHeap->array[smallest]->freq)
        smallest = left;
    
    if (right < minHeap->size && minHeap->array[right]->
        freq < minHeap->array[smallest]->freq)
        smallest = right;
    
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
    = minHeap->array[minHeap->size - 1];
    
    --minHeap->size;
    minHeapify(minHeap, 0);
    
    return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    
    printf("\n");
}

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)
{
    
    struct MinHeap* minHeap = createMinHeap(size);
    
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i],i);
    
    minHeap->size = size;
    buildMinHeap(minHeap);
    
    return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;
    int    iState = 0;
    
    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    
    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
        
        // Step 2: Extract the two minimum
        // freq items from min heap
        left  = extractMin(minHeap);
        right = extractMin(minHeap);
        
        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        
        top = newNode('$', left->freq + right->freq,iState);
        top->left = left;
        top->right = right;
        iState++;
        
        insertMinHeap(minHeap, top);
    }
    
    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printSTCodes(struct MinHeapNode* root, int arr[], int top)
{
    // Assign 0 to left edge and recur
    if(root)
    {
        if(root->data != '$') //printf("Q%d", root->iState);
        {
            //if(root->data=='\n') printf("CR");
            //else printf("%c", root->data);
        }
        else printf("Q%d", root->iState);
    }
    
    if(root->data == '$') printf("\t");
    if(root->left)
    {
        if(root->left->data != '$')
        {
            if(root->left->data=='\n') printf("CR");
            else printf("%c", root->left->data);
        }
        else printf("Q%d", root->left->iState);
    }
    
    if(root->data == '$') printf("\t");
    if(root->right)
    {
        if(root->right->data != '$')
        {
            if(root->right->data=='\n') printf("CR");
            else printf("%c", root->right->data);
        }
        else printf("Q%d", root->right->iState);
    }
    
    
    if(root->data == '$') printf("\n");
        
    if (root->left)
    {
        arr[top] = 0;
        printSTCodes(root->left, arr, top + 1);
    }
    
    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        printSTCodes(root->right, arr, top + 1);
    }

}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    
    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root))
    {
        if(root->data=='\n') printf("[CR]: ");
        else printf("[%c]: ", root->data);
        printArr(arr, top);
    }
}


// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
int getCodes(struct MinHeapNode* root, char cNodeId, int arr[], int top)
{
    int iCallResu =0;
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        iCallResu = getCodes(root->left, cNodeId,arr, top + 1);
    }
    
    // Assign 1 to right edge and recur
    if (root->right && iCallResu == 0)
    {
        arr[top] = 1;
        iCallResu = getCodes(root->right, cNodeId,arr, top + 1);
    }
    
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root) && root->data==cNodeId && iCallResu == 0)
    {
       //printf("[%c]: ", root->data);
       // printArr(arr, top);
       return top;
    }
    return iCallResu;
}


// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size)
{
    // Construct Huffman Tree
    gRoot = buildHuffmanTree(data, freq, size);
    struct MinHeapNode* lRoot;
    
    lRoot = gRoot;
    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT];
    int top = 0;
    
    printf("\nTabla de Codigos de Huffman\n" );
    printf("---------------------------\n" );
    printCodes(lRoot, arr, top);
    
    printf("\nAutomata Finito Deterministico\n" );
    printf("------------------------------\n" );
    printf("S\t0\t1\n" );
    printf("------------------------------\n" );
    printSTCodes(lRoot, arr, top);
}

////////////////////////////////////////////////////////
// Read the input files
int read_freq_file(const char *sFileName,char arr[],int freq[])
{
    std::ifstream infile(sFileName);
    std::string line;
    
    printf("\nTabla de Frecuencia\n" );
    printf("-------------------\n" );
    int i=0;
    
    while (std::getline(infile, line))
    {
        
        char sInput[line.length()+1];
        char *sPtr;
        strcpy(sInput,line.c_str());
        if(strlen(sInput))
        {
           //replace(sInput,',',' ');
           sPtr = strtok( sInput, ",\n\t" );

           if(sPtr[0]=='@') sPtr[0]='\n';
           arr[i]=sPtr[0];
           sPtr = strtok( NULL, " \n\t" );
           freq[i]=atoi(sPtr);
           printf("\n [%c]=%d",arr[i],freq[i]);
           i++;
            
        }
    }
    return i;
}

////////////////////////////////////////////////////////
// Read the input files
void readFile(const char *fileName,char *code)
{
    FILE *file;
    char cChar;

    file = fopen(fileName, "r");
    printf("\n\nArchivo de entrada\n");
    printf("------------------\n\n[" );
    *code = (char)fgetc(file);
    while(!feof(file))
    {
        printf("%c",*code);
        code++;
        cChar = (char)fgetc(file);
        if(!feof(file))
          *code = cChar;
    }
    printf("]\n\n");
    fclose(file);

}

////////////////////////////////////////////////////////
// WriteHuffFile Compress file
void WriteHuffFile(const char *fileName, char *code)
{
    
    struct MinHeapNode* lRoot;
    int arrHuffVales[MAX_TREE_HT];
    char sHuffFile[1000];
    int top=0;
    int iBitCount=0;
    char cByteWrite=0;
    int i=0;
    int iFirstBit=0;
    
    strcpy(sHuffFile,fileName);
    strcat(sHuffFile,".huff");
    FILE *fp = fopen(sHuffFile,"w");
    
    // Return if could not open file
    if (fp == NULL) return;
  
    while(*code)
    {
        lRoot = gRoot;
        top = getCodes(lRoot, *code, arrHuffVales, 0);
        
        for(i=0;i<top;i++)
        {
            //printf("%d",arrHuffVales[i]);
            
            if(iFirstBit!=0)
            {
                cByteWrite <<= 1;
                iBitCount++;
            }
            else
            {
                iFirstBit = 1;
            }
            
            if(arrHuffVales[i]==1)
            {
                cByteWrite = cByteWrite + 1;
            }
            
            /*printf("\n &&& $$$$ i=%d iBC=%d",i,iBitCount);*/
            if(iBitCount==7)
            {
                fputc(cByteWrite, fp);
                /*printf("\n___0x%x",cByteWrite);*/
                iBitCount = 0;
                cByteWrite = 0;
                iFirstBit = 0;
//                i++;
            }
        }
        code ++;
    }
    
    fclose(fp);
    
}

////////////////////////////////////////////////////////
// DecompressHuffFile
void DecompressHuffFile(const char *fileName)
{
    char sHuffFile[1000];
    char cByteRead;
    char cMask;
    char cAux;
    int  iAux;
    struct MinHeapNode* HuffNode = NULL;
    
    strcpy(sHuffFile,fileName);
    strcat(sHuffFile,".huff");
    FILE *fp = fopen(sHuffFile,"r");
    
    printf("\n\nArchivo Descomprimido\n");
    printf("------------------\n\n[" );
    
    cByteRead = (char)fgetc(fp);
    HuffNode = gRoot;
    
    while(!feof(fp))
    {
        
        if(!feof(fp))
        {
            cMask = 0x80;
            iAux = 0x80;
            for(int i=0;i<8;i++)
            {
                cAux = 0;
                cAux = cMask & cByteRead;
                if(cAux!=cMask)  //0
                {
                    //printf("0");
                    HuffNode = HuffNode->left;
                }
                else             //1
                {
                    /*printf("1");*/
                    HuffNode = HuffNode->right;
                }
                if(HuffNode->data != '$' )
                {
                    printf("%c",HuffNode->data);
                    HuffNode = gRoot;
                    /*printf("\n");*/
                }
                
               // cMask = cMask >> 1;
                iAux = iAux/2;
                cMask = iAux;
            }
            cByteRead = (char)fgetc(fp);
        }
       
    }
    
    printf("]\n\n");
    fclose(fp);
}

// Huffan Compression
int main(int argc, const char * argv[])
{
    
    char arr[NUM_LETRAS];
    int freq[NUM_LETRAS];
    char *sContFile;
    
    sContFile = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
    memset(sContFile,NULL,MAX_FILE_SIZE * sizeof(char));
    
    int size = read_freq_file(argv[2], arr,freq);
    printf("\n\nNúmero de letras = %d\n\n",size);
    //int size = sizeof(arr) / sizeof(arr[0]);
    
    HuffmanCodes(arr, freq, size);
    readFile(argv[1],sContFile);
    WriteHuffFile(argv[1], sContFile);
    DecompressHuffFile(argv[1]);
    
    return 0;
}
