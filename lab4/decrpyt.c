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
	while ((nchar = getline(&line, &len, fp)) != -1) {
		strtok(line, "\n");
		printf("password line: %s\n", line);

		char* colon = strchr(line, ':');
		int colon_index = strchr(line, ':') - line;

		char* username = malloc(colon_index * sizeof(char));
		strncpy(username, line, colon_index);

		char* pass_hash = strrchr(line, '$') + 1;
		int id_salt_len = pass_hash - colon;
		char* id_salt =  malloc(id_salt_len * sizeof(char));
		strncpy(id_salt, colon+1, id_salt_len-1);
		printf("salt %s\n", id_salt);
		print_char_vals(id_salt);
		//printf("username: %s\n$id$salt: %s\npassword hash: %s\n", username, id_salt, pass_hash);

		char* found = NULL;
		int word_num = 0;
		char* word = NULL;
		len = 0;
		rewind(words_fp);
		while ((nchar = getline(&word, &len, words_fp)) != -1 && !found)
		{
			strtok(word, "\n");
			word_num++;
			printf("checking %s\n", word);
			int ffix;
			for(ffix = -1; ffix < 10000 & !found; ffix++)//10000000000 = max
			{
				int ffixed_len = floor(log10(abs(ffix)))+1 + strlen(word);
				char prefixed[ffixed_len];
				char suffixed[ffixed_len];
				if(ffix == -1)//check the word by itself
				{
					sprintf(prefixed, "%s", word);
					sprintf(suffixed, "%s", word);
				}else{
					sprintf(prefixed, "%d%s", ffix, word);
					sprintf(suffixed, "%s%d", word, ffix);
				}
				printf("pre result %s\n", prefixed);
				printf("suf result %s\n", suffixed);
				print_char_vals(prefixed);
				char* pre_result = crypt(prefixed, id_salt);
				char* suf_result = crypt(suffixed, id_salt);


				if(pre_result)
				{
					if(strcmp(pre_result, pass_hash) == 0)
						found = pre_result;
				}else
				{
					printf("prefix crypt error\n");
					exit(EXIT_FAILURE);
				}
				if(suf_result)
				{
					if(strcmp(suf_result, pass_hash) == 0)
						found = suf_result;
				}else
					printf("suffix crypt error\n");
			}
		}

		if(found)
			printf("%s's password is %s", username, found);

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
