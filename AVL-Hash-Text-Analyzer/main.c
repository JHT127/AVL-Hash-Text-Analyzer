/*Joud Thaher
1221381
Section 2
*/

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  //for to lower and is alpha
#define TABLE_SIZE 127 //prime number

int load=0; //make sure the user loads the file
int hashTableCreated = 0; //make sure the user create the hash table

typedef struct AVLNode* AVLTree; //pointer for the tree

struct AVLNode {  //the node of the tree content
    char* element;  //word
    AVLTree left;
    AVLTree right;
    int height;  //for balance
    int frequency; //counter for the repeated words
};

typedef struct ListNode* ListPtr;  //pointer for the linked list

struct ListNode { //used chaining for collision handling
    char* key;
    int frequency;
    ListPtr next;
};

typedef ListPtr HashTable[TABLE_SIZE];  //hash table

//function prototypes

void displayMenu();
AVLTree createAVLNode(const char*);
int getHeight(AVLTree);
int maxHeight(int, int);
AVLTree RSR(AVLTree);
AVLTree LSR(AVLTree);
AVLTree LRDR(AVLTree);
AVLTree RLDR(AVLTree);
int getBalance(AVLTree);
AVLTree insertIntoAVL(AVLTree,char*);
AVLTree findMinAVL(AVLTree);
AVLTree deleteAVLNode(const char*,AVLTree);
void inorderTraversal(AVLTree);
void printPreOrder(AVLTree);
void printPostOrder(AVLTree);
int isAlphabetical(char);
AVLTree loadDataFromFile(FILE*,AVLTree);
void checkAVLCreation();
void insertNewWordAVL(AVLTree);
AVLTree findAVL(const char*, AVLTree);
AVLTree deleteWordAVL(AVLTree);
int hashFunction(char*);
ListPtr createListNode(char*);
void insertIntoHashTable(HashTable,char*,int);
ListPtr searchInHashTable(HashTable,char*);
void printHashTable(HashTable);
void traverseInsertIntoHashTable(AVLTree,HashTable);
void toLowerCase(char*);
void insertNewWordIntoHashTable(HashTable);
void createHashTableOnce(AVLTree,HashTable);
void deleteWordHashTable(HashTable);
void searchWordHashTable(HashTable);
int findWordFrequencyInAVL(AVLTree,char*);
void printWordStatistics(HashTable,AVLTree);

//main function

int main() {

    int choice;
    AVLTree T=NULL; //initializing the tree
    HashTable table={NULL}; //initializing the hash table
    FILE* in; //file

    printf("\nWelcome!! :))\n");
    printf("Please note that this is NOT case-sensitive and it ignores NON-alphabetical characters! Enjoy ! :)");

    do {
        //do while so that it prints the menu until he chooses exit
        displayMenu();
        printf("Please enter your choice:");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\nLoading data from the file...\n\n");
                in= fopen("text.txt","r");
                T=loadDataFromFile(in,T);
                break;
            case 2:
                printf("\nCreating the AVL tree...\n\n");
                checkAVLCreation();
                break;
            case 3:
                printf("\nInserting a word into the AVL tree...\n\n");
                insertNewWordAVL(T);
                break;
            case 4:
                printf("\nDeleting a word from the AVL tree...\n\n");
                T= deleteWordAVL(T);
                break;
            case 5:
                printf("\nPrinting words sorted in the AVL tree...\n\n");
                if(load==0){
                    printf("\nYou have not loaded the file:( Enter 1 to load :)\n\n");
                }
                else{
                    printf("\nIn Order Traversal: \n\n");
                    inorderTraversal(T);
                    printf("\n");
                    printf("\nPost Order Traversal: \n\n");
                    printPostOrder(T);
                    printf("\n");
                    printf("\nPre Order Traversal: \n\n");
                    printPreOrder(T);
                    printf("\n");
                }
                break;
            case 6:
                printf("\nCreating the Hash Table...\n\n");
                createHashTableOnce(T,table);
                break;
            case 7:
                printf("\nInserting a word into the Hash table...\n\n");
                insertNewWordIntoHashTable(table);
                break;
            case 8:
                printf("\nDeleting a word from the Hash table...\n\n");
                deleteWordHashTable(table);
                break;
            case 9:
                printf("\nSearching for a word in the Hash table and printing its frequency...\n\n");
                searchWordHashTable(table);
                break;
            case 10:
                printf("\nPrinting words statistics...\n\n");
                printWordStatistics(table,T);
                break;
            case 11:
                if(load==0){
                     printf("\nYou have not loaded the file:( Enter 1 to load :)\n\n");
                }
                else{
                    if(hashTableCreated==0){
                       printf("\nYou have not created the Hash Table yet. Please choose option 6 to create it :)\n\n");
                    }
                    else{
                        printHashTable(table);
                    }
                }
                break;
            case 12:
                printf("\nExiting the application...\n\n");
                break;
            default:
                printf("\nInvalid choice! :( Please reread the menu and enter a valid option.\n\n");
                break;
        }
    } while (choice != 12);

    return 0;
}

