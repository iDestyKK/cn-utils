/*
 * File Lister
 *
 * Description:
 *     Acts like "ls", only it uses the cn_vec library and is written by me! :)
 *
 * Author:
 *     Clara Van Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "lib/cn_vec.h"

typedef struct directory {
    char*  path;
    char*  fname;
    CN_VEC file; //cn_vec of files
    CN_VEC dir;  //cn_vec of directories
	struct stat s_stat;
} DIRECTORY;

typedef struct dir_file {
    char* path;
    char* fname;
	struct stat s_stat;
} DIR_FILE;

typedef struct info {
    //Parameters
    cnv_byte recursion; //Recusion Enabler

	//File information
	cnv_byte permissions, //See permissions of the file
	         fsize,       //See filesize
			 nlink;       //See number of links

    //Program Specific
    cnv_byte first;     //Tells whether or not the first directory has been checked or not
} INFO;

void initialize_info(INFO* DATA) {
    DATA->recursion   = 0;
    DATA->first       = 0;
	DATA->permissions = 0;
	DATA->fsize       = 0;
	DATA->nlink       = 0;
}

void parse_command(char* str, INFO* DATA) {
    cnv_uint i  = 1, //The first character is always a '-'...
             ss = strlen(str);
    for (; i < ss; i++) {
        switch (str[i]) {
            case 'h':
                //Show Help (and exit)
                printf("Usage: fl [OPTION]... [FILE]...\n\nFile Lister by Clara Van Nguyen. Lists files and directories based on given input.\nThe current directory's contents are shown if no arguments are given.\n");
                printf("\nPossible Options:\n");
                printf("  -h\tShows this help prompt (And terminates the program)\n");
                printf("  -r\tEnable Recursion\n");
				printf("  -p\tShows file permissions\n");
                printf("  -s\tShows file sizes\n");
				printf("  -a\tShows all information about a file (Includes -p and -s)\n");

				exit(0);
                break;
            case 'r':
                //Enable Recursion
                DATA->recursion = 1;
                break;
			case 'p':
				//Enable showing permissions
				DATA->permissions = 1;
				break;
			case 's':
				//Enable showing filesizes
				DATA->fsize = 1;
				break;
			case 'a':
				//Enable showing all information
				DATA->permissions = 1;
				DATA->fsize = 1;
				DATA->nlink = 1;
				break;
        }
    }
}

int cmpfile(const void* a, const void* b) {
	DIR_FILE* fa = (DIR_FILE *) a;
	DIR_FILE* fb = (DIR_FILE *) b;

	return strcmp(fa->fname, fb->fname);
}

int cmpdir(const void* a, const void* b) {
	DIRECTORY* fa = (DIRECTORY *) a;
	DIRECTORY* fb = (DIRECTORY *) b;

	return strcmp(fa->fname, fb->fname);
}


void traverse_directory(CN_VEC vec, char* current_directory, INFO* DATA) {
    //Start with Root directory
    DIR* d;
    struct dirent *de;
    struct stat buf;
    int exists, _chk;
    char path[4096]; //ext4 limits paths to 4096 characters

    d = opendir(current_directory);

    if (d == NULL) return;
    DIRECTORY dir;
    dir.path  = strdup(current_directory);
    dir.fname = strdup(current_directory);
    dir.file = cn_vec_init(DIR_FILE);
    dir.dir  = cn_vec_init(DIRECTORY);

    for (de = readdir(d); de != NULL; de = readdir(d)) {
        /*if (current_directory[strlen(current_directory - 1)] == '/')
            sprintf(path, "%s%s", current_directory, de->d_name);
        else*/
            sprintf(path, "%s/%s", current_directory, de->d_name);
        exists = stat(path, &buf);
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            //Good. We won't be in a fucking loop this time...
            if (S_ISDIR(buf.st_mode) && !S_ISLNK(buf.st_mode)) {
                //It's a directory... and it isn't a stupid link.
                DIRECTORY new_dir;
                new_dir.path = strdup(path);
                new_dir.fname = strdup(de->d_name);
				new_dir.s_stat = buf;

                cn_vec_push_back(dir.dir, &new_dir);
            }
            else {
                //Either it is a file or a stupid link
                DIR_FILE new_file;
                new_file.path = strdup(path);
                new_file.fname = strdup(de->d_name);
				new_file.s_stat = buf;

                cn_vec_push_back(dir.file, &new_file);
            }
        }
    }

    closedir(d);
    cn_vec_push_back(vec, &dir);

    cnv_uint i;

    if (DATA->first == 0)
        DATA->first = 1;
    else
        printf("\n");
	
	//Sort the information
	qsort(cn_vec_data(dir.dir ), cn_vec_size(dir.dir ), sizeof(DIRECTORY), cmpdir );
	qsort(cn_vec_data(dir.file), cn_vec_size(dir.file), sizeof(DIR_FILE ), cmpfile);

	//Get the longest filesize (If we have to show the filesize).
	unsigned long long sllen = 0,
	                   lllen = 0;
	int sslen = 0,
	    lslen = 0;
	if (DATA->fsize) {
		DIRECTORY* dd;
		DIR_FILE * fd;
		for (i = 0; i < cn_vec_size(dir.dir); i++) {
			dd = cn_vec_at(dir.dir, i);
			if (dd->s_stat.st_size > sllen)
				sllen = dd->s_stat.st_size;
			if (dd->s_stat.st_nlink > lllen)
				lllen = dd->s_stat.st_nlink;
		}
		for (i = 0; i < cn_vec_size(dir.file); i++) {
			fd = cn_vec_at(dir.file, i);
			if (fd->s_stat.st_size > sllen)
				sllen = fd->s_stat.st_size;
			if (fd->s_stat.st_nlink > lllen)
				lllen = fd->s_stat.st_nlink;
		}

		while (sllen > 0) {
			sslen++;
			sllen *= (float)0.1;
		}

		while (lllen > 0) {
			lslen++;
			lllen *= (float)0.1;
		}
	}

    //Print out information
    printf("\e[0;30;47mDIRECTORY: %s\e[0m\n", current_directory);

    printf("DIRECTORY COUNT: %d\n", cn_vec_size(dir.dir));
    for (i = 0; i < cn_vec_size(dir.dir); i++) {
		DIRECTORY* fd = cn_vec_at(dir.dir, i);
		printf("  ");
		if (DATA->permissions) {
			printf((S_ISDIR(fd->s_stat.st_mode)) ? "d" : "-");
			printf((fd->s_stat.st_mode & S_IRUSR) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWUSR) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXUSR) ? "x" : "-");
			printf((fd->s_stat.st_mode & S_IRGRP) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWGRP) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXGRP) ? "x" : "-");
			printf((fd->s_stat.st_mode & S_IROTH) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWOTH) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXOTH) ? "x" : "-");
			printf(" ");
		}

		if (DATA->nlink)
			printf("%*d ", lslen, fd->s_stat.st_nlink);

		if (DATA->fsize)
			printf("%*llu ", sslen, (unsigned long long) fd->s_stat.st_size);

        printf("%s\n", fd->fname);
	}

    printf("\nFILE COUNT: %d\n", cn_vec_size(dir.file));
    for (i = 0; i < cn_vec_size(dir.file); i++) {
		DIR_FILE* fd = cn_vec_at(dir.file, i);
		printf("  ");
		if (DATA->permissions) {
			printf((S_ISDIR(fd->s_stat.st_mode)) ? "d" : "-");
			printf((fd->s_stat.st_mode & S_IRUSR) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWUSR) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXUSR) ? "x" : "-");
			printf((fd->s_stat.st_mode & S_IRGRP) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWGRP) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXGRP) ? "x" : "-");
			printf((fd->s_stat.st_mode & S_IROTH) ? "r" : "-");
			printf((fd->s_stat.st_mode & S_IWOTH) ? "w" : "-");
			printf((fd->s_stat.st_mode & S_IXOTH) ? "x" : "-");
			printf(" ");
		}
		
		if (DATA->nlink)
			printf("%*u ", lslen, fd->s_stat.st_nlink);

		if (DATA->fsize)
			printf("%*llu ", sslen, (unsigned long long) fd->s_stat.st_size);

        printf("%s\n", fd->fname);
	}

    //Now recursively traverse all directories inside this one...
    if (DATA->recursion) {
        for (i = 0; i < cn_vec_size(dir.dir); i++) {
            char* new_path = strdup(cn_vec_get(dir.dir, DIRECTORY, i).path); /*strdup(current_directory);
            strcat(new_path, "/");
            strcat(new_path, cn_vec_get(dir.dir, DIRECTORY, i).name);*/

            traverse_directory(vec, new_path, DATA);
            free(new_path);
        }
    }
}

