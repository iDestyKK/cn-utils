/*
 * StrVec Library
 *
 * Description:
 *     C lacks vectors from C++.
 *     Therefore, this tries to aid in making string vectors in C.
 *     Might merge this with HandyC later.
 *
 * Author:
 *     Clara Van Nguyen
 */

#ifndef __CT_STRVEC__
#define __CT_STRVEC__

//Typedef
typedef unsigned int sv_uint;

//Struct
typedef struct strvec {
    sv_uint  size;
    sv_uint* length;
    char**   str;
} *STRVEC;

//Initializer
STRVEC new_strvec();

//Modifiers
void    strvec_push_back (STRVEC, char*);
void    strvec_resize    (STRVEC, sv_uint);
void    strvec_set       (STRVEC, sv_uint, char*);
void    strvec_from_array(STRVEC, char**);
void    strvec_clear     (STRVEC);

//Get Values
sv_uint strvec_size      (STRVEC);
sv_uint strvec_length    (STRVEC, sv_uint);
char*   strvec_get       (STRVEC, sv_uint);
char**  strvec_get_array (STRVEC);
int     strvec_empty     (STRVEC);

//Deconstructor
void    strvec_free      (STRVEC);

#endif
