#include <stdio.h> //library untuk standard input output function
#include <stdlib.h> //library untuk malloc dan system cls
#include <string.h> //library untuk manipulation string
#include <stdbool.h> //library untuk boolean function
#include <ctype.h> //library untuk change character

#define SIZE 26 //alphabet size dari a-z

typedef struct TrieNode{
	//declare variable untuk word and description 
    struct TrieNode *children[SIZE]; 
    char *description;
    
    bool is_end_of_word; //variabel untuk cek apakah node sudah di akhir dari kata atau belum
    
}TrieNode;

struct TrieNode *getNewTrieNode(){
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode)); //memory allocation
    
    node->is_end_of_word = false; // node diasumsikan bahwa belum akhir dari kata
    node->description = NULL; //node yang dibuat belum ada description
    
    for (int i = 0; i < SIZE; i++){
        node->children[i] = NULL; //tidak ada children yang terhubung ke node
    }
    
    return node;
}

void pause(){
    printf("Press enter to continue..."); 
    getchar(); 
    while(getchar() != '\n');
}

int hasSpace(char *slang){
	while(*slang != '\0'){ //selama slang tidak NULL
		
		//bila ada spasi
		if(*slang == ' '){
			return 1;
		}
		
		//bila tidak ada spasi akan mengecheck karakter selanjutnya
		slang++;
	}
	return 0;
}

int inputWord(char *word){
    printf("Input a new slang word [Must be more than 1 character and contains no space]: ");
    scanf(" %[^\n]s", word);
    
    // akan mengembalikan 1(true) bila word lebih dari 1 karakter dan tidak ada spasi
    return (strlen(word) > 1 && !hasSpace(word));
}

int inputDescription(char *description){
    printf("Input a new slang word description [Must be more than 2 words]: ");
    scanf(" %[^\n]s", description);
    
    // akan mengembalikan 1(true) bila ada spasi
    return (strlen(description) > 1 && hasSpace(description));
}

void releaseSlang(struct TrieNode *root){
    char slang[100], description[255]; //declare varibel char untuk input
    int isValid; //declare varibel integer untuk validasi

    do{
        isValid = inputWord(slang);
    }while(!isValid); //akan terus looping bila tidak valid
    
    do{
        isValid = inputDescription(description);
    }while(!isValid); //akan terus looping bila tidak valid
    
    TrieNode *current_node = root;
    bool isNewWord = true;  //asumsi kata baru adalah true
    
    for(int i=0; slang[i]; i++){
        int index = tolower(slang[i]) - 'a'; //mencari index setiap karakter dalam satu kata
        
        if (!current_node->children[index]){
            current_node->children[index] = getNewTrieNode(); //membuat node baru jika belum ada index itu
        }
        
        current_node = current_node->children[index];
    }
    
    //cek apakah node sudah di akhir dari sebuah kata
    //artinya kata tersebut sudah ada dalam trie
    if(current_node->is_end_of_word){
        isNewWord = false;  //kata sudah ada, hanya update deskripsi
        
        free(current_node->description);  //membebaskan memori deskripsi lama
        current_node->description = strdup(description);  //nyalin deskripsi baru
        
        printf("\nSuccessfully updated a slang word.\n");
        
    }else{ //kata tersebut belum ada
    
        current_node->is_end_of_word = true; //menandai bahwa node adalah akhir dari kata
        current_node->description = strdup(description); //nyalin deskripsi baru
        
        printf("\nSuccessfully released new slang word.\n");
    }

    pause();
}

void searchSlang(struct TrieNode *root){
    char word[100]; //declare varibel char untuk input
    int isValid; //declare varibel integer untuk validasi
    
    do{
        isValid = inputWord(word);
    }while(!isValid);  //akan terus looping bila tidak valid

    struct TrieNode *current_node = root;
    
    for(int i = 0; word[i]; i++){
        int index = tolower(word[i]) - 'a';
        
        if(!current_node->children[index]){ //bila tidak ada huruf di kata yang dicari 
            printf("\nThere is no word \"%s\" in the dictionary.\n",word);
            return;
        }
        
        current_node = current_node->children[index];
    }

    if(current_node != NULL && current_node->is_end_of_word){ //bila ketemu
        printf("\n");
        printf("Slang word  : %s\n", word);
        printf("Description : %s\n", current_node->description);
        printf("\n");
        
        return;
    }

    printf("\nThere is no word \"%s\" in the dictionary.\n",word);  //bila ga ketemu
    return;
}

