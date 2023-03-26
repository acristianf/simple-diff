#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define MEGABYTE(n) (n * 1024)

#define TRACE_CACHE 0

#define MAX_LINE_LENGTH 100
#define MAX_NUMBER_OF_LINES MEGABYTE(4)
#define DIST_TRESHOLD 0

int levIterative(char *s1, char *s2) {
    int m = (int)strlen(s1);
    int n = (int)strlen(s2);
    int **d = (int **)malloc(sizeof(int *)*(m+1));
    for(int i = 0; i <= m; i++) {
	d[i] = (int *)malloc(sizeof(int)*(n+1));
    }
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                d[i][j] = j;
            } else if (j == 0) {
                d[i][j] = i;
            } else if (s1[i-1] == s2[j-1]) {
                d[i][j] = d[i-1][j-1];
            } else {
                d[i][j] = 1 + min(3, d[i][j-1], d[i-1][j], d[i-1][j-1]);
            }
        }
    }
    int dist = d[m][n];
    for(int i = 0; i <= m; i++) 
	free(d[i]);
    free(d);
    return dist;		
}

void writeDiffLines(const char *filename1, const char *filename2) {

    FILE *fp1;
    FILE *fp2;

    fopen_s(&fp1, filename1, "rb");
    fopen_s(&fp2, filename2, "rb");

    if (!fp1 || !fp2) {
	perror("ERROR DURING FILE OPENING");
	return;
    }

    char line[MAX_LINE_LENGTH];

    char *linesf1[MAX_NUMBER_OF_LINES];
    int num_file1_lines = 0;
    while(fgets(line, MAX_LINE_LENGTH, fp1) != NULL) {
	linesf1[num_file1_lines] = _strdup(line);
	num_file1_lines++;
    }

    int num_file2_lines = 0;
    char *linesf2[MAX_NUMBER_OF_LINES];
    while(fgets(line, MAX_LINE_LENGTH, fp2) != NULL) {
	linesf2[num_file2_lines] = _strdup(line);
	num_file2_lines++;
    }

    fclose(fp1);
    fclose(fp2);

    for(int i = 0; i < num_file1_lines; i++) {
	if (i < num_file2_lines) {
	    int dist = levIterative(linesf1[i], linesf2[i]);
	    if (dist > DIST_TRESHOLD) {
		printf("%d: \n\tFILE 1: %s", i+1, linesf1[i]);
		printf("\tFILE 2: %s", linesf2[i]);
	    }
	} else if (i >= num_file2_lines) {
	    printf("%d: (-) %s", i+1, linesf1[i]);
	}
    }

    if(num_file2_lines > num_file1_lines) {
	for(int i = num_file1_lines; i < num_file2_lines; i++) {
	    printf("%d: (+) %s", i, linesf2[i]);
	}
    }
}

int main(int argc, char *argv[]) {

    if (argc <= 1) {
	printf("ERROR: Number of arguments incorrect.");
	return -1;
    } else if (argc == 2) {
	if (strcmp(argv[1], "--help") == 0)
	    printf("usage: lev <FILE 1> <FILE 2>\n");
	return 0;
    } else if (argc == 3) {
	writeDiffLines(argv[1], argv[2]);
	return 0;
    } else {
	return -1;
    }

}
