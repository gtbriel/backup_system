#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define BSIZE (2*1024)

/*void makeCopy() {
	fd_origem = open(<dir_origem>, O_RDONLY);
	fd_destino = open(<dir_destino>, O_CREAT | O_TRUNC);
	
	char bloco[BSIZE];
	
	while(1) {
		n = read(fd_origem, bloco, BSIZE);
		if(n > 0) {
			write(fd_destino, bloco, n);
		}else {
			break;
		}
	}
	
}*/

int main(int argc, char **argv) {
	
	if(argc != 3) {
		printf("%s <dir_origem> <dir_destino> \n", argv[0]);
		return 0;
	}
	while(1) {
		struct dirent **nameList_origem;
		struct dirent **nameList_destino;
		int n_origem, n_destino, i = 0, j = 0;
		
		n_origem = scandir(argv[1], &nameList_origem, NULL, alphasort);
		n_destino = scandir(argv[2], &nameList_destino, NULL, alphasort);
		
		if(n_origem == -1 || n_destino == -1) {
			perror("scandir");
			exit(EXIT_FAILURE);
		}
		
		if(n_destino == 2) {
			printf("--INICIANDO BACKUP --\n");		
			for(i = 2;i < n_origem;i++) {			
				printf("Fazendo backup de: %s\n",nameList_origem[i]->d_name);
			}		
			sleep(3);
			return 0;
		}
		
		struct stat buffer_origem[n_origem];
		struct stat buffer_destino[n_destino];
		
		for(i=2; i <n_origem;i++) {
			stat(nameList_origem[i]->d_name, &buffer_origem[i]);
		}
		for(i=2;i < n_destino;i++) {
			stat(nameList_destino[i]->d_name, &buffer_destino[i]);
		}
		
		for(i=2;i < n_origem;i++) {
			int foiEncontrado = 0;				
			for(j=2;j < n_destino;j++) {
				if(strcmp(nameList_origem[i]->d_name, nameList_destino[j]->d_name) == 0) {
					foiEncontrado = 1;
					if(buffer_origem[i].st_mtime > buffer_destino[j].st_mtime) {
						printf("(Arquivo desatualizado) Fazendo backup de: %s\n", nameList_origem[i]->d_name);
						break;
					}
				}	
			}
			
			if(foiEncontrado == 0) {
				printf("(Nao foi encontrado) Fazendo backup de: %s\n", nameList_origem[i]->d_name);
			}
			
		}
		
		for(i=0;i < n_origem;i++) {
			free(nameList_origem[i]);
		}
		for(i=0;i < n_destino;i++) {
			free(nameList_destino[i]);
		}
		
		free(nameList_origem);
		free(nameList_destino);
		sleep(3);
	}
		
	return 0;
}
