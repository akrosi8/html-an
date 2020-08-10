#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

void rmcom(FILE *fip, FILE *fop) { /* Remove comments from fip and write modified file to fop */
  int stage = 0;
  bool comment = false;
  char c;
  while((c = getc(fip)) != EOF) { /* Read to end of file */
    if(comment == false) {
      if(c == '-' && stage == 3) { /* This is bad. I'll fix it with a rewrite. */
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
      fputc(c,fop); /* Writes current character to output file if not within  a comment. */
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

char *fname(char *fip) { /* Redact last two characters from filename */
  int u = 0;
  char *fname = malloc(sizeof(fip));
  strcpy(fname, fip);
  while(fip[u] != '\0')
    u++;
  fname[u-2] = '\0';
  return fname;
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("\033[1;31m");
    printf("no input files\n");
    exit(1);
  }
  for(int i = 1; i < argc; i++) { /*Scan all given files */
    FILE *fip;
    FILE *fop;
    fip = fopen(argv[i],"r");
    char *foutname = fname(argv[i]);
    fop = fopen(foutname, "w");
    rmcom(fip,fop);
    printf("%s\n",foutname);
  }
}
