#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define INFINITY 99999
#define MAX_CITIES 100


typedef struct
{
    char name[50];// Name of the city
    int distance;// Distance of the city from a reference point
} City;


typedef struct
{
    int adjacentMatrix[MAX_CITIES][MAX_CITIES];// Adjacency matrix representing connections between cities
    int numCities; // Number of cities in the graph
    City cities[MAX_CITIES]; // Array of City structures representing the cities
} Graph;


void createGraph(Graph* graph)
{
// Initialize the adjacency matrix with initial values (Infinity)
    for(int i=0; i<MAX_CITIES; i++)
    {
        for(int j=0; j<MAX_CITIES; j++)
        {
            //graph is weighed so the initial value will be infinity
            graph->adjacentMatrix[i][j]=INFINITY;
        }

    }
// Set the number of cities to 0 (no cities initially)
    graph->numCities=0;

}

void addCity(Graph* graph,char* cityName)
{
// Create a new city
    City city;
    // Copy the city name to the new city's name member
    strcpy(city.name, cityName);
    // Set the distance of the city to infinity
    city.distance = INFINITY;
    // Add the city to the cities array in the graph
    graph->cities[graph->numCities] = city;
    // Increment the number of cities in the graph
    graph->numCities++;

}


int findCity(Graph graph,char* cityName)
{

// Iterate over the cities in the graph
    for(int i=0; i<graph.numCities; i++)
    {
// Compare the name of each city with the given cityName using strcmp
        if (strcmp(graph.cities[i].name,cityName) == 0)
        {
            // If a match is found, return the index of the city
            return i;
        }

    }
// If no match is found, return -1 to indicate the city was not found
    return -1;

}

void loadFromFile(char* filename, Graph* graph)
{
    // Open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
// Buffer to hold each line read from the file
    char line[100];
    // Read each line from the file
    while (fgets(line, sizeof(line), file))
    {
        char city1[50];
        char city2[50];
        int distance;
        // Extract city1, city2, and distance from the line
        sscanf(line, "%s %s %d", city1, city2, &distance);

        int index1 = findCity(*graph, city1);
        int index2 = findCity(*graph, city2);
// If city1 is not found in the graph, add it
        if (index1 == -1)
        {
            addCity(graph, city1);
            index1 = findCity(*graph, city1);
        }
// If city2 is not found in the graph, add it
        if (index2 == -1)
        {
            addCity(graph, city2);
            index2 = findCity(*graph, city2);
        }
        // Set the distance between city1 and city2 in the adjacency matrix
        graph->adjacentMatrix[index1][index2] = distance;
    }

    fclose(file);
    printf("Cities loaded successfully!\n\n");
}

int minDistance(int distance[],int visited[],int numCities)
{

    int min = INFINITY, minIndex;
// Find the minimum distance vertex that has not been visited
    for (int v = 0; v < numCities; v++)
        // Check if the vertex has not been visited and its distance is smaller than the current minimum
        if (visited[v] !=1 && distance[v] < min)
        {
            min = distance[v];// Update the minimum distance
            minIndex = v;
        } // Update the index of the minimum distance

    return minIndex; // Return the index of the vertex with the minimum distance
}

