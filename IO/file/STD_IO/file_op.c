#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    FILE *fp_file = NULL;
    //FILE *fopen(const char *path, const char *mode);
    fp_file = fopen("./file.txt", "r");
    if(fp_file == NULL)
    {
        perror("fopen file.txt");
        exit(EXIT_FAILURE);
    }

    int c = fgetc(fp_file);
    if(c == EOF)
    {
        perror("fgetc file.txt");
        exit(EXIT_FAILURE);
    }

    int ret_fputc = fputc(c, stdout);
    
    printf("ret_fputc = %d\n", ret_fputc);

    fclose(fp_file);

    return 0;
}
