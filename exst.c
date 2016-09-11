/*
 * exst (Extension Statistics)
 * 
 * Returns information about files in a directory by extension. The use of this
 * is to tell what percentage of files in a directory is used by a particular
 * type of file.
 *
 * Clara Van Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

//CNDS Includes
#include "lib/cn_comp.h"
#include "lib/cn_map.h"

typedef unsigned int e_uint;
const char* NA = "[n/a]";

typedef struct total_info {
	e_uint total_size,
	       name_size,
	       size_size;
} TOTAL_INFO;

typedef struct ext_info {
	e_uint total_size;
} EXT_INFO;

//Get file extension
const char* get_extension_sh(const char* fname) {
	//sh = shorthand, assuming you guarantee that the
	//fname inserted is the file only.

	const char* ext  = strrchr(fname , '.');
	if (!ext)
		return "";
	return ext + 1;
}

const char* get_extension(const char* fname) {
	//Get file name, then extract last .

	const char* file = strrchr(fname, '\\');
	return get_extension_sh(file);
}

void print_extension_details(CN_MAP emap, TOTAL_INFO* data) {
	printf("Analysed %d bytes\n\nEXTENSION DATA\n", data->total_size);
	CNM_ITERATOR* iterator = cn_map_begin(emap);
	while (iterator->node != NULL) {
		printf("  %*s - %*d bytes (%*.2f\%%)\n", data->name_size, *(char**)iterator->node->key,
		    data->size_size, cn_map_deref(iterator->node->data, EXT_INFO).total_size,
		    6, 100. * ((double)cn_map_deref(iterator->node->data, EXT_INFO).total_size / data->total_size));
		iterator = cn_map_next(emap, iterator);
	}
	cn_map_iterator_free(iterator);
}

void show_help() {
	printf("Usage: exst [OPTION]... [DIRECTORY]...\n\nExtension Statistics by Clara Van Nguyen.\nLists extensions of files in a folder and tells how much space is taken by each respectively.\nThe current directory's contents are shown if no arguments are given.\n");
    printf("\nPossible Options:\n");
    printf("  -h\tShows this help prompt (And terminates the program)\n");
}

main(int argc, char** argv) {
	unsigned int i = 1;
	char* working_directory = "."; //Default
	
	for (; i < argc; i++) {
		if (argv[i][0] == '-') {
			//This is an argument... at least we are pretty sure it is.
			switch (argv[i][1]) {
				case 'h':
					show_help();
					exit(0);
					break; //lol
			}
		} else {
			working_directory = argv[i];
		}
	}
	
	
	//Initialise our data structures
	TOTAL_INFO data;
	data.total_size = 0;
	data.name_size  = 0;
	data.size_size  = 0;
	
	CN_MAP emap = cn_map_init(void*, EXT_INFO, cn_cmp_cstr);
	
	DIR *d;
	struct dirent *de;
	struct stat buf;
	char new_dir[4096];
	int exists, _chk;
	
	d = opendir(working_directory);
	
	if (d == NULL) return;
	
	for (de = readdir(d); de != NULL; de = readdir(d)) {
		sprintf(new_dir, "%s/%s", working_directory, de->d_name);
		exists = stat(new_dir, &buf);
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		if (S_ISDIR(buf.st_mode) && !S_ISLNK(buf.st_mode)) {
		}
		else {
			char* ext = get_extension_sh(de->d_name) - 1;
			CNM_NODE* ext_node;
			if (strcmp(ext, "") == 0) {
				ext_node = cn_map_find(emap, &NA);
				if (ext_node == NULL) {
					cn_map_insert_blank(emap, &NA);
					ext_node = cn_map_find(emap, &NA);
					if (data.name_size < 5)
						data.name_size = 5;
				}
			} else {
				char* _EXT = strdup(ext);
				ext_node = cn_map_find(emap, &_EXT);
				if (ext_node == NULL) {
					cn_map_insert_blank(emap, &_EXT);
					int __sz = strlen(ext);
					ext_node = cn_map_find(emap, &_EXT);
					if (data.name_size < __sz)
						data.name_size = __sz;
				} else
					free(_EXT);
			}
			if (ext_node != NULL) {
				cn_map_deref(ext_node->data, EXT_INFO).total_size += buf.st_size;
				data.total_size += buf.st_size;
				unsigned long long __tsz = cn_map_deref(ext_node->data, EXT_INFO).total_size;
				unsigned char      __tsl = 0;
				while (__tsz > 0) {
					__tsz *= 0.1;
					__tsl++;
				}
				if (__tsl > data.size_size)
					data.size_size = __tsl;
			}
		
			//printf("%s %s\n", de->d_name, get_extension_sh(de->d_name) - 1);
		}
	}
	closedir(d);
	
	//__cn_map_print_tree(emap);
	print_extension_details(emap, &data);
	
	//Clean up
	cn_map_free(emap);
}
