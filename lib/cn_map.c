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
 * Changelog of library is located at the bottom of "cn_map.h".
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_map/index.html
 */

#include "cn_map.h"

//Functions
//Initialiser
CN_MAP new_cn_map(cnm_uint key_size, cnm_uint elem_size, CNC_COMP (*__func)(void*, void*)) {
	CN_MAP map     = (CN_MAP) malloc(sizeof(struct cn_map));
	map->size      = 0;
	map->key_size  = key_size;
	map->elem_size = elem_size;
	map->head      = NULL;
	map->cmpfunc   = __func;

	return map;
}

//Add
void cn_map_insert(CN_MAP map, void* key, void* ptr) {
	//Create the node
	CNM_NODE* node = __cn_map_create_node(key, ptr, map->key_size, map->elem_size);
	__cn_map_proceed_insert(map, key, node);
	__cn_map_calculate_edge(map);
}

void cn_map_insert_blank(CN_MAP map, void* key) {
	//Create the node
	CNM_NODE* node = __cn_map_create_node(key, NULL, map->key_size, map->elem_size);
	__cn_map_proceed_insert(map, key, node);
	__cn_map_calculate_edge(map);
}

//Set

//Modify
void cn_map_clear(CN_MAP map) {
	__cn_map_recursive_free(map, map->head);
}

//Get
CNM_NODE* cn_map_find(CN_MAP map, void* key) {
	if (map->head == NULL)
		return NULL;

	CNM_NODE* cur_node = map->head;
	CNC_COMP  compare;
	while (1) {
		compare = map->cmpfunc(key, cur_node->key);
		if (compare == 0) {
			//We have our match.
			return cur_node;
			break;
		}
		if (compare < 0) {
			if (cur_node->left == NULL)
				return NULL;
			else
				cur_node = cur_node->left;
		}
		if (compare > 0) {
			if (cur_node->right == NULL)
				return NULL;
			else
				cur_node = cur_node->right;
		}
	}
}

cnm_uint cn_map_size(CN_MAP map) {
	return map->size;
}

//Iteration
CNM_ITERATOR* cn_map_begin(CN_MAP map) {
	//Move along the left side of the tree. The left-most node is guaranteed
	//to be the lowest valued number.
	CNM_ITERATOR* iterator = (CNM_ITERATOR *) malloc(sizeof(struct cnm_iterator));
	iterator->count = 0;
	if (map->head == NULL) {
		iterator->node = NULL;
		return iterator;
	}
	CNM_NODE* cur_node = map->head;
	/*while (1) {
		if (cur_node->left != NULL) {
			//printf("yes\n");
			iterator->prev = cur_node;
			cur_node = cur_node->left;
			//printf("%d\n", *(int*)cur_node->key);
		} else {
			iterator->node = cur_node;
			break;
		}
	}*/

	//BOOM, CONSTANT TIME IMPLEMENTATION
	iterator->node = map->least;
	if (iterator->node->up != NULL)
		iterator->prev = iterator->node->up;
	else
		iterator->prev = NULL;


	return iterator;
}

CNM_ITERATOR* cn_map_end(CN_MAP map) {
	return NULL;
}

CNM_ITERATOR* cn_map_rbegin(CN_MAP map) {
	//Move along the right side of the tree. The right-most node is guaranteed
	//to be the highest valued number.
	CNM_ITERATOR* iterator = (CNM_ITERATOR *) malloc(sizeof(struct cnm_iterator));
	iterator->count = 0;
	if (map->head == NULL) {
		iterator->node = NULL;
		return iterator;
	}
	/*CNM_NODE* cur_node = map->head;
	while (1) {
		if (cur_node->right != NULL) {
			iterator->prev = cur_node;
			cur_node = cur_node->right;
		} else {
			iterator->node = cur_node;
			break;
		}
	}*/

	//BOOM, CONSTANT TIME IMPLEMENTATION
	iterator->node = map->most;
	if (iterator->node->up != NULL)
		iterator->prev = iterator->node->up;
	else
		iterator->prev = NULL;

	return iterator;
}

