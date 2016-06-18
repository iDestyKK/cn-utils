/*
 * StrVec Library
 *
 * Description:
 *     C lacks vectors from C++.
 *     Therefore, this tries to aid in making string vectors in C.
 *     Might merge this with HandyC later.
 *
 * Author:
 *     Clara Taylor
 */

#include <stdlib.h>
#include <string.h>

#include "strvec.h"

//Initializer
STRVEC new_strvec() {
    STRVEC val;

    val = (STRVEC) malloc(sizeof(struct strvec));
    val->size = 0;

    //Adjust the strings properly
    val->str = (char**) malloc(sizeof(char*) * (val->size + 1));
    val->str[0] = "\0"; //Null terminate

    //Adjust lengths properly
    val->length = (int*) malloc(sizeof(int) * (val->size + 1));
    val->length[0] = 0;

    return val;
}

//Modifiers
void strvec_push_back(STRVEC obj, char* str) {
    obj->size++;
    char** new_Sptr = realloc(obj->str   , sizeof(char*) * (obj->size + 1));
    int *  new_Iptr = realloc(obj->length, sizeof(int  ) * (obj->size + 1));

    if (!new_Sptr || !new_Iptr)
        return; //You failed (Memory Error)
    else {
        obj->str    = new_Sptr;
        obj->length = new_Iptr;
    }

    //free(new_Sptr[obj->size - 1]);
    new_Sptr[obj->size - 1] = strdup(str);
    new_Iptr[obj->size - 1] = strlen(new_Sptr[obj->size - 1]);

    //Null Terminate the end
    obj->str[obj->size] = "\0";
}

void strvec_resize(STRVEC obj, sv_uint size) {
    if (size == obj->size)
        return; //You're wasting my time.

    char**  new_Sptr;
    int *   new_Iptr;
    sv_uint i;

    if (size < obj->size) {
        i = size;
        for (; i < obj->size; i++)
            if (strlen(obj->str[i]) > 0)
                free(obj->str[i]);

        new_Sptr = realloc(obj->str   , sizeof(char*) * (size + 1));
        new_Iptr = realloc(obj->length, sizeof(int  ) * (size + 1));

        if (!new_Sptr || !new_Iptr)
            return; //You failed (Memory Error)
        else {
            obj->str    = new_Sptr;
            obj->length = new_Iptr;
        }

        //Null Terminate the end
        obj->str[size] = "\0";
    }

    if (size > obj->size) {
        new_Sptr = realloc(obj->str   , sizeof(char*) * (size + 1));
        new_Iptr = realloc(obj->length, sizeof(int  ) * (size + 1));

        if (!new_Sptr || !new_Iptr)
            return; //You failed (Memory Error)
        else {
            obj->str    = new_Sptr;
            obj->length = new_Iptr;
        }


        i = obj->size;
        //free(obj->str[i - 1]); //Free NULL-terminated string.
        for (; i < size; i++) {
            //Insert a blank string.
            obj->str   [i] = "\0";
            obj->length[i] = 1;
        }

        //Null Terminate the end
        obj->str[size] = "\0";
    }

    //Replace the size.
    obj->size = size;
}

void strvec_set(STRVEC obj, sv_uint ind, char* str) {
    if (strcmp(obj->str[ind], "\0") != 0)
        free(obj->str[ind]);
    obj->str   [ind] = strdup(str);
    obj->length[ind] = strlen(obj->str[ind]);
}

void strvec_from_array(STRVEC obj, char** str_array) {
    //Truncates Strvec, then copies data over from char**.
    //This REQUIRES the char** to be NULL terminated.

    //Free original memory
    strvec_clear(obj);

    //Get Line Count
    sv_uint line_count = 0;
    while (str_array[++line_count] != NULL) {} //lol oh my god

    //Copy pointers
    obj->str    = (char   **) malloc(sizeof(char*  ) * (line_count + 1));
    obj->length = (sv_uint* ) malloc(sizeof(sv_uint) * line_count);

    sv_uint i = 0;
    for (; i < line_count; i++) {
        obj->str   [i] = strdup(str_array[i]);
        obj->length[i] = strlen(str_array[i]);
    }
    obj->size = line_count;
}

void strvec_clear(STRVEC obj) {
    sv_uint i = 0;
    for (; i < obj->size; i++)
        if (strcmp(obj->str[i], "\0") != 0)
            free(obj->str[i]);
    free(obj->str);
    obj->size = 0;
    free(obj->length);
}

//Get Values
sv_uint strvec_size(STRVEC obj) {
    return obj->size;
}

sv_uint strvec_length(STRVEC obj, sv_uint ind) {
    return obj->length[ind];
}

char* strvec_get(STRVEC obj, sv_uint ind) {
    return obj->str[ind];
}

char** strvec_get_array(STRVEC obj) {
    return obj->str;
}

int strvec_empty(STRVEC obj) {
    return (obj->size == 0);
}

//Deconstructor
void strvec_free(STRVEC obj) {
    strvec_clear(obj);
    free(obj);
}
