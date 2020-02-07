#include <stdio.h>
#include <stdlib.h>

#define N 27
int numPassengers = 3;
int numCars = 6;
struct Graph {
	struct Node* head[N];
};

struct Node {
	int dest, weight1,weight2,weight3;
	int passenger;
	struct Node* next;
};
struct Edge {
	int src, dest, weight1,weight2,weight3;

};

struct Graph* createGraph(struct Edge edges[], int n)
{
	unsigned i;

	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

	for (i = 0; i < N; i++)
		graph->head[i] = NULL;

	for (i = 0; i < n; i++)
	{
		int src = edges[i].src;
		int dest = edges[i].dest;
		int weight1 = edges[i].weight1;
		int weight2 = edges[i].weight2;
		int weight3 = edges[i].weight3;

		
		struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
		newNode->dest = dest;
		newNode->weight1 = weight1;
		newNode->weight2 = weight2;
		newNode->weight3 = weight3;

		newNode->next = graph->head[src];

		graph->head[src] = newNode;
	}

	return graph;
}

void printGraph(struct Graph* graph)
{
	int i;
	for (i = 0; i < N; i++)
	{
		struct Node* ptr = graph->head[i];
		while (ptr != NULL)
		{
			printf("%d -> %d (%d)(%d)(%d)\t", i, ptr->dest, ptr->weight1,ptr->weight2,ptr->weight3);
			ptr = ptr->next;
		}

		printf("\n");
	}
}


int totalDist(struct Graph* graph,int path[],int size)
{
	int dist = 0;
	int i;
	
	for (int i = 0; i <= size - 2; i++)
	{
		struct Node* ptr = graph->head[path[i]];
		while (ptr != NULL && ptr->dest != path[i + 1])
		{ 
			ptr = ptr->next;
		}
		if(ptr!=NULL)
			dist += ptr->weight1;
	}
		return dist;
		
}
int totalRc(struct Graph* graph, int path[], int size)
{
	int dist = 0;
	int i;

	for (int i = 0; i <= size - 2; i++)
	{
		struct Node* ptr = graph->head[path[i]];
		while (ptr != NULL && ptr->dest != path[i + 1])
		{
			ptr = ptr->next;
		}
		if (ptr != NULL)
			dist += ptr->weight2;
	}
	return dist;

}
int totalTraffic(struct Graph* graph, int path[], int size)
{
	int dist = 0;
	int i;

	for (int i = 0; i <= size - 2; i++)
	{
		struct Node* ptr = graph->head[path[i]];
		while (ptr != NULL && ptr->dest != path[i + 1])
		{
			ptr = ptr->next;
		}
		if (ptr != NULL)
			dist += ptr->weight3;
	}
	return dist;

}

