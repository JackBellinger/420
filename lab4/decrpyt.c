#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h> // rand
#include <stdio.h> // puts, printf, etc
#include <time.h> // time
#include <string.h>
#include <math.h>
#include <crypt.h>
#include <errno.h>

//#include <mpi.h> // mpi stuff

int num_words = 235886;

// void check_s(char* word, char* id_salt, char* pass_hash)
// {
// 	int ffix;
// 	for(ffix = 0; ffix < 10000000000; ffix++)
// 	{
//
// 	}
// }
void print_char_vals(char* x)
{
	printf("printing\n");
	int i;
	for(i = 0; i < strlen(x); i++)
		printf("%d ", x[i]);
	printf("\n");
}

int main(int argc, char** argv){
	// MPI_Init(NULL, NULL);
	//
	// MPI_Comm world = MPI_COMM_WORLD;
	// int rank, world_size;
	// MPI_Comm_rank(&rank, world);
	// MPI_Comm_size(&world_size, world);
	FILE* fp;
	FILE* words_fp;

	fp = fopen("shadow", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	words_fp = fopen("words", "r");
	if (words_fp == NULL)
		exit(EXIT_FAILURE);

	char* line = NULL;
	size_t len = 0;
	ssize_t nchar;
	char* username;
	int pass_num = 0;
	while ((nchar = getline(&line, &len, fp)) != -1) {
		//printf("password line: %s\n", line);

		char* pass_hash = strchr(line, ':') + 1;
		pass_hash[strlen(pass_hash)-1] = 0;
		int colon_index = strchr(line, ':') - line;

		char* username = malloc(colon_index * sizeof(char));
		strncpy(username, line, colon_index);

		int id_salt_len = strrchr(line, '$') - pass_hash;
		char* id_salt =  malloc(id_salt_len * sizeof(char) + 1);//for some reason only this one needs a null terminating char
		memset(id_salt, 0, id_salt_len+1);//zero out the array
		strncpy(id_salt, pass_hash, id_salt_len);

		printf("\nusername: %s\n$id$salt: %s\npassword hash: %s\n", username, id_salt, pass_hash);

		char* found = NULL;
		int word_num = 0;
		char* word = NULL;
		len = 0;
		rewind(words_fp);
		while ((nchar = getline(&word, &len, words_fp)) != -1 && !found)
		{
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

				char* pre_result = crypt(prefixed, id_salt);
				char* suf_result = crypt(suffixed, id_salt);

				if(pre_result)
				{
					//printf("prefix: %s hashed to %s\n", prefixed, pre_result);
					if(strcmp(pre_result, pass_hash) == 0)
					{
						found = malloc(strlen(prefixed));
						strcpy(found, prefixed);
					}
				}else{
					//printf("prefix: %s hashed to %s\n", suffixed, suf_result);
					printf("prefix crypt error%s, args: str: %s salt: %s\n", strerror(errno), prefixed, id_salt);
					exit(EXIT_FAILURE);
				}

				if(suf_result)
				{
					if(strcmp(suf_result, pass_hash) == 0)
					{
						found = malloc(strlen(suffixed));
						strcpy(found, suffixed);
					}
				}else{
					//printf("prefix: %s hashed to %s\n", suffixed, suf_result);
					printf("suffix crypt error%s, args: str: %s salt: %s\n", strerror(errno), suffixed, id_salt);
					exit(EXIT_FAILURE);
				}
			}

			free(id_salt);
		}

		if(found)
			printf("%s's password is %s\n", username, found);
		if(username)
			free(username);
		if(found)
			free(found);
		pass_num++;
	}



	// fclose(words_fp);
	// if (word)
	// 	free(word);
	//
	// fclose(fp);
	// if (line)
	// 	free(line);
	exit(EXIT_SUCCESS);

	// MPI_Finalize();
	// return 0;
}
