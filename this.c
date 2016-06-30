/*
 * THIS, a file information viewer
 * 
 * THIS will gather information about files and print it
 * to stdout. This is basic information from sys/stat.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

typedef unsigned int u_int32;

void print_help() {
	fprintf(stderr, "Usage: this [FILENAMES]...\n\n"
	                "THIS, a file information viewer by Clara Van Nguyen.\n"
	                "THIS takes any number of arguments and will show information about\n"
					"files specified in the arguments.\n\n"
					"There are no special parameters needed (nor allowed) to get the\n"
					"information for the file. This program simply uses the stat library\n"
					"to get the data and output it.\n");
}

main(int argc, char** argv) {
	if (argc == 1) {
		//This program needs arguments so...
		print_help();
		exit(0);
	}
	
	u_int32 i = 1;
	for (; i < argc; i++) {
		//Parse the file information
		struct stat buf;
		if (stat(argv[i], &buf) == 0) {
			printf("\e[0;30;47mPath: %s\e[0m\n", argv[i]);
			printf("  %-*s: ", 13, "Type");
			if (S_ISDIR(buf.st_mode)) {
				//Directory!
				printf("\e[1;34mDirectory\e[0m\n");
			} else
			if (S_ISLNK(buf.st_mode)) {
				//Symbolic Link!
				printf("\e[1;36mSymbolic Link\e[0m\n");
			} else {
				//Must be a file
				printf("\e[1;31mFile\e[0m\n");
			}
		} else {
			//Something went wrong. Report the error message.
		}
		
		//Other file information.
		//Full Path
		char actual_path[PATH_MAX];
		char* path;
		path = realpath(argv[i], actual_path);

		printf("  %-*s: %s\n", 13, "Full Path", path);

		//Permissions
		printf("  %-*s: ", 13, "Permissions");
		printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
		printf((buf.st_mode & S_IRUSR) ? "r" : "-");
		printf((buf.st_mode & S_IWUSR) ? "w" : "-");
		printf((buf.st_mode & S_IXUSR) ? "x" : "-");
		printf((buf.st_mode & S_IRGRP) ? "r" : "-");
		printf((buf.st_mode & S_IWGRP) ? "w" : "-");
		printf((buf.st_mode & S_IXGRP) ? "x" : "-");
		printf((buf.st_mode & S_IROTH) ? "r" : "-");
		printf((buf.st_mode & S_IWOTH) ? "w" : "-");
		printf((buf.st_mode & S_IXOTH) ? "x" : "-");
		printf("\n");

		//Filesize
		printf("  %-*s: %llu\n", 13, "File Size", (unsigned long long)buf.st_size);

		//Number of links
		printf("  %-*s: %u\n", 13, "Links", buf.st_nlink);

		//Owners
		struct group *grp;
		struct passwd *uid;
		grp = getgrgid(buf.st_gid);
		uid = getpwuid(buf.st_uid);
		printf("  %-*s: %s\n  %-*s: %s\n", 13, "User", uid->pw_name, 13, "Group", grp->gr_name);

		//inode number
		printf("  %-*s: %lu\n", 13, "Inode Number", buf.st_ino);

		//Device ID
		printf("  %-*s: %llu\n", 13, "Device ID", buf.st_dev);

		//If this isn't the last entry, we will make sure it skips the next line for neatness.
		if (i < argc - 1)
			printf("\n");
	}
}