//functions implementations

void displayMenu() { //menu

    printf("\n\nMenu:\n");
    printf("1. Load data from the file.\n");
    printf("2. Create the AVL tree.\n");
    printf("3. Insert a word into the AVL tree.\n");
    printf("4. Delete a word from the AVL tree.\n");
    printf("5. Print the words sorted in the AVL tree.\n");
    printf("6. Create the Hash Table.\n");
    printf("7. Insert a word into the Hash table.\n");
    printf("8. Delete a word from the hash table.\n");
    printf("9. Search for a word in the hash table and print its frequency.\n");
    printf("10. Print words statistics.\n");
    printf("11. Print Hash Table.\n");
    printf("12. Exit the application.\n\n");

}

AVLTree createAVLNode(const char* word) { //create node for AVL tree

    AVLTree newNode = (AVLTree )malloc(sizeof(struct AVLNode));

    if (newNode == NULL) {
        printf("Out of memory\n");
        return NULL;
    }

    newNode->element = (char*)malloc((strlen(word) + 1) * sizeof(char));

    if (newNode->element == NULL) {
        free(newNode);
        printf("Out of memory\n");
        return NULL;
    }

    strcpy(newNode->element, word);
    toLowerCase(newNode->element);
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    newNode->frequency = 1;
    return newNode;
}

int getHeight(AVLTree T) {

    if (T == NULL)
        return 0;
    return T->height;
}

int maxHeight(int LHS, int RHS) {

    return LHS > RHS ? LHS : RHS;
}

AVLTree RSR(AVLTree T) { //right single rotation

    AVLTree newRoot = T->left;
    T->left = newRoot->right;
    newRoot->right = T;

    //fix height
    T->height = maxHeight(getHeight(T->left), getHeight(T->right)) + 1;
    newRoot->height = maxHeight(getHeight(newRoot->left), T->height) + 1;

    return newRoot;
}

