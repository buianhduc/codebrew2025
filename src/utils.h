#ifndef UTILS_H
#define UTILS_H

#include "triangles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
triangle_t* import_matrix_from_file(char* filePath){
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("The file is not found");
        return NULL;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read=getline(&line, &len, file)) != -1) {
        
    }
}


#endif