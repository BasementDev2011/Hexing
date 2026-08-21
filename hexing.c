#define HEXING_VERSION "V1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define HEXING_USER_INPUT_BUFFER_SIZE 64
#define HEXING_BASE_HEX 16
#define HEXING_FILE_NAME_SIZE_MAX 64

unsigned char hex_buffer;
char user_input[HEXING_USER_INPUT_BUFFER_SIZE];
char filename[HEXING_FILE_NAME_SIZE_MAX];

int main(void) {

    printf("Type in File Name (Max %d chars): ",HEXING_FILE_NAME_SIZE_MAX);
    if (scanf("%63s",&filename) != 1) {
        return 1;
    }

    int character = getchar();
    if (character != '\n' && character != EOF) {
        printf("Filename too long\n");
        return 1;
    }

    FILE *file = fopen(filename,"wb");

    if (file == NULL) {
        printf("Failure to open hex.dat\n");
        return 1;
    }

    printf("Opening %s\n",filename);
    printf("You can start typing hex values now\n");

    while (1) {

        if (scanf("%63s",user_input) != 1) {
            return 1;
        }
        int character = getchar();
        if (character != '\n' && character != EOF) {
            printf("\033[2J\033[H");
            printf("Input is too long\n");
            while ((character = getchar()) != '\n' && character != EOF) {
            }
            continue;
        }

        if (strcmp(user_input,"done") == 0) {
            fclose(file);
            printf("\033[2J\033[H");
            printf("Closing file %s\n",filename);
            fflush(stdout);
            return 0;
        }

        errno = 0;
        char *endptr;
        unsigned long value = (unsigned char)strtoul(user_input,&endptr,HEXING_BASE_HEX);

        if (errno == ERANGE || *endptr != '\0' || value > UCHAR_MAX) {
            printf("\033[2J\033[H");
            printf("Invalid hex value\n");
            continue;
        }
        printf("\033[2J\033[H");
        printf("Writing hex value: %s\n",user_input);
        hex_buffer = (unsigned char)value;
        fwrite(&hex_buffer,sizeof(hex_buffer),1,file);
    }

    fclose(file);

    return 0;
}