main(int argc, char** argv) {
    INFO info;
    initialize_info(&info);

    //Make a vector of categories.
    CN_VEC vec = cn_vec_init(DIRECTORY);

    cnv_uint a = 1;

    for (; a < argc; a++)
        if (argv[a][0] != '-')
            traverse_directory(vec, argv[a], &info);
        else
            parse_command(argv[a], &info);

    //if (argc == 1 /*&& argv[1][0] != '-'*/)
    if (argc == 1 || (argc == 2 && argv[1][0] == '-'))
        traverse_directory(vec, ".", &info);

    cnv_uint i = 0,
             di, fi;

    /*for (; i < cn_vec_size(vec); i++) {
        DIRECTORY* cur_dir = (DIRECTORY*) cn_vec_at(vec, i);
        if (i > 0)
            printf("\n");
        printf("\e[0;30;47mDIRECTORY: %s\e[0m\n", cur_dir->path);
        //Traverse Directories first
        printf("DIRECTORY COUNT: %d\n", cn_vec_size(cur_dir->dir));
        for (di = 0; di < cn_vec_size(cur_dir->dir); di++) {
            DIRECTORY* dir_in = (DIRECTORY*) cn_vec_at(cur_dir->dir, di);
            printf("  %s\n", dir_in->fname);
        }
        printf("\n");

        //Traverse Files second
        printf("FILE COUNT: %d\n", cn_vec_size(cur_dir->file));
        for (fi = 0; fi < cn_vec_size(cur_dir->file); fi++) {
            DIR_FILE* file_in = (DIR_FILE*) cn_vec_at(cur_dir->file, fi);
            printf("  %s\n", file_in->fname);
        }
    }*/

    for (; i < cn_vec_size(vec); i++) {
        DIRECTORY* cur_dir = (DIRECTORY*) cn_vec_at(vec, i);
        //Traverse Directories first
        free(cur_dir->fname);
        free(cur_dir->path);
        for (di = 0; di < cn_vec_size(cur_dir->dir); di++) {
            DIRECTORY* dir_in = (DIRECTORY*) cn_vec_at(cur_dir->dir, di);
            free(dir_in->fname);
            free(dir_in->path);
        }

        //Traverse Files second
        for (fi = 0; fi < cn_vec_size(cur_dir->file); fi++) {
            DIR_FILE* file_in = (DIR_FILE*) cn_vec_at(cur_dir->file, fi);
            free(file_in->fname);
            free(file_in->path);
        }
        cn_vec_free(cur_dir->dir);
        cn_vec_free(cur_dir->file);
    }

    cn_vec_free(vec);
}
