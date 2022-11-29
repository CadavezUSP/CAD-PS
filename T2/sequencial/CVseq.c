#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INT_MAX 214748364

int ** genGraph(FILE *fp, int nodes){
    int ** graph;
    char read;
    for (int i=0;i<nodes;i++){
        graph = (int**)calloc(nodes, sizeof(int*));
        for (int j=0;j<nodes;j++){
            graph[i] = (int*) calloc(nodes, sizeof(int));
            graph[i][j] = INT_MAX;
        }
    }
    read = fgetc(fp);
    while (read != EOF)
    {
        int i = ((int) read) -48;
        read = fgetc(fp);
        read = fgetc(fp);
        int j = ((int) read) -48;
        read = fgetc(fp);
        read = fgetc(fp);
        int weight = ((int) read) -48;
        graph[i][j] = weight;
        while (read <= '\n')
        {
            read = fgetc(fp);
        } 

    }
    return graph;
    
    
}

int main(int argc, char *argv[]){
    FILE *fp;
    char filename[100];
    int **graph, size;
    scanf("%d", &size);
    scanf("%s", filename);
    fp = fopen(filename, "r");
    graph = genGraph(fp, size);
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            printf("%d %d %d",i,j, graph[i][j]);
        }
        printf("\n");
    }
    fclose(fp);

}