CNM_ITERATOR* cn_map_rend(CN_MAP map) {
	return NULL;
}

CNM_ITERATOR* cn_map_prev(CN_MAP map, CNM_ITERATOR* iterator) {
	
}

CNM_ITERATOR* cn_map_next(CN_MAP map, CNM_ITERATOR* iterator) {
	//TAKE 3
	if (iterator->node == NULL) {
		//Nice try
		iterator->prev = NULL;
		return iterator;
	}
	else
	if (iterator->node == map->most) {
		iterator->prev = iterator->node;
		iterator->node = NULL;
	}
	else
	if (iterator->prev == iterator->node->left && iterator->node->right == NULL) {
		if (iterator->node->up != NULL) {
			iterator->prev = iterator->node;
			iterator->node = iterator->node->up;
			while (iterator->prev == iterator->node->right) {
				//We need to go up
				if (iterator->node->up != NULL) {
					iterator->prev = iterator->node;
					iterator->node = iterator->node->up;
				}
			}
		}
	}
	else
	if (iterator->prev == iterator->node->left && iterator->node->right != NULL) {
		//Go to the right and as far to the left as possible
		iterator->prev = iterator->node;
		iterator->node = iterator->node->right;
		while (iterator->node->left != NULL) {
			iterator->prev = iterator->node;
			iterator->node = iterator->node->left;
		}
	}
	else
	if (iterator->node->left == NULL && iterator->node->right == NULL) {
		//No more parents... We need to go up
		if (iterator->node->up != NULL) {
			iterator->prev = iterator->node;
			iterator->node = iterator->node->up;
			while (iterator->prev == iterator->node->right) {
				//We need to go up
				if (iterator->node->up != NULL) {
					iterator->prev = iterator->node;
					iterator->node = iterator->node->up;
				}
			}
		}
		else {
			//We are back at the top of the tree
			iterator->prev = NULL;
			iterator->node = NULL;
			return iterator;
		}
	}
	else {
		while (iterator->prev == iterator->node->right) {
			//We need to go up
			if (iterator->node->up != NULL) {
				iterator->prev = iterator->node;
				iterator->node = iterator->node->up;
			}
		}
		if (iterator->node->left != NULL && iterator->prev != iterator->node->left) {
			while (iterator->node->left != NULL) {
				iterator->prev = iterator->node;
				iterator->node = iterator->node->left;
			}
		}
		else
		if (iterator->node->right != NULL && iterator->prev != iterator->node->right) {
			//Go down and as far left as possible
			iterator->prev = iterator->node;
			iterator->node = iterator->node->right;
			while (iterator->node->left != NULL) {
				iterator->prev = iterator->node;
				iterator->node = iterator->node->left;
			}
		}
	}
	return iterator;
}

void cn_map_iterator_free(CNM_ITERATOR* iterator) {
	free(iterator);
}

//Destructor
void cn_map_free(CN_MAP map) {
	if (map->size > 0)
		cn_map_clear(map);
	free(map);
}

//Functions you won't use if you are sane
CNM_NODE* __cn_map_create_node(void* key, void* ptr, cnm_uint key_size, cnm_uint elem_size) {
	CNM_NODE* node = (CNM_NODE*) malloc(sizeof(struct cnm_node));
	node->key    = (void*) malloc(key_size );
	node->data   = (void*) malloc(elem_size);
	node->up     = NULL;
	node->left   = NULL;
	node->right  = NULL;
	node->colour = CNM_BLACK;

	//Copy data over
	memcpy(node->key , key, key_size );
	if (ptr == NULL)
		memset(node->data, 0  , elem_size);
	else
		memcpy(node->data, ptr, elem_size);

	return node;
}

void __cn_map_free_node(CNM_NODE* ptr) {
	if (ptr->key  != NULL) free(ptr->key );
	if (ptr->data != NULL) free(ptr->data);

	free(ptr);
}