int compare(float d1, float r1, float t1, float d2, float r2, float t2, float d2r,float d2t,float r2t) //AHP
{
	float pathSelection[3][3] =
	{ {1,d2r,d2t}, //shortest path
	{1/d2r,1,r2t},
	{1/d2t,1/r2t,1} };
	float dist[2][2] = { {1,d2/d1},{d1 / d2,1 } }; //shorter dist
	float rc[2][2] = { {1,r1 / r2},{r2 / r1,1 } }; //better rd condition
	float traffic[2][2] = { {1,(float)t2/t1},{(float)t1/t2,1 } };//lesser traffic
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//printf("matrix ==  %f", dist[i][j]);
		}
	}
	float colSum1[2];
	float colSum2[2];
	float colSum3[2];
	float colSumPath[3];
	float priorityVec1[2];
	float priorityVec2[2];
	float priorityVec3[2];

	float priorityVecPath[3];
	float sum1 = 0;
	float sum2 = 0;
	float sum3 = 0;
	float sumPath = 0;
	for (int j = 0; j < 2; j++)
	{
		colSum1[j] = 0;
		colSum2[j] = 0;
		colSum3[j] = 0;

		priorityVec1[j] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		colSumPath[i] = 0;
		priorityVecPath[i] = 0;
	}
	for (int i = 0; i < 2; i++) // i here is looooooop for column
	{
		for (int j = 0; j < 2; j++)
		{
			colSum1[i] += dist[j][i];
		}
		for (int j = 0; j < 2; j++)
		{
			dist[j][i] /= colSum1[i];
		}
	}
	for (int i = 0; i < 2; i++) // i here is looooooop for column
	{
		for (int j = 0; j < 2; j++)
		{
			colSum2[i] += rc[j][i];
		}
		for (int j = 0; j < 2; j++)
		{
			rc[j][i] /= colSum2[i];
		}
	}
	for (int i = 0; i < 2; i++) // i here is looooooop for column
	{
		for (int j = 0; j < 2; j++)
		{
			colSum3[i] += traffic[j][i];
		}
		for (int j = 0; j < 2; j++)
		{
			traffic[j][i] /= colSum3[i];
		}
	}
	for (int i = 0; i < 3; i++) // i here is looooooop for column
	{
		for (int j = 0; j < 3; j++)
		{
			colSumPath[i] += pathSelection[j][i];
		}
		for (int j = 0; j < 3; j++)
		{
			pathSelection[j][i] /= colSumPath[i];
		}
	}
	//for dist
	for (int j = 0; j < 2; j++)
	{
		priorityVec1[j] = 0;
	}


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			priorityVec1[i] += dist[i][j];

		}
	}
	for (int j = 0; j < 2; j++)
	{
		priorityVec1[j] /= 2;
		sum1 += priorityVec1[j]; //sum is always 1
	}
	//printf("sum of priority vec for dist matrix %f \n", sum1);
	//for rc
	for (int j = 0; j < 2; j++)
	{
		priorityVec2[j] = 0;
	}


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			priorityVec2[i] += rc[i][j];

		}
	}
	for (int j = 0; j < 2; j++)
	{
		priorityVec2[j] /= 2;
		sum2 += priorityVec2[j]; //sum is always 1
	}
	//printf("sum of priority vec for rc matrix %f \n", sum2);
	//for traffic
	for (int j = 0; j < 2; j++)
	{
		priorityVec3[j] = 0;
	}


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			priorityVec3[i] += traffic[i][j];

		}
	}
	for (int j = 0; j < 2; j++)
	{
		priorityVec3[j] /= 2;
		sum3 += priorityVec3[j]; //sum is always 1
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			priorityVecPath[i] += pathSelection[i][j];

		}
	}


	
	for (int j = 0; j < 3; j++)
	{
		priorityVecPath[j] /= 3;  //avg
		sumPath += priorityVecPath[j]; //sum is always 1
	}
	float num1 = priorityVec1[0] * priorityVecPath[0] + priorityVec2[0] * priorityVecPath[1] + priorityVec3[0] * priorityVecPath[2];
	float num2 = priorityVec1[1] * priorityVecPath[0] + priorityVec2[1] * priorityVecPath[1] + priorityVec3[1] * priorityVecPath[2];
	//printf("\n\n num1 :: %f ", num1);
	//printf("\n\n num1 :: %f ", num2);
	float compo[2] = {num1,num2};
	if (num1 >= num2)
	{
		return 1;
	}
	else
	{
		return 2;
	}
	
}

