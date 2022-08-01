#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct stream Stream;

Stream* init_stream(FILE* f,unsigned int MAX_SIZE_bytes);

void fill_stream( Stream* s );

int stream_feof(Stream* s);

int read_by_stream(Stream* s , unsigned char * c );

int empty_stream(Stream* s);

void free_stream(Stream* s );

#endif 