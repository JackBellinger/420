#define _XOPEN_SOURCE
#define __STDC_LIB_EXT1__
#include <unistd.h>
#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <string.h>
#include <math.h>
#include <crypt.h>
#include <errno.h>
#include <mpi.h> // mpi stuff

int num_words = 235886;

void print_char_vals(char* x)
{
	printf("printing\n");
	int i;
	for(i = 0; i < strlen(x); i++)
		printf("%d ", x[i]);
	printf("\n");
}
MPI_Offset find_offset(int rank, int num_nodes, MPI_File* file)
{//sets the offset of file to where the node should start
//and returns the length of how many bytes it should read
	MPI_File_seek(file, 0, MPI_SEEK_END);
	MPI_Offset file_len;
	MPI_File_get_size(file, &file_len);
	MPI_Offset starting_point = (file_len / num_nodes) * rank;
	MPI_Offset ending_point = (file_len / num_nodes) * (rank + 1) -1;




	MPI_Status status;
	char c = ' ';
	printf("Node#%d starting at %d\n", rank, starting_point);
	MPI_File_seek(file, starting_point, MPI_SEEK_SET);
	if(rank != 0)
	{
		while( c != '\n' && c != 0)
		{
			//MPI_File_seek(file, -2, MPI_SEEK_CUR);
			MPI_File_read(file, &c, 1, MPI_CHAR, &status);
			printf("Node #%d s reading char %c\n", rank, c);
		}
		MPI_File_seek(file, -1, MPI_SEEK_CUR);
		MPI_File_get_position(file, &starting_point);
	}
	printf("Node#%d starting at %d\n", rank, starting_point);

	c = ' ';
	MPI_File_seek(file, ending_point, MPI_SEEK_SET);
	while( c != '\n'  && c != 0)
	{
		//MPI_File_seek(file, -2, MPI_SEEK_CUR);
		MPI_File_read(file, &c, 1, MPI_CHAR, &status);
		printf("Node #%d e reading char %c\n", rank, c);
	}
	MPI_File_seek(file, 1, MPI_SEEK_CUR);

	MPI_File_get_position(file, &ending_point);
	printf("Node #%d ended at %d\n", rank, ending_point);//minus 2 so it goes back past the newline
	MPI_File_seek(file, starting_point, MPI_SEEK_SET);
	return ending_point;
}