void dijkstra (Graph* graph,int sourceIndex,int destinationIndex,int* distance, int* parent)
{


    int visited[MAX_CITIES];// Array to track visited cities

// Initialize distance, visited, and parent arrays
    for (int i = 0; i < graph->numCities; i++)
    {
        distance[i] = INFINITY;
        visited[i] = 0;
        parent[i] = -1;
    }

    distance[sourceIndex] = 0;
    // Run Dijkstra's algorithm

    for (int j = 0; j < graph->numCities - 1; j++)
    {
        // Find the vertex with the minimum distance value
        int u = minDistance(distance, visited, graph->numCities);
        visited[u] = 1;
// Update the distance values of adjacent vertices
        for (int v = 0; v < graph->numCities; v++)
        {
            // Check if vertex v is not visited, there is an edge from u to v,
            // and the total weight of the path from source to v through u is smaller than the current distance value of v
            if (!visited[v] && graph->adjacentMatrix[u][v]
                    && distance[u] != INFINITY
                    && distance[u] + graph->adjacentMatrix[u][v] < distance[v])
            {
                // Update the distance and parent of vertex v
                distance[v] = distance[u] + graph->adjacentMatrix[u][v];
                parent[v]=u;
            }
        }
    }


    int path[MAX_CITIES];
    int length = 0;
    int current = destinationIndex;
    // Find the shortest path by backtracking from the destination to the source
    while (current != -1)
    {
        path[length++] = current;// Add the current city to the path
        current = parent[current];// Move to the parent city
    }
// Print the shortest path
    printf("Shortest path from %s to %s using Dijkstra's algorithm:\n\n", graph->cities[sourceIndex].name, graph->cities[destinationIndex].name);
    for (int i = length - 1; i >= 0; i--)
    {
        printf("%s", graph->cities[path[i]].name);
        if(i>0)
        {
            printf("->");
        }
    }
    printf("\n");

// Print the distances between cities on the path
    for (int i = length - 1; i > 0; i--)
    {
        printf("\nFrom %s To %s : %d km\n", graph->cities[path[i]].name, graph->cities[path[i - 1]].name, graph->adjacentMatrix[path[i]][path[i - 1]]);
    }
    // Print total distance
    if(distance[destinationIndex]!=INFINITY)
    {
        printf("\nTotal distance: %d km\n\n", distance[destinationIndex]);
    }

    else
    {
        printf("Oops! Looks like you can't reach %s from %s\n\n", graph->cities[destinationIndex].name,graph->cities[sourceIndex].name);
    }

}

void BFS(Graph* graph,int sourceIndex,int destinationIndex, int* parent)
{

    int queueArr[MAX_CITIES]; // Array to store the indices of visited cities in the queue
    int visited[MAX_CITIES];// Array to track visited cities

    int front = 0, rear = 0;// Front and rear indices for the queue

    int source=sourceIndex;// Store the source index for reference
// Initialize visited and parent arrays
    for (int i = 0; i < graph->numCities; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
    }

    visited[sourceIndex] = 1;// Mark the source city as visited
    queueArr[rear++] = sourceIndex;// Enqueue the source city
// Perform Breadth-First Search
    while(front!=rear)
    {

        sourceIndex=queueArr[front++];// Dequeue a city from the front of the queue
// Check all adjacent cities of the dequeued city
        for (int v = 0; v < graph->numCities; v++)
        {
            // Check if vertex v is adjacent to the current sourceIndex and not visited
            if (graph->adjacentMatrix[sourceIndex][v]!=INFINITY && !visited[v])
            {
                visited[v] = 1;// Mark the city as visited
                parent[v] = sourceIndex; // Set the parent of the adjacent city
                queueArr[rear++] = v; // Enqueue the adjacent city
            }
        }
    }

    int path[MAX_CITIES];
    int length = 0;
    int current = destinationIndex;
    // Find the shortest path by backtracking from the destination to the source
    while (current != -1)
    {
        path[length++] = current;// Add the current city to the path
        current = parent[current];// Move to the parent city
    }
    // Print the shortest path and distances between cities on the path

    printf("*************************************************************************\n\n");
    printf("Shortest path from %s to %s using BFS algorithm:\n\n", graph->cities[source].name, graph->cities[destinationIndex].name);
    for (int i = length - 1; i >= 0; i--)
    {
        printf("%s", graph->cities[path[i]].name);

        if (i > 0)
        {
            printf("->");

        }
    }
    printf("\n\n");


}

