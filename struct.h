#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct disk {
    int disknum;
    int working;
    int* array;
} disk;

#endif