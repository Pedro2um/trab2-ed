#ifndef CODE_TABLE
#define CODE_TABLE

typedef struct code_table Code_Table;

Code_Table* init_code_table(void);

void insert_code_table(Code_Table* c_table, char* string , unsigned char index);

void free_code_table(Code_Table* c_table);

char * get_code_table(Code_Table* c_table, int index);

void show_code_table(Code_Table* c_table);


#endif 