void __cn_map_print_tree(CN_MAP map) {
	printf("Key   : %s\nValue : %d\nColour: %s\nLevel : 0\n", (char*)map->head->key, *(int*)map->head->data, !map->head->colour ? "BLACK" : "RED");
	
	if (map->head->left  != NULL) printf(" - Has Left Node\n");
	if (map->head->right != NULL) printf(" - Has Right Node\n");
	
	//Recursively search
	if (map->head->left  != NULL) __cn_map_print_nodes(map->head->left , 1);
	if (map->head->right != NULL) __cn_map_print_nodes(map->head->right, 1);
}

void __cn_map_print_nodes(CNM_NODE* ptr, cnm_uint level) {
	printf("\nKey   : %s\nValue : %d\nColour: %s\nLevel : %d\nUp Key: %s\n", (char*)ptr->key, *(int*)ptr->data, !ptr->colour ? "BLACK" : "RED", level, (char*)ptr->up->key);
	
	if (ptr->left  != NULL) printf(" - Has Left Node\n");
	if (ptr->right != NULL) printf(" - Has Right Node\n");
	
	if (ptr->left  != NULL) __cn_map_print_nodes(ptr->left , level + 1);
	if (ptr->right != NULL) __cn_map_print_nodes(ptr->right, level + 1);
}

void __cn_map_proceed_insert(CN_MAP map, void* key, CNM_NODE* node) {
	node->colour   = CNM_RED;
	
	//Now figure out where to put it in the tree.
	if (map->size == 0) {
		//Well that was easy...
		map->head         = node;
		map->head->colour = CNM_BLACK;
		map->size++;
	} else {
		CNM_NODE* cur_node = map->head;
		CNC_COMP  compare;
		while (1) {
			compare = map->cmpfunc(key, cur_node->key);
			if (compare < 0) {
				//LESS THAN
				if (cur_node->left == NULL) {
					cur_node->left = node;
					node->up = cur_node;
					break;
				} else
					cur_node = cur_node->left;
			}
			if (compare == 0) {
				//EQUAL
				__cn_map_free_node(node);
				fprintf(stderr, "WARNING: Duplicate Key\n");
				break;
			}
			if (compare > 0) {
				//GREATER THAN
				if (cur_node->right == NULL) {
					cur_node->right = node;
					node->up = cur_node;
					break;
				} else
					cur_node = cur_node->right;
			}
		}
		if (compare != 0)
			map->size++;
		
		//if (*(int*)node->key != 10)
		__cn_map_nodes_adjust(map, node);
	}
}

void __cn_map_nodes_adjust(CN_MAP map, CNM_NODE* node) {
	if (node == NULL || node->up == NULL)
		return; //You're a failure
	
	CNM_NODE* sibling = NULL;
	
	//Detect Double Red Problem
	if (node->up->colour == CNM_RED) {
		//We will attempt to recolour
		//printf("Attempting Recolour\n");
		if (node->up->up != NULL) {
			//Get the sibling node... lol
			if (node->up == node->up->up->left  && node->up->up->right != NULL)
				sibling = node->up->up->right;
			if (node->up == node->up->up->right && node->up->up->left  != NULL)
				sibling = node->up->up->left;
			
			if (sibling != NULL && sibling->colour == CNM_RED) {
				sibling->colour  = CNM_BLACK;
				node->up->colour = CNM_BLACK;
				node->up->up->colour = CNM_RED;
				__cn_map_nodes_adjust(map, node->up->up);
			}
			else
			if (node->up == node->up->up->left) {
				if (node == node->up->right) {
					__cn_map_node_rotate_left(map, node->up);
				}
				if (node->left != NULL && node->colour == CNM_RED && node->left->colour == CNM_RED) {
					__cn_map_node_rotate_right(map, node->up);
					node->colour       = CNM_BLACK;
					node->right->colour = CNM_RED;
				} else {
					node->up->colour     = CNM_BLACK;
					node->up->up->colour = CNM_RED;
					__cn_map_node_rotate_right(map, node->up->up);
				}
			}
			else
			if (node->up == node->up->up->right) {
				if (node == node->up->left) {
					__cn_map_node_rotate_right(map, node->up);
				}
				//__cn_map_print_tree(map);
				if (node->right != NULL && node->colour == CNM_RED && node->right->colour == CNM_RED) {
					__cn_map_node_rotate_left(map, node->up);
					node->colour       = CNM_BLACK;
					node->left->colour = CNM_RED;
				} else {
					node->up->colour     = CNM_BLACK;
					node->up->up->colour = CNM_RED;
					__cn_map_node_rotate_left(map, node->up->up);
				}
			}
		}
	}
	
	//Colour the root node black... just in case it became red.
	map->head->colour = CNM_BLACK;
}

