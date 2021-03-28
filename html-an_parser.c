#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void rmcom(FILE *fip, FILE *fop) {
  int stage = 0;
  bool comment = false;
  char c;
  while((c = getc(fip)) != EOF) {
    if(comment == false) {
      fputc(c,fop); /* Change to printf for debugging */
      if(c == '-' && stage == 3) {
	fseek(fop,-4,SEEK_CUR);
	comment = true;
	stage = 0;
      }
      if(c == '-' && stage == 2)
        stage = 3;
      if(c == '!' && stage == 1)
        stage = 2;
      if(c == '<')
        stage = 1;
    }
    if(comment == true) {
      if(stage == 2 && c == '>') {
	comment = false;
	stage = 0;
      }
      if(stage == 1 && c == '-')
	  stage = 2;
      if(stage == 0 && c == '-')
	  stage = 1;
    }
  }
}

char *fname(char *fip) {
  int u = 0;
  char *fname = malloc(sizeof(fip));
  strcpy(fname, fip);
  while(fip[u] != '\0')
    u++;
  fname[u-2] = '\0';    
  return fname;
}

int main(int argc, char *argv[]) {
  if(argc == 1) {
    printf("\033[1;31mfatal: \033[0mno input files specified\n");
    return(1);
  }
  for(int i = 1; i < argc; i++) {
    FILE *fip;
    FILE *fop;
    if(access(argv[i], R_OK) == 0) {
        fip = fopen(argv[i],"r");
        char *foutname = fname(argv[i]);
        fop = fopen(foutname, "w");
        rmcom(fip,fop);
        printf("%s\n",foutname);
    }
    else {
         printf("\033[1;31mfatal: \033[0mPlease provide a valid input file name.\n");
         return(1);
    }
  }
}