void previxList(struct TrieNode *node, char *word, int level, int *count){
    if(node->is_end_of_word){ //jika node ini berada di akhir dari sebuah kata, maka kata tersebut akan dicetak
        word[level] = '\0'; //menambahkan NULL untuk array akhir dari kata
        printf("%d. %s\n", (*count)++, word); 
    }

    for(int i=0; i<SIZE; i++){
    	
        if(node->children[i]){
            word[level] = i + 'a'; //mengubah index tadi jadi kata
            previxList(node->children[i], word, level + 1, count); //rekrusif untuk print
        }
    }
    
}

void searchPrefix(struct TrieNode *root){
    char prefix[100]; //variabel untuk input
    printf("Input a prefix to be searched: ");
    scanf(" %[^\n]s", prefix);

    struct TrieNode *current_node = root;
    
    int level;
    for(level=0; prefix[level] != '\0'; level++){
        int index = tolower(prefix[level]) - 'a'; //mencari index setiap karakter dalam satu kata
        
        if (!current_node->children[index]){ //bila tidak ada kata sesuai prefix
            printf("There is no prefix \"%s\" in the dictionary.\n", prefix);
            return;
        }
        
        current_node = current_node->children[index];
    }

    char word[100]; //declare variabel buat di-copy
    strncpy(word, prefix, level); //copy prefix kedalam word yang akan digunakan buat nyari di function lain
    
    int count=1; //variabel penomoran dan untuk validasi
    printf("\nWords start with \"%s\":\n", prefix); //header dari list
    
    previxList(current_node, word, level, &count);
    
}

bool isTrieEmpty(struct TrieNode *root){
    if(!root) return true; //jika Trie kosong
    
    if(root->is_end_of_word) return false; //ada kata di root

    for(int i=0; i<SIZE; i++){
    	
        if(root->children[i]){
            if (!isTrieEmpty(root->children[i])){ //rekursif untuk mengecek anak-anak
                return false;
            }
        }
    }
    return true;
}

void printTrie(struct TrieNode *node, char *word, int level, int *count){
    if(node->is_end_of_word){
        word[level] = '\0'; //menambahkan NULL untuk array akhir dari kata
        printf("%d. %s\n", (*count)++, word); 
    }

    for(int i=0; i<SIZE; i++){
    	
        if(node->children[i]){
            word[level] = i + 'a'; //mengubah index tadi jadi kata
            printTrie(node->children[i], word, level + 1, count); //rekursif untuk print
        }
    }
    
}

void printAll(TrieNode *root){
    if (isTrieEmpty(root)){ //kalau ternyata kosong
        printf("There is no slang word yet in the dictionary.\n");
        return;
    }
    
    printf("List of all slang words in the dictionary:\n"); //header
    char word[100];
    int count = 1; //variabel untuk penomoran
    printTrie(root, word, 0, &count);
    
}

int main(){
    TrieNode *root = getNewTrieNode(); //inisialisasi Trie root
    int menu; //declare variable untuk menu input

    do {
        system("cls"); 

        printf("1. Release a new slang word\n");
        printf("2. Search a slang word\n");
        printf("3. View all slang words starting with a certain prefix word \n");
        printf("4. View all slang words\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &menu); system("cls");

        switch(menu){
            case 1: //create slang
                releaseSlang(root);
                break;

            case 2: //search slang
		        searchSlang(root);
		        pause();
                break;

            case 3: //display by prefix
                searchPrefix(root); printf("\n");
                pause();
                break;

            case 4: //display all slangs
                printAll(root); printf("\n");
                pause();
                break;

            case 5: //exit
                printf("Thank you... Have a nice day :)\n");
                return 0;

            default:
                break;
        }
    } while (1);

    return 0;
}