void __cn_map_node_rotate_left(CN_MAP map, CNM_NODE* node) {
	//printf("Rotating Left (%d)\n", *(int*)node->key);
	if (node == NULL)
		return;
	
	if (node == map->head)
		map->head = node->right;
	
	CNM_NODE* parent = node->up,
	        * new_up = node->right,
	        * trans  = new_up->left;
	char    __dir = -1;
	
	if (parent != NULL) {
		//Adjust the parent's node too.
		__dir = (node == parent->right); //If it isn't the right, it is guaranteed to be the left
	}
	
	new_up->left = node;
	new_up->up = node->up;
	node->up = new_up;

	//new_up->colour = CNM_BLACK;
	//node->colour   = CNM_RED;
	
	if (trans != NULL)
		node->right = trans;
	else
		node->right = NULL;
	
	if (__dir != -1 && parent != NULL)
		if (__dir == 1)
			parent->right = new_up;
		else
			parent->left  = new_up;

	//Apply a quick fix if needed
	if (new_up->left  != NULL) new_up->left ->up = new_up;
	if (new_up->right != NULL) new_up->right->up = new_up;
	
	//Also down one
	if (new_up->left != NULL) {
		if (new_up->left->left  != NULL) new_up->left->left ->up = new_up->left;
		if (new_up->left->right != NULL) new_up->left->right->up = new_up->left;
	}
	if (new_up->right != NULL) {
		if (new_up->right->left  != NULL) new_up->right->left ->up = new_up->right;
		if (new_up->right->right != NULL) new_up->right->right->up = new_up->right;
	}
}

void __cn_map_node_rotate_right(CN_MAP map, CNM_NODE* node) {
	//printf("Rotating Right (%d)\n", *(int*)node->key);
	if (node == NULL)
		return;
	
	if (node == map->head)
		map->head = node->left;
	
	CNM_NODE* parent = node->up,
	        * new_up = node->left,
	        * trans  = new_up->right;
	char    __dir = -1;
	
	if (parent != NULL) {
		//Adjust the parent's node too.
		__dir = (node == parent->right); //If it isn't the right, it is guaranteed to be the left
	}
	
	new_up->right = node;
	new_up->up = node->up;
	node->up = new_up;

	//new_up->colour = CNM_BLACK;
	//node->colour   = CNM_RED;
	
	if (trans != NULL)
		node->left = trans;
	else
		node->left = NULL;
	
	if (__dir != -1 && parent != NULL)
		if (__dir == 1)
			parent->right = new_up;
		else
			parent->left  = new_up;

	//Apply a quick fix if needed
	if (new_up->left  != NULL) new_up->left ->up = new_up;
	if (new_up->right != NULL) new_up->right->up = new_up;

	//Also down one
	if (new_up->left != NULL) {
		if (new_up->left->left  != NULL) new_up->left->left ->up = new_up->left;
		if (new_up->left->right != NULL) new_up->left->right->up = new_up->left;
	}
	if (new_up->right != NULL) {
		if (new_up->right->left  != NULL) new_up->right->left ->up = new_up->right;
		if (new_up->right->right != NULL) new_up->right->right->up = new_up->right;
	}

}

void __cn_map_calculate_edge(CN_MAP map) {
	map->least = map->head;
	map->most  = map->head;
	
	while (map->least->left != NULL)
		map->least = map->least->left;
	while (map->most->right != NULL)
		map->most = map->most->right;
}

void __cn_map_recursive_free(CN_MAP map, CNM_NODE* node) {
	if (node->left != NULL)
		__cn_map_recursive_free(map, node->left);
	if (node->right != NULL)
		__cn_map_recursive_free(map, node->right);

	free(node->key);
	free(node->data);
	free(node);
}
