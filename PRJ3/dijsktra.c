#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//structure single point on the map i.e. a city for example
typedef struct Vertex_node
{
    int x; //x coordinate
    int y; // y coordinate
    int lcv;
    int dist; // distance between 2 nodes
    bool visited; // has current node been visited?
    int previous; // previous vertex visited
}Vertex;

typedef struct Adjacency_node
{
    struct Adjacency_node* next;
    int lcv;
    int weight;
}Adjacency_node;

typedef struct Adjacency_list
{
    struct Adjacency_node* head; //points to the head node of the list
}Adjacency_list;


//Global Declaration of functions
int calcDistance(int x, int y, int i, int j);
int Dijkstra(Vertex node[], Adjacency_list *adjacency_list, int source, int destination, int vert_total);
int closest_node(Vertex node[], int n);
void relax(Vertex node[],int curr_node_dist, int edge, int neighbor, int previous);
void trace_path(Vertex node[], int source, int dest);
void reinitialize_node(Vertex node[], int vertex_total);
void call_relax(Vertex node[], Adjacency_list *adjacency_list, int curr);
Adjacency_node* create(int index, int weight);
void Destroy_adjacency_list(Adjacency_list* adjacency_list, int n);
void Destroy_adjacency_node(Adjacency_node *adjacency_node);



//free list
void Destroy_adjacency_list(Adjacency_list* adjacency_list, int n)
{
    int i = 0;
    
    for(i = 0; i < n; i++)
    {
        if(adjacency_list[i].head != NULL)
        {
            Destroy_adjacency_node(adjacency_list[i].head);
        }
    }
    free(adjacency_list);
}

//free node
void Destroy_adjacency_node(Adjacency_node *adjacency_node)
{
    Adjacency_node *ptr = adjacency_node;
    if(ptr != NULL)
    {
        Destroy_adjacency_node(ptr->next);
        free(ptr);
    }
}

//malloc for adjacency node
Adjacency_node* create(int lcv, int weight)
{
    Adjacency_node* node = malloc(sizeof(Adjacency_node));
    node->next  = NULL;
    node->lcv = lcv;
    node->weight = weight;
    return node;
}

void reinitialize_node(Vertex node[], int vertex_total)
{
    int ctr1 = 0;
    for(ctr1 = 0; ctr1 < vertex_total; ctr1++)
    {
        node[ctr1].dist = 100000;
        node[ctr1].previous = -1;
        node[ctr1].lcv = ctr1;
        node[ctr1].visited = false;
    }
}


//calculate distance between two vertices
int calcDistance(int x, int y, int i, int j)
{
    int distance;
    distance = (sqrt((pow((i - x),2) + pow((j - y),2))));
    return distance;
}

void relax(Vertex v[],int curr_node_dist, int edge, int neighbor, int previous)
{
    if(v[neighbor].dist > curr_node_dist + edge)
    {
        v[neighbor].dist = curr_node_dist + edge;
    }
    
    v[neighbor].previous = previous;
}

void call_relax(Vertex node[], Adjacency_list *adjacency_list, int curr)
{
    int index = 0;
    int weight = 0;
    Adjacency_node *list_ptr = adjacency_list[curr].head;
    
    while(list_ptr != NULL)
    {
        index = list_ptr->lcv;
        weight = list_ptr->weight;
        
        if(node[index].visited == false )
        {
            relax(node,node[curr].dist,weight,index,curr);
        }
        list_ptr = list_ptr->next;
    }
}

//determine the closest node while updating
int closest_node(Vertex node[], int n)
{
    int max = 100000;
    int min = 0;
    int i = 0;
    
    while ( i < n)
    {
        if((node[i].visited == false) & (node[i].dist < max))
        {
            max = node[i].dist;
            min = node[i].lcv;
        }
        i++;
    }
    return min;
}

int Dijkstra(Vertex node[], Adjacency_list *adjacency_list, int source, int destination, int vert_total)
{
    int current_node;
    int j = vert_total;
    current_node = source;
    node[source].dist = 0;
    node[current_node].visited = true;
    
    while(current_node != destination)
    {
        call_relax(node,adjacency_list,current_node);
        current_node = closest_node(node,vert_total);
        node[current_node].visited = true;
        j--;
    }
    
    return EXIT_FAILURE;
}

void trace_path(Vertex node[], int source, int dest)
{
    int i = dest;
    int j = 0;
    int path[10000] = {0};
    
    printf("%d\n",node[dest].dist);
    
    path[j] = dest;
    j++;
    
    while(i != source)
    {
        path[j] = node[i].previous;
        i = node[i].previous;
        j++;
    }
    
    j--;
    
    while(j >= 0)
    {
        printf("%d ",path[j]);
        j--;
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    int vertex_total = 0;
    int edge_total = 0;
    int nquery = 0;
    int src = 0;
    int dest = 0;
    int ctr1 = 0;
    int i;
    int j;
    int adj_weight = 0;
    int Dijkstra_res = 0;
    FILE* fp = fopen(argv[1],"r");;
    if (fp == NULL)
    {
        return 0;
    }
    fscanf(fp,"%d %d",&vertex_total,&edge_total);
    
    Vertex vertices[vertex_total];
    Adjacency_list *adjacency_list = malloc(vertex_total * sizeof(Adjacency_list));
    for (i = 0; i < vertex_total; i++)
    {
        adjacency_list[i].head = NULL;
    }
    
    
    for(ctr1 = 0; ctr1 < vertex_total; ctr1++)
    {
        fscanf(fp,"%d %d %d",&i,&vertices[ctr1].x,&vertices[ctr1].y);
        vertices[ctr1].dist = 100000;
        vertices[ctr1].previous = -1;
        vertices[ctr1].lcv = ctr1;
        vertices[ctr1].visited = false;
    }
    
    
    while(fscanf(fp,"%d %d",&i,&j) != EOF)
    {
        adj_weight = calcDistance(vertices[i].x,vertices[i].y,vertices[j].x,vertices[j].y);
        
        Adjacency_node* newNode = create(j,adj_weight);
        newNode->next = adjacency_list[i].head;
        adjacency_list[i].head = newNode;
        
        newNode = create(i,adj_weight);
        newNode->next = adjacency_list[j].head;
        adjacency_list[j].head = newNode;
    }
    
    
    fclose(fp);
    FILE* fptr = fopen(argv[2],"r");
    fscanf(fptr,"%d",&nquery);
    
    while(fscanf(fptr,"%d %d",&src,&dest) != EOF)
    {
        Dijkstra_res = Dijkstra(vertices,adjacency_list,src,dest,vertex_total);
        if(Dijkstra_res == 1)
        {
            trace_path(vertices,src,dest);
        }
        reinitialize_node(vertices,vertex_total);
    }
    
    fclose(fptr);
    Destroy_adjacency_list(adjacency_list,vertex_total);
    
    return 0;
}
