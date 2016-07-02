/*
 * CN_RENDER
 *
 * Description:
 *     Traverses current directory and gathers information about AVI files, then
 *     exports the files to MP4 via ffmpeg. Must have package "ffmpeg" installed.
 *
 * Author:
 *     Clara Van Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "lib/cn_vec.h"

typedef struct dir_file {
    char* path;
    char* fname;
} DIR_FILE;

typedef struct info {
    //Parameters
    cnv_byte recursion; //Recusion Enabler

    //Program Specific
    cnv_byte first;     //Tells whether or not the first directory has been checked or not
} INFO;

void initialize_info(INFO* DATA) {
    DATA->first     = 0;
}

void show_help() {
    printf("Usage: cn_render [OPTION]...\n\nAVI2MP4 Concatenation by Clara Van Nguyen.\nTakes AVI files in the current directory and concatenates them all in alphabetical\norder into a single mp4 file. This requires the \"ffmpeg\" package to be installed.\n");
    printf("\nPossible Options:\n");
    printf("  -h\tShows this help prompt (And terminates the program)\n");
}

void parse_command(char* str, INFO* DATA) {
    cnv_uint i  = 1, //The first character is always a '-'...
    ss = strlen(str);
    for (; i < ss; i++) {
        switch (str[i]) {
            case 'h':
                //Show Help (and exit)
                show_help();
                exit(0);
                break;
        }
    }
}

int cmpfile(const void* a, const void* b) {
    DIR_FILE* fa = (DIR_FILE *) a;
    DIR_FILE* fb = (DIR_FILE *) b;

    return strcmp(fa->fname, fb->fname);
}

int substr_count(char* str, char substr) {
    char* av = str;
    int occur;
    for (occur = 0; av[occur]; av[occur]==substr ? occur++ : *av++);
    return occur;
}

int main(int argc, char** argv) {
    INFO info;
    initialize_info(&info);

    //Make a vector of categories.
    CN_VEC vec = cn_vec_init(DIR_FILE);

    cnv_uint a = 1;

    for (; a < argc; a++)
        //if (argv[a][0] == '-')
        parse_command(argv[a], &info);

    //if (argc == 1 /*&& argv[1][0] != '-'*/)
    if (argc == 1 || (argc == 2 && argv[1][0] == '-')) {
        //Traverse the current directory for files
        //traverse_directory(vec, ".", &info);

        DIR* d;
        struct dirent *de;
        struct stat buf;
        int exists, _chk;
        char path[4096]; //ext4 limits paths to 4096 characters
        char* current_directory = ".";

        d = opendir(current_directory);

        if (d == NULL) exit(1);

        for (de = readdir(d); de != NULL; de = readdir(d)) {
            sprintf(path, "%s/%s", current_directory, de->d_name);
            exists = stat(path, &buf);
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                //Good. We won't be in a fucking loop this time...
                if (S_ISDIR(buf.st_mode) && !S_ISLNK(buf.st_mode)) {
                    //Do nothing
                }
                else {
                    //It is a file. Push back the file into the CN_Vec.
                    DIR_FILE new_file;
                    new_file.path  = strdup(path);
                    new_file.fname = strdup(de->d_name);

                    cn_vec_push_back(vec, &new_file);
                }
            }
        }

        //We are done here.
        closedir(d);

        //Write the file "list.txt".
        FILE* fp = fopen("list.txt", "w");
        if (!fp) {
            fprintf(stderr, "\e[1;31m[ ERROR ]\x1b[0m Unable to create \"list.txt\" in directory.\n");
            exit(1);
        }
        printf("\e[0;30;47mRENDER QUEUE (Number of Files: %d)\e[0m\n", cn_vec_size(vec));

        //Sort the array of files in alphabetical order
        qsort(cn_vec_data(vec), cn_vec_size(vec), sizeof(DIR_FILE), cmpfile);

        //Write the files to the file
        DIR_FILE* ii;
        cn_vec_traverse(vec, ii) {
            printf("  -%s\n", ii->fname);
            fprintf(fp, "file '%s'\n", ii->fname);
        }

        fclose(fp);

        int fk = fork();
        if (fk == 0) {
            //We are the child.
            //Configure ffmpeg.
            //char* argnv[23];
            char* argval = "ffmpeg -safe 0 -hide_banner -f concat -i list.txt -strict -2 -pix_fmt yuvj420p -vcodec libx264 -b:v 12000k -maxrate 12000k -bufsize 12000k -acodec aac -f mp4 final.mp4";
            int argcount = substr_count(argval, ' ') + 1,
                i, j, k, strleng, strlent;

            char* argnv[argcount + 1];
            j = 0;
            strlent = strlen(argval);
            for (i = 0; i < argcount; i++) {
                k = j; //Set the current position to k.
                strleng = 0;
                for (;;j++) {
                    if (argval[j] == ' ' || j >= strlent) {
                        j += 1;
                        break;
                    }
                    strleng++;
                }
                argnv[i] = (char*) malloc(sizeof(char) * (strleng + 1));
                j = k;
                for (;;j++) {
                    if (argval[j] == ' ' || j >= strlent) {
                        j += 1;
                        break;
                    }
                    argnv[i][j - k] = argval[j];
                }
                argnv[i][strleng] = '\0';
                //printf("%s\n", argnv[i]);
            }
            argnv[argcount] = NULL; //The char** is NULL-terminated.

            //Execute ffmpeg.
            fk = execvp(argnv[0], argnv);

            //Assume failure at this point. Print Error Message.
            fprintf(stderr, "\e[1;31m[ Error ]\x1b[0m ");
            perror("");
            exit(1);
        }
        else {
            //We are the parent
            //Wait for the child process to finish.
            int j, wt;
            j = wait(&wt);
            if (WIFEXITED(wt))
                printf("The video has been rendered successfully!\n");
        }
    }

    //Free up memory
    printf("Freeing Memory... ");
    cn_vec_free(vec);
    printf("Done!\n");

    //Delete files
    printf("Deleting Temporary Files... ");
    remove("list.txt");
    printf("Done!\n");

    //Have a nice day.
    return;
}