int main(void)
{
	struct Edge edges[] =
	{
		{1,2,1,4,6},{1,3,9,8,5},{1,4,1,1,2},{2,5,5,8,1},{3,6,5,2,1},{3,7,2,3,4},{4,8,1,5,5},{5,9,5,8,9},{5,10,1,2,3},{6,11,3,8,6},{7,11,4,5,9},{8,12,4,5,1},{9,13,2,7,8},{13,16,3,9,7},{10,16,3,1,2},{11,14,7,8,1},{12,15,4,5,6},{14,17,5,5,6},{17,20,3,2,1},{17,21,1,1,2},{17,22,1,1,5},{15,18,1,6,8},{16,19,2,2,1},{20,24,1,2,5},{21,24,6,2,1},{22,24,3,4,1},{18,25,1,5,2},{15,23,2,6,8},{19,26,4,1,3},{24,26,3,7,2},{25,26,2,4,8},{23,26,6,7,8}
	};
	//changing 15 23 2 6 8 to 15 23 2 6 1
	//changing 23 26 6 7 8 to 23 26 6 7 1
	//21 24 2 2 1 to 21 34 6 2 1
	// calculate number of edges
	int n = sizeof(edges) / sizeof(edges[0]);
	printf("Number of cars :: 6 \n" );
	printf("Numbers of Passengers in waiting:: 3 \n");
	
	float p2D2R = 1/5, p2D2T = 1/9, p2R2T = 1/9;
	float p3D2R = 1/9, p3D2T = 1/9, p3R2T = 9;
	int newCar1[5];
	int newCar2[6];
	int newCar3[4];
	int newCar4[4];
	int newCar5[3];
	int newCar6[4];
	int capacity[] = { 4,4,4,4,4,4 };//for 6 cars
	int car1[] = { 1,2,5,10,16,19,26 };
	int size1 = sizeof(car1) / sizeof(car1[0]);

	int car2[] = { 1,2,5,9,13,16,19,26 };
	int size2 = sizeof(car2) / sizeof(car2[0]);

	int car3[] = { 1,3,6,11,14,17,21,24,26 };
	int size3 = sizeof(car3) / sizeof(car3[0]);

	int car4[] = { 1,3,6,11,14,17,22,24,26 };
	int size4 = sizeof(car4) / sizeof(car4[0]);

	int car5[] = { 12,15,23,26 };
	int size5= sizeof(car5) / sizeof(car5[0]);
	int car6[] = { 12,15,18,25,26 };
	
	printf("\nEnter nodes car 7 will cover:: ");
	
	
	

	int size6 = sizeof(car6) / sizeof(car6[0]);
	int passenger1 = 5;//car1car2
	int passenger2 = 17;//car3car4
	int passenger3 = 15;//car5car6

	int passenger4 = 17;//c3c4
	int passenger5 = 12;//c5c6

	int passenger6 = 5;//c1c2
	int passenger7 = 5;//c1c2
	int newCar31[5];
	int newCar41[5];
	int point1,point2,point3,point4,point5,point6;
	struct Graph* graph = createGraph(edges, n);
	printf("Position of Passengers :: \n");
	printf("P1 :: %d \t",passenger1);
	printf("P2 :: %d \t", passenger2);
	printf("P3 :: %d \t", passenger3);
	printf("P4 :: %d \t", passenger4);
	printf("P5 :: %d \t", passenger5);
	printf("P6 :: %d \t", passenger6);
	printf("P7 :: %d \t", passenger7);
	printGraph(graph);
	for (int i = 0; i <= 5; i++)
	{
		printf("\n\n Capacity of car%d = %d", i + 1, capacity[i]);
	}
	printf("\nPath of Car1:: \n\n");
	
	for (int i = 0; i < size1; i++)
	{
		printf("%d-> ", car1[i]);
	}
	printf("\n\nPath of Car2:: \n\n");

	for (int i = 0; i < size2; i++)
	{
		printf("%d-> ", car2[i]);
	}
	printf("\n\nPath of Car3:: \n\n");

	for (int i = 0; i < size3; i++)
	{
		printf("%d-> ", car3[i]);
	}
	printf("\n\nPath of Car4:: \n\n");

	for (int i = 0; i < size4; i++)
	{
		printf("%d-> ", car4[i]);
	}
	printf("\n\nPath of Car5:: \n\n");

	for (int i = 0; i < size5; i++)
	{
		printf("%d-> ", car5[i]);
	}
	printf("\n\nPath of Car6:: \n\n");

	for (int i = 0; i < size6; i++)
	{
		printf("%d-> ", car6[i]);
	}
	for (int i = 0; i < size1; i++)
	{
		if (car1[i] == passenger1)
		{
			point1 = i;
		}
	}
	int p = 0, q = 0, r = 0,s=0,t=0,u=0;
	for (int i = point1; i < size1; i++)
	{
		newCar1[p++] = car1[i];
	}
	//car2
	for (int i = 0; i < size2; i++)
	{
		if (car2[i] == passenger1)
		{
			point2 = i;
		}
	}
	for (int i = point2; i < size2; i++)
	{
		newCar2[q++] = car2[i];
	}
	//car3
	for (int i = 0; i < size3; i++)
	{
		if (car3[i] == passenger2)
		{
			point3 = i;
		}
	}
	for (int i = point3; i < size3; i++)
	{
		newCar3[r++] = car3[i];
	}
	//car4
	for (int i = 0; i < size4; i++)
	{
		if (car4[i] == passenger2)
		{
			point4 = i;
		}
	}
	for (int i = point4; i < size4; i++)
	{
		newCar4[s++] = car4[i];
	}
	//car5
	for (int i = 0; i < size5; i++)
	{
		if (car5[i] == passenger3)
		{
			point5 = i;
		}
	}
	for (int i = point5; i < size5; i++)
	{
		newCar5[t++] = car5[i];
	}
	//car6
	for (int i = 0; i < size6; i++)
	{
		if (car6[i] == passenger3)
		{
			point6 = i;
		}
	}
	for (int i = point6; i < size6; i++)
	{
		newCar6[u++] = car4[i];
	}
	
	size1 = sizeof(newCar1) / sizeof(newCar1[0]);
	for (int i =0; i < size1; i++)
	{
		//printf("\n\n%d", newCar1[i]);
	}
	size2 = sizeof(newCar2) / sizeof(newCar2[0]);
	

	size5 = sizeof(newCar5) / sizeof(newCar5[0]);
	size6 = sizeof(newCar6) / sizeof(newCar6[0]);
	int dist1 = totalDist(graph, newCar1, 5);
	int rc1 = totalRc(graph, newCar1, 5);
	int traffic1 = totalTraffic(graph, newCar1, 5);
	//printf("dist =%d", dist1);
	int dist2 = totalDist(graph, newCar2, 6);
	//printf("dist =%d", dist2);
	int rc2 = totalRc(graph, newCar2, 6);
	int traffic2 = totalTraffic(graph, newCar2, 6);
	printf("\nPassenger1 has priority :: distanceToRoadCondition = 9 distanceToTraffic = 9 roadconditionToTraffic = 5");
	int resultPassenger1 = compare(dist1, rc1, traffic1, dist2, rc2, traffic2,9,9,5);
	if (resultPassenger1 == 1)
	{
		printf("\n\n\nPassenger 1 at 5 chooses car 1\n");
		capacity[0]--;
	}
	else
	{
		printf("\n\n\nPassenger 1 at 5 chooses car 2\n");
		capacity[1]--;
	}
	
	int dist3 = totalDist(graph, newCar3, 4);
	int rc3 = totalRc(graph, newCar3, 4);
	int traffic3 = totalTraffic(graph, newCar3, 4);

	int dist4 = totalDist(graph, newCar4, 4);
	int rc4 = totalRc(graph, newCar4, 4);
	int traffic4 = totalTraffic(graph, newCar4, 4);
	printf("\nPassenger2 has priority :: distanceToRoadCondition = 1/5 distanceToTraffic = 1/9 roadconditionToTraffic = 1/9");
	resultPassenger1 = compare(dist3, rc3, traffic3, dist4, rc4, traffic4, (float)1 / 5, (float) 1 / 9, (float)1 / 9);
	if (resultPassenger1 == 1)
	{
		printf("\n\nPassenger 2 at 17 chooses car 3");
		capacity[2]--;
	}
	else
	{
		printf("\nPassenger 2 at 17 chooses car 4");
		capacity[3];
	}
	int dist5 = totalDist(graph, newCar5,3 ),x =6;
	int rc5 = totalRc(graph, newCar5, 3);
	int traffic5 = totalTraffic(graph, newCar5, 3);

	int dist6 = totalDist(graph, newCar6, 4);
	int rc6 = totalRc(graph, newCar6, 4);
	int traffic6 = totalTraffic(graph, newCar6, 4);
	printf("\nPassenger3 has priority :: distanceToRoadCondition = 1/5 distanceToTraffic = 1/9 roadconditionToTraffic = 1/9");
	resultPassenger1 = compare(dist5, rc5, traffic5, dist6, rc6, traffic6, (float)1 /5, (float)1 / 9, (float)1/9);
	if (resultPassenger1 == 1)
	{
		printf("\n\nPassenger 3 at 15 chooses car 5\n\n");
		capacity[4]--;
	}
	else
	{
		printf("\n\nPassenger 3 at 15 chooses car 6");
		capacity[5];
	}

	//NEWCAR3CAR4
	 dist3 = totalDist(graph, newCar3, 4);
	 rc3 = totalRc(graph, newCar3, 4);
	 traffic3 = totalTraffic(graph, newCar3, 4);

	 dist4 = totalDist(graph, newCar4, 4);
	 rc4 = totalRc(graph, newCar4, 4);
	 traffic4 = totalTraffic(graph, newCar4, 4);
	 printf("\n\nPassenger4 has priority :: distanceToRoadCondition = 5 distanceToTraffic = 9 roadconditionToTraffic = 9");
	resultPassenger1 = compare(dist3, rc3, traffic3, dist4, rc4, traffic4, (float) 5, (float) 9, (float)9);
	if (resultPassenger1 == 1)
	{
		printf("\n\nPassenger 4 chooses car 3\n\n");
		capacity[2]--;
	}
	else
	{
		printf("\nPassenger 4 chooses car 4\n\n");
		capacity[3]--;
	}

	dist5 = totalDist(graph, car5, 5), x = 6;
	rc5 = totalRc(graph, car5, 5);
	traffic5 = totalTraffic(graph, car5, 5);
	
	dist6 = totalDist(graph, car6, 6);
	rc6 = totalRc(graph, car6, 6);
	traffic6 = totalTraffic(graph, car6, 6);
	printf("Passenger5 has priority :: distanceToRoadCondition = 5 distanceToTraffic = 9 roadconditionToTraffic = 9");
	resultPassenger1 = compare(dist5, rc5, traffic5, dist6, rc6, traffic6, (float)5, (float)9, (float)9);
	if (resultPassenger1 == 1)
	{
		printf("\n\nPassenger 5 at 17 chooses car 5\n\n");
		capacity[4]--;
	}
	else
	{
		printf("\n\nPassenger 5 at 17 chooses car 6");
		capacity[5]--;
	}
	

	printf("Passenger6 has priority :: distanceToRoadCondition = 1/9 distanceToTraffic = 1/9 roadconditionToTraffic = 5");
	 resultPassenger1 = compare(dist1, rc1, traffic1, dist2, rc2, traffic2, 1/9,1/9,5);
	if (resultPassenger1 == 1)
	{
		printf("\n\n\nPassenger 6 at 12 chooses car 1\n");
		capacity[0]--;
	}
	else
	{
		printf("\n\n\nPassenger 6 at 12 chooses car 2\n");
		capacity[1]--;
	}
	//p6p7

	printf("Passenger7 has priority :: distanceToRoadCondition = 1/9  distanceToTraffic = 9 roadconditionToTraffic = 9");
	 resultPassenger1 = compare(dist1, rc1, traffic1, dist2, rc2, traffic2, 1/9,9,9);
	if (resultPassenger1 == 1)
	{
		printf("\n\n\nPassenger 7 at 5 chooses car 1\n");
		capacity[0]--;
	}
	else
	{
		printf("\n\n\nPassenger 7 at 5 chooses car 2\n");
		capacity[1]--;
	}
	for (int i = 0; i <= 5; i++)
	{
		printf("\n\n Capacity of car%d = %d", i + 1, capacity[i]);
	}
	return 0;
}