int main(int argc, char** argv)
{
	MPI_Init(NULL, NULL);

	MPI_Comm world = MPI_COMM_WORLD;
	int rank, world_size;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &world_size);
	MPI_File mpi_file;
	MPI_File_open(
		world,
		"test_offset.txt",
		MPI_MODE_RDONLY,
		MPI_INFO_NULL,
		&mpi_file
	);

	//FILE* test = fopen("test_offset.txt", "r");
	find_offset(rank, world_size, mpi_file);
	//printf("Node#%d should start at %d and end at ")

	MPI_Finalize();
	return 0;
	FILE* shadow_fp;
	FILE* words_fp;

	shadow_fp = fopen("shadow", "r");
	if (shadow_fp == NULL)
		exit(EXIT_FAILURE);
	words_fp = fopen("words", "r");
	if (words_fp == NULL)
		exit(EXIT_FAILURE);

	int num_users = 0;
	char c;
	// Extract characters from file and store in character c
	for (c = getc(shadow_fp); c != EOF; c = getc(shadow_fp))
		if (c == '\n') // Increment count if this character is newline
			num_users++;

	char** user_names = (char**) calloc(num_users, sizeof(char*));
	char** user_pass_hashes = (char**) calloc(num_users, sizeof(char*));
	char** user_passwords = (char**) calloc(num_users, sizeof(char*));

	int i;
	for(i = 0; i < num_users; i++) {
		user_names[i] = calloc(255,sizeof(char*));
		user_pass_hashes[i] =calloc(255,sizeof(char*));
		user_passwords[i] = calloc(255,sizeof(char*));
	}
	char* line = NULL;
	size_t len = 0;
	ssize_t nchar;
	char* username;

	int pass_num = 0;
	while ((nchar = getline(&line, &len, shadow_fp)) != -1)
	{
		//printf("password line: %s\n", line);

		char* pass_hash = strchr(line, ':') + 1;
		printf("hash %s\n", pass_hash);
		//pass_hash[strlen(pass_hash)-1] = 0;
		int colon_index = strchr(line, ':') - line;

		strcpy(user_names[pass_num], line);
		strcpy(user_pass_hashes[pass_num]+colon_index, line);

		//int id_salt_len = strrchr(line, '$') - pass_hash;
		// char* "$5$ab$" =  malloc("$5$ab$"_len * sizeof(char) + 1);//for some reason only this one needs a null terminating char
		// memset("$5$ab$", 0, "$5$ab$"_len+1);//zero out the array
		// strncpy("$5$ab$", pass_hash, "$5$ab$"_len);
		// "$5$ab$" = "$5$ab$";
		printf("\nusername: %s\n$id$salt: %s\npassword hash: %s\n", username, "$5$ab$", pass_hash);
	}

	int word_num = 0;
	char* word = NULL;
	len = 0;
	rewind(words_fp);
	while ((nchar = getline(&word, &len, words_fp)) != -1)
	{
		char* found = NULL;
		word_num++;
		//skip checking the word testpassword if it's not on the first password
		if(pass_num != 0 && word_num == 1 || strlen(word) < 4)
			continue;

		word[strlen(word)-1] = 0;
		printf("checking %s\n", word);
		int ffix;
		for(ffix = -1; ffix < 10000 && !found; ffix++)//10000000000 = max
		{

			int ffixed_len = (int)(ffix ? log10(abs(ffix)) + 1 : 1) + strlen(word);
			char prefixed[ffixed_len];
			char suffixed[ffixed_len];
			memset(prefixed, 0, ffixed_len);
			memset(suffixed, 0, ffixed_len);

			if(ffix == -1)//check the word by itself
			{
				sprintf(prefixed, "%s", word);
				sprintf(suffixed, "%s", word);
			}else{
				sprintf(prefixed, "%d%s", ffix, word);
				sprintf(suffixed, "%s%d", word, ffix);
			}

			char* pre_result = crypt(prefixed, "$5$ab$");
			char* suf_result = crypt(suffixed, "$5$ab$");

			int u;
			for(u = 0; u < num_users; u++)
			{
				printf("checking against %s", user_pass_hashes[u]);
				char* pass_hash = malloc(strlen(user_pass_hashes[u]) * sizeof(char));
				strcpy(pass_hash, user_pass_hashes[u]);
				if(pre_result)
				{
					//printf("prefix: %s hashed to %s\n", prefixed, pre_result);
					if(strcmp(pre_result, pass_hash) == 0)
					{
						found = malloc(strlen(prefixed));
						strcpy(found, prefixed);
					}
				}else{
					printf("prefix crypt error%s, args: str: %s salt: %s\n", strerror(errno), prefixed, "$5$ab$");
					exit(EXIT_FAILURE);
				}

				if(suf_result)
				{
					//printf("suffix: %s hashed to %s\n", suffix, suf_result);
					if(strcmp(suf_result, pass_hash) == 0)
					{
						found = malloc(strlen(suffixed));
						strcpy(found, suffixed);
					}
				}else{
					//printf("prefix: %s hashed to %s\n", suffixed, suf_result);
					printf("suffix crypt error%s, args: str: %s salt: %s\n", strerror(errno), suffixed, "$5$ab$");
					exit(EXIT_FAILURE);
				}
			}
		}
		if(found)
			printf("%s's password is %s\n", username, found);
		if(username)
			free(username);
		if(found)
			free(found);
		//free(id_salt);
	}






	// fclose(words_fp);
	// if (word)
	// 	free(word);
	//
	// fclose(fp);
	// if (line)
	// 	free(line);
	exit(EXIT_SUCCESS);

	MPI_Finalize();
	return 0;
}