void saveToFile(Graph* graph, char* filename, int sourceIndex, int destinationIndex, int* parent, int* distance, int isDijkstra)
{
    FILE* file = fopen(filename, "a"); // Open the file in append mode for writing
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    int path[MAX_CITIES]; // Array to store the indices of cities on the shortest path
    int length = 0; // Variable to track the length of the path
    int current = destinationIndex; // Start from the destination city

    // Find the shortest path by backtracking from the destination to the source
    while (current != -1)
    {
        path[length++] = current; // Add the current city to the path
        current = parent[current]; // Move to the parent city
    }

    // print based on the algorithm used
    if (isDijkstra)
    {
        // Print the shortest path
        fprintf(file,"Shortest path from %s to %s using Dijkstra's algorithm:\n\n", graph->cities[sourceIndex].name, graph->cities[destinationIndex].name);
        for (int i = length - 1; i >= 0; i--)
        {
            fprintf(file,"%s", graph->cities[path[i]].name);
            if(i>0)
            {
                fprintf(file,"->");
            }
        }
        fprintf(file,"\n");

// Print the distances between cities on the path
        for (int i = length - 1; i > 0; i--)
        {
            fprintf(file,"\nFrom %s To %s : %d km\n", graph->cities[path[i]].name, graph->cities[path[i - 1]].name, graph->adjacentMatrix[path[i]][path[i - 1]]);
        }
        // Print total distance
        if(distance[destinationIndex]!=INFINITY)
        {
            fprintf(file,"\nTotal distance: %d km\n\n", distance[destinationIndex]);
        }

        else
        {
            fprintf(file,"Oops! Looks like you can't reach %s from %s\n\n", graph->cities[destinationIndex].name,graph->cities[sourceIndex].name);
        }
    }
    else
    {
        fprintf(file,"*************************************************************************\n\n");
        fprintf(file,"Shortest path from %s to %s using BFS algorithm:\n\n", graph->cities[sourceIndex].name, graph->cities[destinationIndex].name);
        for (int i = length - 1; i >= 0; i--)
        {
            fprintf(file,"%s", graph->cities[path[i]].name);

            if (i > 0)
            {
                fprintf(file,"->");

            }
        }
        fprintf(file,"\n\n");

        // Print the total distances

    }
    fclose(file); // Close the file
}

void flush(char* in)
{
    // Open the file for writing
    FILE* file = fopen(in, "w");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
// Flush the file stream
    fflush(file);
    fclose(file);
}
int main()
{
    Graph graph;
    createGraph(&graph);  // Create the graph
    int parentDij[MAX_CITIES],parentBfs[MAX_CITIES],distance[MAX_CITIES];// Arrays for Dijkstra's and BFS algorithms
    char sourceCity[50],destinationCity[50];
    int choice,sourceIndex,destinationIndex,flag=1;
    char *inputFile="cities.txt";
    char *outputFile="shortest_distance.txt";
    flush(outputFile);// Flush the output file
    printf("Welcome to the shortest path finder!\n\n");
    while(flag)
    {
//print menu to user
        printf("Enter your choice\n");
        printf("1. Load cities\n");
        printf("2. Enter source\n");
        printf("3. Enter destination\n");
        printf("4. Exit\n\n");

        scanf("%d", &choice);
        printf("\n");
        switch(choice)
        {


        case 1:
            // Load data from the input file into the graph
            loadFromFile(inputFile,&graph);

            break;

        case 2:

            printf("Enter the source city:\n");
            scanf("%s",sourceCity);
            printf("\n");
            //find source Index
            sourceIndex=findCity(graph,sourceCity);
            if (sourceIndex == -1)
            {
                printf("Source city not found!\n\n");
            }

            break;

        case 3:

            printf("Enter the destination city:\n");
            scanf("%s",destinationCity);
            printf("\n");
            //find destination Index
            destinationIndex=findCity(graph,destinationCity);
            if (destinationIndex == -1)
            {
                printf("destination city not found!\n\n");
            }
            else
            {
                // Perform Dijkstra's algorithm and BFS to find the shortest paths
                dijkstra(&graph, sourceIndex, destinationIndex,distance,parentDij);
                BFS(&graph, sourceIndex, destinationIndex,parentBfs);
            }

            break;
        case 4:
            // Save the shortest path information to the output file
            saveToFile(&graph,outputFile,sourceIndex,destinationIndex,parentDij,distance,1);
            saveToFile(&graph,outputFile,sourceIndex,destinationIndex,parentBfs,distance,0);
            // Set flag to exit the program
            flag=0;
            printf("Information printed to file.\n");
            printf("Exiting the program.\n");

            break;
        default:
            printf("Oops! That doesn't seem like a valid choice. Please try again.\n\n");
            break;

        }
    }

    return 0;
}
