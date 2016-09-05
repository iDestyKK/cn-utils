/*
 * CN_Map Library
 *
 * Version 0.1.3 (Last Updated: 2016-08-26)
 *
 * Description:
 *     C++ Maps for C library. Implements the data structure with a struct and
 *     Red-Black Trees. Any data type can be stored in a CN_Map, just like C++
 *     Maps. CN_Maps require specification of two file types. One for the key,
 *     and another for what data type the tree node will store. It will also
 *     require a comparison function to sort the tree.
 *
 *     It is highly recommended to use this along with the CN_Comp library, as
 *     it contains comparison functions for C data types to use in the CN_Map.
 *
 * Changelog of library is located at the bottom of this file.
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_map/index.html
 */

#ifndef __CN_MAP__
#define __CN_MAP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Custom Types
typedef unsigned int       cnm_uint;
typedef unsigned long long cnm_u64;
typedef unsigned char      cnm_byte;

typedef enum cnm_colour {
	CNM_BLACK,
	CNM_RED
} CNM_COLOUR;

#ifndef __CN_COMP__
	typedef int CNC_COMP;
#endif

//Structs
typedef struct cnm_node {
	void* key,
		* data;
	struct cnm_node* left,
	               * right,
	               * up;
	CNM_COLOUR colour;
} CNM_NODE;

typedef struct cnm_iterator {
	struct cnm_node* prev,
	               * node;
	cnm_uint count;
} CNM_ITERATOR;

typedef struct cn_map {
	struct cnm_node* head,
	               * least,
	               * most;
	cnm_uint key_size,
	         elem_size,
	         size;
	CNC_COMP (*cmpfunc)(void*, void*);
} *CN_MAP;

typedef CN_MAP MAP; //For you C++ people...

//Functions
//Initialiser
CN_MAP        new_cn_map          (cnm_uint, cnm_uint, CNC_COMP (*__func)(void*, void*));

//Add
void          cn_map_insert       (CN_MAP, void*, void*);
void          cn_map_insert_blank (CN_MAP, void*);

//Set

//Modify
void          cn_map_clear        (CN_MAP);

//Get
CNM_NODE*     cn_map_find         (CN_MAP, void*);
cnm_uint      cn_map_size         (CN_MAP);

//Iteration
CNM_ITERATOR* cn_map_begin        (CN_MAP);
CNM_ITERATOR* cn_map_end          (CN_MAP);
CNM_ITERATOR* cn_map_rbegin       (CN_MAP);
CNM_ITERATOR* cn_map_rend         (CN_MAP);
CNM_ITERATOR* cn_map_prev         (CN_MAP, CNM_ITERATOR*);
CNM_ITERATOR* cn_map_next         (CN_MAP, CNM_ITERATOR*);
void          cn_map_iterator_free(CNM_ITERATOR*);

//Destructor
void          cn_map_free         (CN_MAP);

//Functions you won't use if you are sane
CNM_NODE*     __cn_map_create_node      (void*, void*, cnm_uint, cnm_uint);
void          __cn_map_free_node        (CNM_NODE*);
void          __cn_map_print_tree       (CN_MAP);
void          __cn_map_print_nodes      (CNM_NODE*, cnm_uint);
void          __cn_map_proceed_insert   (CN_MAP, void*, CNM_NODE*);
void          __cn_map_nodes_adjust     (CN_MAP, CNM_NODE*);
void          __cn_map_node_rotate_left (CN_MAP, CNM_NODE*);
void          __cn_map_node_rotate_right(CN_MAP, CNM_NODE*);
void          __cn_map_calculate_edge   (CN_MAP);
void          __cn_map_recursive_free   (CN_MAP, CNM_NODE*);

//Macros
#define cn_map_init(key_type, elem_type, __func) \
	new_cn_map(sizeof(key_type), sizeof(elem_type), __func)

#define cn_map_deref(ptr, type) \
	(*(type*)ptr) //lol cheat

#define cn_map_find_deref(map, key, type) \
	(*(type*)cn_map_find(map, key)->data)

#ifndef deref
	#define deref(ptr, type) \
		(*(type*)ptr)
#endif

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *
\***************************************/

/*
    2016-08-26 (0.1.3)
      - Fixed up-node misplacement after rotation.

    2016-08-19 (0.1.1)
      - Added Balancing via recolouring and rotation.

    2016-08-16 (0.0.1)
      - Started writing CN_Map. Doesn't use Red-Black Trees as of now.    
*/