AVLTree LSR(AVLTree T) { //left single rotation

    AVLTree newRoot = T->right;
    T->right = newRoot->left;
    newRoot->left = T;

    T->height = maxHeight(getHeight(T->left), getHeight(T->right)) + 1;
    newRoot->height = maxHeight(getHeight(T->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}

AVLTree LRDR(AVLTree T) { //left right double rotation

    T->left = LSR(T->left);
    RSR(T);
    return T;
}

AVLTree RLDR(AVLTree T) { //right left double rotation

    T->right = RSR(T->right);
    LSR(T);
    return T;
}

int getBalance(AVLTree T) {

    if (T == NULL)
        return 0;
    return getHeight(T->left) - getHeight(T->right);
}

AVLTree insertIntoAVL(AVLTree T,char* word) {

    char wordInLower[100];
    strcpy(wordInLower, word);
    toLowerCase(wordInLower);

    if (T == NULL){
        T= createAVLNode(word);
        return T;
    }

    int compare = strcmp(wordInLower, T->element);

    if (compare > 0) {

        T->right = insertIntoAVL(T->right, wordInLower);

        if (getBalance(T) == 2) {
            if (strcmp(wordInLower, T->right->element) > 0) {
                T = LSR(T);
            }
            else {
                T = RLDR(T);
            }
        }
    }

    else if (compare < 0) {
        T->left = insertIntoAVL(T->left, wordInLower);
        if (getBalance(T) == -2) {
            if (strcmp(wordInLower, T->left->element) < 0) {
                T = RSR(T);
            }
            else {
                T = LRDR(T);
            }
        }
    }

    else {
        //printf("Element exists\n");
        T->frequency++;
    }

    T->height = maxHeight(getHeight(T->left), getHeight(T->right)) + 1;
    return T;
}

AVLTree findMinAVL(AVLTree T) {

    if (T == NULL) {
        return NULL;
    }
    else if (T->left == NULL) {
        return T;
    }
    else {
        return findMinAVL(T->left);
    }
}

AVLTree deleteAVLNode(const char* word, AVLTree T) {

    AVLTree temp;
    if (T == NULL) {
        printf("%s is not found\n", word);
        return T;
    }

    char wordInLower[100];
    strcpy(wordInLower, word);
    toLowerCase(wordInLower);

    char elementInLower[100];
    strcpy(elementInLower, T->element);
    toLowerCase(elementInLower);

    int compare = strcmp(wordInLower, elementInLower);

    if (compare < 0) {
        T->left = deleteAVLNode(word, T->left);
    }
    else if (compare > 0) {
        T->right = deleteAVLNode(word, T->right);
    }
    else {
        if (T->left && T->right) {
            temp = findMinAVL(T->right);
            free(T->element);
            T->element = malloc(strlen(temp->element) + 1);
            strcpy(T->element, temp->element);
            T->right = deleteAVLNode(temp->element, T->right);
        }
        else {
            temp = T;
            if (T->left == NULL) {
                T = T->right;
            }
            else if (T->right == NULL) {
                T = T->left;
            }
            free(temp->element);
            free(temp);
        }
    }

    if (T == NULL){
        return T;
    }

    T->height = 1 + maxHeight(getHeight(T->left), getHeight(T->right));

    //to balance the tree after deleting
    if (getBalance(T) > 1 && getBalance(T->left) >= 0)
        return RSR(T);
    if (getBalance(T) > 1 && getBalance(T->left) < 0) {
        T->left = LSR(T->left);
        return RSR(T);
    }
    if (getBalance(T) < -1 && getBalance(T->right) <= 0)
        return LSR(T);
    if (getBalance(T) < -1 && getBalance(T->right) > 0) {
        T->right = RSR(T->right);
        return LSR(T);
    }

    return T;
}

int isAlphabetical(char c) { //works as is alpha
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void inorderTraversal(AVLTree T) {

    if (T != NULL) {
        inorderTraversal(T->left);
        printf("Element is:%s, frequency is:%d\n", T->element,T->frequency);
        inorderTraversal(T->right);
    }
}

void printPreOrder(AVLTree T) {

    if (T != NULL) {
        printf("%s, frequency is:%d\n", T->element,T->frequency);
        printPreOrder(T->left);
        printPreOrder(T->right);
    }
}

void printPostOrder(AVLTree T) {

    if (T != NULL) {
        printPostOrder(T->left);
        printPostOrder(T->right);
        printf("%s, frequency is:%d\n", T->element,T->frequency);
    }
}

AVLTree loadDataFromFile(FILE* in, AVLTree T){

    in=fopen("input.txt","r"); //opens file

    if(in==NULL){ //if file does not exist
        printf("File does not exist :( \n");
        return T;
    }

    char word[100]; //before is lower and is alpha
    char result[100]; //after is lower and is alpha

    while(fscanf(in,"%s", word) != EOF){ //reads until the ends of the file

        int j=0;
        for(int i=0; word[i] != '\0'; i++){

            if(isalpha(word[i])){ //checks whether it is alphabetical
                result[j++]=tolower(word[i]); //to lower the word then add it to result
            }
        }

        result[j]='\0'; //null terminator
        T=insertIntoAVL(T,result); //insert to the avl
    }

    fclose(in); //closes the file
    printf("Data loaded successfully from the file! :)\n");
    load=1; //indicated file is loaded

    return T;

}

void checkAVLCreation(){ //hecks if the file is loaded before telling the user tree is created in option 2

    if(load==0){
        printf("\nYou have not loaded the file:( Enter 1 to load:)\n\n");
    }
    else {
        printf("AVL tree created! :)\nChoose option 5 if you want to print it ! :) ");
    }
}

void insertNewWordAVL(AVLTree T) { //insert new word into AVL for option 3

    if (load == 0) { //if not loaded ask the user to load
        printf("\nYou have not loaded the file :( Please enter 1 to load :) \n\n");
        return;
    }

    char yesNo[3]; //user choice to continue adding

    do {
        char newWord[100]; //the word the user enters
        printf("Please enter a new word to insert: ");
        scanf("%s", newWord);
        getchar();

        char onlyCharWord[100]; //the word containing only alphabetical characters
        int j = 0;
        for (int i = 0; newWord[i] != '\0'; i++) {
            if (isAlphabetical(newWord[i])) {
                onlyCharWord[j++] = tolower(newWord[i]);
            }
        }
        onlyCharWord[j] = '\0';

        if (j == 0) {
            printf("The word contains no alphabetical characters and cannot be inserted.\n"); //the string with alpha is empty
        }

        else {
            //asking the user how many times he wants to add the word
            int frequency;
            printf("How many times do you want to insert '%s'? ", onlyCharWord);
            scanf("%d", &frequency);
            getchar();

            for (int i = 0; i < frequency; i++) {
                insertIntoAVL(T, onlyCharWord);
            }
            printf("%s inserted successfully %d times :)\n", onlyCharWord, frequency);
        }
        //continue inserting until the user stops
        printf("Do you want to insert another word? (make sure to ONLY enter yes or y, other entries will stop the insertion): ");
        scanf("%s", yesNo);
        getchar();

    } while (strcasecmp(yesNo, "yes") == 0 || strcasecmp(yesNo, "y") == 0 );
}

AVLTree findAVL(const char* x, AVLTree T) { //find the word in AVL tree

    if (T == NULL) {
        return NULL;
    }

    char elementInLower[100], XinLower[100];
    strcpy(elementInLower, T->element);
    strcpy(XinLower, x);

    toLowerCase(elementInLower);
    toLowerCase(XinLower);

    int compare = strcmp(XinLower, elementInLower);
    if (compare < 0) {
        return findAVL(XinLower, T->left);
    }
    else if (compare > 0) {
        return findAVL(XinLower, T->right);
    }
    else {
        return T;
    }
}

AVLTree updateFrequency(AVLTree T, int count) { //to update the frequency helpful for the delete function
    if (T->frequency <= 0) {
        return NULL;
    }
    T->frequency -= count;
    return T;
}

AVLTree deleteWordAVL(AVLTree T) { //delete word from user(everything is similar to insert new word)

    if (load == 0) {
        printf("\nYou have not loaded the file :( Please enter 1 to load :)\n\n");
        return T;
    }

    else {
        char yesNo[3];
        do {
            char input[100];
            printf("Please enter a word to delete: ");
            scanf("%s", input);
            getchar();
            toLowerCase(input);

            char wordToDelete[100];
            int wordIndex = 0;


            for (int i = 0; input[i] != '\0'; i++) {
                if (isAlphabetical(input[i])) {
                    wordToDelete[wordIndex++] = input[i];
                }
            }
            wordToDelete[wordIndex] = '\0';

            if (strlen(wordToDelete) == 0) {
                printf("Invalid input. Please enter a word containing alphabetical characters.\n");
                continue;
            }

            AVLTree node = findAVL(wordToDelete, T);
            if (node == NULL) {
                printf("Word '%s' not found in the tree.\n", wordToDelete);
            }
            else {
                if (node->frequency > 1) {
                    int choice;
                    printf("Word '%s' has a frequency of %d.\n", wordToDelete, node->frequency);
                    while (1) {
                        printf("Do you want to delete the whole node or decrease the frequency? (enter 1 to delete and 2 to decrease): ");
                        scanf("%d", &choice);
                        getchar();
                        if (choice == 1) {
                            T = deleteAVLNode(wordToDelete, T);
                            printf("Node with word '%s' deleted.\n", wordToDelete);
                            break;
                        }
                        else if (choice == 2) {
                            int count;
                            printf("How many times do you want to decrease the frequency by? ");
                            scanf("%d", &count);
                            getchar();
                            if (count >= node->frequency) {
                                T = deleteAVLNode(wordToDelete, T);
                                printf("Node with word '%s' deleted.\n", wordToDelete);
                            }
                            else {
                                node = updateFrequency(node, count);
                                printf("Frequency of word '%s' decreased by %d. New frequency: %d.\n", wordToDelete, count, node->frequency);
                            }
                            break;
                        }
                        else {
                            printf("Invalid option. Please make sure you only enter 1 or 2 as described!\n");
                        }
                    }
                }
                else {
                    T = deleteAVLNode(wordToDelete, T);
                    printf("Node with word '%s' deleted.\n", wordToDelete);
                }
            }
            printf("Do you want to delete another word? (make sure to ONLY enter yes or y, other entries will stop the deletion): ");
            scanf("%s", yesNo);
            getchar();
        } while (strcasecmp(yesNo, "yes") == 0 || strcasecmp(yesNo, "y") == 0);

        return T;
    }
}

int hashFunction(char* key) { //hash function to calculate the index

    int hashValue = 0;
    while (*key != '\0') {
        hashValue = ((hashValue << 5) + *key) % TABLE_SIZE;
        key++;
    }
    return hashValue;
}

ListPtr createListNode(char* key) { //linked list node creation

    ListPtr newNode = (ListPtr)malloc(sizeof(struct ListNode));

    if (newNode == NULL) {
        printf("Out of memory:(\n");
        return NULL;
    }

    newNode->key = (char*)malloc((strlen(key)+ 1) * sizeof(char));

    if (newNode->key == NULL) {
        printf("Out of memory\n");
        free(newNode);
        return NULL;
    }

    strcpy(newNode->key, key);
    newNode->frequency = 1;
    newNode->next = NULL;
    return newNode;
}

void insertIntoHashTable(HashTable hashTable,char* key,int frequency) {

    int index = hashFunction(key);
    ListPtr current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->frequency += frequency;
            return;
        }
        current = current->next;
    }

    ListPtr newNode = createListNode(key);

    if(newNode==NULL){
        printf("Out of memory!\n");
        return;
    }

    newNode->frequency=frequency;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

ListPtr searchInHashTable(HashTable hashTable,char* key) {

    int index = hashFunction(key);
    ListPtr current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printHashTable(HashTable hashTable) {

    if(load==0){
        printf("\nYou have not loaded the file:( Please enter 1 to load:) \n\n");
    }
    else{

        if(hashTableCreated==0){ //if the user have not loaded the data to hash table
        printf("\nYou have not created the hash table :( Enter 6 to do so!! \n\n");
        }

        printf("\nHash Table :) !!\n\n");
        for (int i = 0; i < TABLE_SIZE; i++) {
            ListPtr current = hashTable[i];
            while (current != NULL) {
                printf("key is:%s and its frequency is:%d\n", current->key, current->frequency);
                current = current->next;
            }
        }
    }
}

void traverseInsertIntoHashTable(AVLTree T, HashTable hashTable) { //load from AVL to hash table

    if(load==0){
        printf("\nYou have not loaded the file:( Please enter 1 to load:)\n\n");
    }

    else{
        if (T != NULL) {
            insertIntoHashTable(hashTable, T->element,T->frequency);
            traverseInsertIntoHashTable(T->left, hashTable);
            traverseInsertIntoHashTable(T->right, hashTable);
        }
    }
}

void createHashTableOnce(AVLTree T, HashTable hashTable) { //create the hash table

    if(load == 0){
        printf("\nYou have not loaded the file:( Please enter 1 to load:) \n\n");
        return;
    }

    if (hashTableCreated == 1) {
        printf("Hash Table has already been created.\n");
        return;
    }

    traverseInsertIntoHashTable(T, hashTable);

    hashTableCreated = 1;
    printf("Hash Table created successfully:)\nChoose option 11 if you want to print it! :) ");
    //printHashTable(hashTable);
}

void toLowerCase(char* str) { //function to lower letters

    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }

}

void insertNewWordIntoHashTable(HashTable hashTable) { //user enters word to insert

    if (load == 0) {
        printf("\nYou have not loaded the file :( Please enter 1 to load :)\n\n");
        return;
    }
    if (hashTableCreated == 0) {
        printf("\nYou have not created the Hash Table yet. Please choose option 6 to create it :)\n\n");
        return;
    }

    char yesNo[3];

    do {

        char newWord[100];
        printf("Please enter a new word to insert: ");
        scanf("%s", newWord);
        getchar();

        char validWord[100];
        int j = 0;
        for (int i = 0; newWord[i] != '\0'; i++) {
            if (isAlphabetical(newWord[i])) {
               validWord[j++] = tolower(newWord[i]);
            }
        }
        validWord[j] = '\0';

        if (j == 0) {
            printf("The word contains no alphabetical characters and cannot be inserted.\n");
        }
        else {
            int frequency;
            printf("How many times do you want to insert '%s'? ", validWord);
            scanf("%d", &frequency);
            getchar();

            for (int i = 0; i < frequency; i++) {
                ListPtr wordNode = searchInHashTable(hashTable, validWord);
                if (wordNode != NULL) {
                    wordNode->frequency++;
                }
                else {
                    insertIntoHashTable(hashTable, validWord, 1);
                }
            }
            printf("%s inserted successfully %d times :)\n", validWord, frequency);
        }

        printf("Do you want to insert another word? (make sure to ONLY enter yes or y, other entries will stop the insertion): ");
        scanf("%s", yesNo);
        getchar();
    } while (strcasecmp(yesNo, "yes") == 0 || strcasecmp(yesNo, "y") == 0 );;

    //printHashTable(hashTable);
}

void deleteWordHashTable(HashTable hashTable) { //user enter word to delete

    if (load == 0) {
        printf("\nYou have not loaded the file :( Please enter 1 to load :)\n\n");
        return;
    }
    if (hashTableCreated == 0) {
        printf("\nYou have not created the Hash Table yet. Please choose option 6 to create it :)\n\n");
        return;
    }

    char yesNo[3];
    do {
        char wordToDelete[100];
        int validWord = 0;

        while (!validWord) {
            char input[100];
            printf("Please enter a word to delete: ");
            scanf("%s", input);
            getchar();
            toLowerCase(input);

            int wordIndex = 0;
            for (int i = 0; input[i] != '\0'; i++) {
                if (isAlphabetical(input[i])) {
                    wordToDelete[wordIndex++] = input[i];
                }
            }
            wordToDelete[wordIndex] = '\0';

            if (strlen(wordToDelete) == 0) {
                printf("Invalid input. Please enter a word containing only alphabetical characters.\n");
            } else {
                validWord = 1;
            }
        }

        //to handle collisions
        int index = hashFunction(wordToDelete);
        ListPtr current = hashTable[index];
        ListPtr previous = NULL;

        while (current != NULL) {
            if (strcmp(current->key, wordToDelete) == 0) {

                if (current->frequency > 1) { //word repeated multiple times

                    int choice;
                    printf("Word '%s' found in the hash table with frequency %d.\n", wordToDelete, current->frequency);
                    while (1) {
                        printf("Do you want to delete the whole word or decrease the frequency? (enter 1 to delete and 2 to decrease): ");
                        scanf("%d", &choice);
                        getchar();
                        if (choice == 1) {
                            if (previous == NULL) {
                                hashTable[index] = current->next;
                            }
                            else {
                                previous->next = current->next;
                            }
                            printf("Word '%s' deleted from the hash table.\n", wordToDelete);
                            free(current->key);
                            free(current);
                            break;
                        }
                else if (choice == 2) {
                            int count;
                            printf("How many times do you want to decrease the frequency? ");
                            scanf("%d", &count);
                            getchar();
                            if (current->frequency > count) {
                                current->frequency -= count;
                                printf("Frequency of word '%s' decreased by %d. New frequency: %d.\n", wordToDelete, count, current->frequency);
                            }
                            else {
                                if (previous == NULL) {
                                    hashTable[index] = current->next;
                                }
                                else {
                                    previous->next = current->next;
                                }
                                printf("Word '%s' deleted from the hash table.\n", wordToDelete);
                                free(current->key);
                                free(current);
                            }
                            break;
                        }
                        else {
                            printf("Invalid input. Please make sure you only enter 1 or 2 as described!\n");
                        }
                    }
                }
                else {
                    if (previous == NULL) {
                        hashTable[index] = current->next;
                    }
                    else {
                        previous->next = current->next;
                    }
                    printf("Word '%s' deleted from the hash table.\n", wordToDelete);
                    free(current->key);
                    free(current);
                }
                break;
            }

            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            printf("Word '%s' not found in the hash table.\n", wordToDelete);
        }

        printf("Do you want to delete another word? (make sure to ONLY enter yes, other entries will stop the deletion): ");
        scanf("%s", yesNo);
        getchar();

    } while (strcasecmp(yesNo, "yes") == 0 || strcasecmp(yesNo, "y") == 0);
}

void searchWordHashTable(HashTable hashTable) { //search in hash table

    if (load == 0) {
        printf("\nYou have not loaded the file :( Please enter 1 to load :)\n\n");
        return;
    }
    else {
        if (hashTableCreated == 0) {
            printf("\nYou have not created the Hash Table yet. Please choose option 6 to create it :)\n\n");
            return;
        }

        char yesNo[3];
        do {
            char input[100];
            printf("Please enter the word to search for: ");
            scanf("%s", input);
            getchar();

            char wordToSearch[100];
            int wordIndex = 0;

            for (int i = 0; input[i] != '\0'; i++) {
                if (isAlphabetical(input[i])) {
                    wordToSearch[wordIndex++] = tolower(input[i]);
                }
            }
            wordToSearch[wordIndex] = '\0';

            if (strlen(wordToSearch) == 0) {
                printf("Invalid input. Please enter a word containing alphabetical characters.\n");
                continue;
            }

            ListPtr wordNode = searchInHashTable(hashTable, wordToSearch);

            if (wordNode != NULL) {
                printf("Word '%s' found in the hash table with frequency %d.\n", wordToSearch, wordNode->frequency);
            }
            else {
                printf("Word '%s' not found in the hash table.\n", wordToSearch);
            }

            printf("Do you want to search for another word? (make sure to ONLY enter yes or y, other entries will stop searching): ");
            scanf("%s", yesNo);
            getchar();

        } while (strcasecmp(yesNo, "yes") == 0 || strcasecmp(yesNo, "y") == 0);
    }
}

int findWordFrequencyInAVL(AVLTree T,char* word) { //to use it in word statistics

    if (T == NULL) return -1;
    int compare = strcmp(word, T->element);
    if (compare == 0) return T->frequency;
    if (compare < 0) return findWordFrequencyInAVL(T->left, word);
    return findWordFrequencyInAVL(T->right, word);

}

void printWordStatistics(HashTable hashTable, AVLTree T) { //word statistics

    if (load == 0) {
        printf("\nYou have not loaded the file :( Please enter 1 to load :)\n\n");
        return;
    }

    int threshold; //a value for the user to decide
    printf("Enter the threshold value for words repeated more than specific times: ");
    scanf("%d", &threshold);
    getchar();

    if (hashTableCreated == 0) {
        printf("Hash table has not been created yet. Please create the hash table first.\n");
    }
    else {
        int totalUniqueWordsHash = 0; //words with frequency 1 in the hash table
        int totalNonUniqueWordsHash = 0; //words repeated in the hash table
        int maxFrequencyHash = 0; //maximum frequency in the hash table
        char mostFrequentWordHash[100]; //word with most frequency

        printf("\nWord Count Statistics for Hash Table :) !!!\n\n");

        for (int i = 0; i < TABLE_SIZE; i++) {
            ListPtr current = hashTable[i];
            while (current != NULL) {
                if (current->frequency == 1) {
                    totalUniqueWordsHash++;
                    printf("Unique word: %s, Frequency: %d\n", current->key, current->frequency);
                }
                else {
                    totalNonUniqueWordsHash++;
                    printf("Non-unique word: %s, Frequency: %d\n", current->key, current->frequency);
                }

                if (current->frequency > maxFrequencyHash) {
                    maxFrequencyHash = current->frequency;
                    strcpy(mostFrequentWordHash, current->key);
                }

                current = current->next;
            }
        }

        printf("\nTotal number of unique words: %d\n\n", totalUniqueWordsHash);
        printf("Total number of non-unique words: %d\n\n", totalNonUniqueWordsHash);
        printf("Most frequent word: %s with Frequency: %d\n\n", mostFrequentWordHash, maxFrequencyHash);

        printf("\nWords repeated more than %d times:\n", threshold);
        int foundHash = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            ListPtr current = hashTable[i];
            while (current != NULL) {
                if (current->frequency > threshold) {
                    printf("Word is: %s and its frequency is: %d\n", current->key, current->frequency);
                    foundHash = 1;
                }
                current = current->next;
            }
        }
        if (foundHash == 0) {
            printf("No words with frequency greater than %d were found in Hash Table.\n", threshold);
        }
        //to find the words in hash table with frequency greater the frequency of the word entered (AVL)
        int validAVL=0;

        do{

        char input[100];
        printf("Extra:) Enter a word to find words with higher frequency in AVL tree(Remember that option 5 print the tree!) : \n");
        scanf("%s", input);
        getchar();

        char wordToSearch[100];
        int wordIndex = 0;

            for (int i = 0; input[i] != '\0'; i++) {
                if (isAlphabetical(input[i])) {
                    wordToSearch[wordIndex++] = tolower(input[i]);
                }
            }
            wordToSearch[wordIndex] = '\0';

            if (strlen(wordToSearch) == 0) {
                printf("Invalid input. Please enter a word containing alphabetical characters.\n");
                continue;
            }

        int inputFrequency = findWordFrequencyInAVL(T, wordToSearch);
        if (inputFrequency == -1) {
            printf("The word '%s' was not found in the AVL tree.\n", wordToSearch);
            validAVL=0;
        }
        else {
            printf("Words in AVL tree with frequency higher than '%s' (Frequency: %d):\n", wordToSearch, inputFrequency);
            int foundAVL = 0;
            for (int i = 0; i < TABLE_SIZE; i++) {
                ListPtr current = hashTable[i];
                while (current != NULL) {
                    int wordFrequency = findWordFrequencyInAVL(T, current->key);
                    if (wordFrequency > inputFrequency) {
                        printf("Word: %s, Frequency: %d\n", current->key, wordFrequency);
                        foundAVL = 1;
                    }
                    current = current->next;
                }
            }
            if (foundAVL == 0) {
                printf("No words with frequency higher than '%s' were found in the AVL tree.\n", input);
            }
             validAVL=1;
            }
        }while(validAVL==0);
    }
}