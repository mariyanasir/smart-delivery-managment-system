#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_ORDERS 100
#define MAX_LOCATIONS 20

struct Order
{
    int orderId;
    int customerId;
    int restaurantId;
    int restaurantLocation;
    int customerLocation;
    char foodItem[50];
    int quantity;
    int priority;
    int status;
};

struct Partner
{
    int partnerId;
    char name[50];
    int location;
    int available;
    struct Partner *next;
};

struct History
{
    int orderId;
    int customerId;
    char foodItem[50];
    int status;
    struct History *next;
};

struct Order priorityQueue[MAX_ORDERS];
int prioritySize = 0;

struct Partner *partnerHead = NULL;
struct History *historyHead = NULL;

int graph[MAX_LOCATIONS][MAX_LOCATIONS];
int locations = 0;

int addOrder()
{
    struct Order order;

    if (prioritySize == MAX_ORDERS)
    {
        printf("\nOrder queue is full.\n");
        return 0;
    }

    printf("\nEnter Order ID: ");
    scanf("%d", &order.orderId);

    printf("Enter Customer ID: ");
    scanf("%d", &order.customerId);

    printf("Enter Restaurant ID: ");
    scanf("%d", &order.restaurantId);

    printf("Enter Restaurant Location: ");
    scanf("%d", &order.restaurantLocation);

    printf("Enter Customer Location: ");
    scanf("%d", &order.customerLocation);

    printf("Enter Food Item: ");
    scanf(" %[^\n]", order.foodItem);

    printf("Enter Quantity: ");
    scanf("%d", &order.quantity);

    printf("\nPriority:\n");
    printf("1. Urgent\n");
    printf("2. High\n");
    printf("3. Normal\n");
    printf("Enter Priority: ");
    scanf("%d", &order.priority);

    order.status = 1;

    priorityQueue[prioritySize] = order;
    prioritySize++;

    printf("\nOrder added successfully.\n");

    return 1;
}

int displayOrders()
{
    int i;

    if (prioritySize == 0)
    {
        printf("\nNo pending orders.\n");
        return 0;
    }

    printf("\n========== PENDING ORDERS ==========\n");

    for (i = 0; i < prioritySize; i++)
    {
        printf("\nOrder ID: %d", priorityQueue[i].orderId);
        printf("\nCustomer ID: %d", priorityQueue[i].customerId);
        printf("\nRestaurant ID: %d", priorityQueue[i].restaurantId);
        printf("\nFood: %s", priorityQueue[i].foodItem);
        printf("\nQuantity: %d", priorityQueue[i].quantity);
        printf("\nPriority: ");

        if (priorityQueue[i].priority == 1)
            printf("Urgent");
        else if (priorityQueue[i].priority == 2)
            printf("High");
        else
            printf("Normal");

        printf("\n-----------------------------------");
    }

    printf("\n");

    return 1;
}

int getHighestPriority()
{
    int i;
    int index = 0;

    for (i = 1; i < prioritySize; i++)
    {
        if (priorityQueue[i].priority < priorityQueue[index].priority)
        {
            index = i;
        }
    }

    return index;
}

int addPartner()
{
    struct Partner *newPartner;
    struct Partner *temp;

    newPartner = (struct Partner *)malloc(sizeof(struct Partner));

    if (newPartner == NULL)
    {
        printf("\nMemory allocation failed.\n");
        return 0;
    }

    printf("\nEnter Partner ID: ");
    scanf("%d", &newPartner->partnerId);

    printf("Enter Partner Name: ");
    scanf(" %[^\n]", newPartner->name);

    printf("Enter Current Location: ");
    scanf("%d", &newPartner->location);

    newPartner->available = 1;
    newPartner->next = NULL;

    if (partnerHead == NULL)
    {
        partnerHead = newPartner;
    }
    else
    {
        temp = partnerHead;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newPartner;
    }

    printf("\nDelivery partner added successfully.\n");

    return 1;
}

int displayPartners()
{
    struct Partner *temp;

    if (partnerHead == NULL)
    {
        printf("\nNo delivery partners available.\n");
        return 0;
    }

    temp = partnerHead;

    printf("\n========== DELIVERY PARTNERS ==========\n");

    while (temp != NULL)
    {
        printf("\nPartner ID: %d", temp->partnerId);
        printf("\nName: %s", temp->name);
        printf("\nLocation: %d", temp->location);
        printf("\nStatus: ");

        if (temp->available == 1)
            printf("Available");
        else
            printf("Busy");

        printf("\n---------------------------------------");

        temp = temp->next;
    }

    printf("\n");

    return 1;
}

int addRoad()
{
    int source;
    int destination;
    int distance;

    printf("\nEnter Source Location: ");
    scanf("%d", &source);

    printf("Enter Destination Location: ");
    scanf("%d", &destination);

    printf("Enter Distance: ");
    scanf("%d", &distance);

    if (source < 0 || source >= MAX_LOCATIONS ||
        destination < 0 || destination >= MAX_LOCATIONS)
    {
        printf("\nInvalid location.\n");
        return 0;
    }

    graph[source][destination] = distance;
    graph[destination][source] = distance;

    if (source >= locations)
        locations = source + 1;

    if (destination >= locations)
        locations = destination + 1;

    printf("\nRoad added successfully.\n");

    return 1;
}

int displayGraph()
{
    int i;
    int j;

    printf("\n========== ROAD NETWORK ==========\n");

    for (i = 0; i < locations; i++)
    {
        printf("\nLocation %d:", i);

        for (j = 0; j < locations; j++)
        {
            if (graph[i][j] != 0)
            {
                printf(" -> Location %d (%d km)", j, graph[i][j]);
            }
        }
    }

    printf("\n");

    return 1;
}

int dijkstra(int source, int destination, int path[], int *pathSize)
{
    int distance[MAX_LOCATIONS];
    int visited[MAX_LOCATIONS];
    int previous[MAX_LOCATIONS];

    int i;
    int j;
    int minDistance;
    int current;
    int count;
    int reversePath[MAX_LOCATIONS];

    for (i = 0; i < locations; i++)
    {
        distance[i] = INT_MAX;
        visited[i] = 0;
        previous[i] = -1;
    }

    distance[source] = 0;

    for (i = 0; i < locations; i++)
    {
        minDistance = INT_MAX;
        current = -1;

        for (j = 0; j < locations; j++)
        {
            if (visited[j] == 0 && distance[j] < minDistance)
            {
                minDistance = distance[j];
                current = j;
            }
        }

        if (current == -1)
            break;

        visited[current] = 1;

        for (j = 0; j < locations; j++)
        {
            if (graph[current][j] != 0 &&
                visited[j] == 0 &&
                distance[current] != INT_MAX &&
                distance[current] + graph[current][j] < distance[j])
            {
                distance[j] = distance[current] + graph[current][j];
                previous[j] = current;
            }
        }
    }

    if (distance[destination] == INT_MAX)
    {
        return -1;
    }

    count = 0;
    current = destination;

    while (current != -1)
    {
        reversePath[count] = current;
        count++;
        current = previous[current];
    }

    *pathSize = count;

    for (i = 0; i < count; i++)
    {
        path[i] = reversePath[count - i - 1];
    }

    return distance[destination];
}

int findShortestRoute()
{
    int source;
    int destination;
    int path[MAX_LOCATIONS];
    int pathSize;
    int distance;
    int i;

    printf("\nEnter Restaurant Location: ");
    scanf("%d", &source);

    printf("Enter Customer Location: ");
    scanf("%d", &destination);

    if (source < 0 || source >= locations ||
        destination < 0 || destination >= locations)
    {
        printf("\nInvalid location.\n");
        return 0;
    }

    distance = dijkstra(source, destination, path, &pathSize);

    if (distance == -1)
    {
        printf("\nNo route exists between these locations.\n");
        return 0;
    }

    printf("\n========== SHORTEST ROUTE ==========\n");

    printf("Route: ");

    for (i = 0; i < pathSize; i++)
    {
        printf("%d", path[i]);

        if (i != pathSize - 1)
            printf(" -> ");
    }

    printf("\nTotal Distance: %d km\n", distance);

    return 1;
}

int findNearestPartner(int restaurantLocation)
{
    struct Partner *temp;
    struct Partner *selected;
    int path[MAX_LOCATIONS];
    int pathSize;
    int distance;
    int minimumDistance;

    temp = partnerHead;
    selected = NULL;
    minimumDistance = INT_MAX;

    while (temp != NULL)
    {
        if (temp->available == 1)
        {
            distance = dijkstra(temp->location, restaurantLocation,
                                path, &pathSize);

            if (distance != -1 && distance < minimumDistance)
            {
                minimumDistance = distance;
                selected = temp;
            }
        }

        temp = temp->next;
    }

    if (selected == NULL)
    {
        printf("\nNo available delivery partner found.\n");
        return 0;
    }

    selected->available = 0;

    printf("\nDelivery Partner Assigned\n");
    printf("Partner ID: %d\n", selected->partnerId);
    printf("Partner Name: %s\n", selected->name);
    printf("Distance from Restaurant: %d km\n", minimumDistance);

    return selected->partnerId;
}

int addHistory(struct Order order)
{
    struct History *newHistory;
    struct History *temp;

    newHistory = (struct History *)malloc(sizeof(struct History));

    if (newHistory == NULL)
    {
        printf("\nMemory allocation failed.\n");
        return 0;
    }

    newHistory->orderId = order.orderId;
    newHistory->customerId = order.customerId;
    strcpy(newHistory->foodItem, order.foodItem);
    newHistory->status = order.status;
    newHistory->next = NULL;

    if (historyHead == NULL)
    {
        historyHead = newHistory;
    }
    else
    {
        temp = historyHead;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newHistory;
    }

    return 1;
}

int processOrder()
{
    int index;
    int i;
    int partnerId;
    struct Order order;
    int path[MAX_LOCATIONS];
    int pathSize;
    int distance;

    if (prioritySize == 0)
    {
        printf("\nNo pending orders.\n");
        return 0;
    }

    index = getHighestPriority();
    order = priorityQueue[index];

    printf("\n========== PROCESSING ORDER ==========\n");

    printf("Order ID: %d\n", order.orderId);
    printf("Food: %s\n", order.foodItem);

    partnerId = findNearestPartner(order.restaurantLocation);

    if (partnerId == 0)
    {
        printf("\nOrder cannot be processed right now.\n");
        return 0;
    }

    distance = dijkstra(order.restaurantLocation,
                         order.customerLocation,
                         path,
                         &pathSize);

    if (distance == -1)
    {
        printf("\nNo delivery route available.\n");
        return 0;
    }

    printf("\nShortest Delivery Route: ");

    for (i = 0; i < pathSize; i++)
    {
        printf("%d", path[i]);

        if (i != pathSize - 1)
            printf(" -> ");
    }

    printf("\nTotal Delivery Distance: %d km\n", distance);

    order.status = 3;

    addHistory(order);

    for (i = index; i < prioritySize - 1; i++)
    {
        priorityQueue[i] = priorityQueue[i + 1];
    }

    prioritySize--;

    printf("\nOrder %d delivered successfully.\n", order.orderId);

    return 1;
}

int cancelOrder()
{
    int orderId;
    int i;
    int found = 0;
    struct Order order;

    printf("\nEnter Order ID to cancel: ");
    scanf("%d", &orderId);

    for (i = 0; i < prioritySize; i++)
    {
        if (priorityQueue[i].orderId == orderId)
        {
            order = priorityQueue[i];
            order.status = 4;

            addHistory(order);

            for (; i < prioritySize - 1; i++)
            {
                priorityQueue[i] = priorityQueue[i + 1];
            }

            prioritySize--;

            found = 1;

            printf("\nOrder %d cancelled successfully.\n", orderId);

            break;
        }
    }

    if (found == 0)
    {
        printf("\nOrder not found.\n");
    }

    return found;
}

int displayHistory()
{
    struct History *temp;

    if (historyHead == NULL)
    {
        printf("\nNo order history available.\n");
        return 0;
    }

    temp = historyHead;

    printf("\n========== ORDER HISTORY ==========\n");

    while (temp != NULL)
    {
        printf("\nOrder ID: %d", temp->orderId);
        printf("\nCustomer ID: %d", temp->customerId);
        printf("\nFood: %s", temp->foodItem);
        printf("\nStatus: ");

        if (temp->status == 3)
            printf("Delivered");
        else if (temp->status == 4)
            printf("Cancelled");

        printf("\n----------------------------------");

        temp = temp->next;
    }

    printf("\n");

    return 1;
}

int assignPartner()
{
    int restaurantLocation;

    printf("\nEnter Restaurant Location: ");
    scanf("%d", &restaurantLocation);

    findNearestPartner(restaurantLocation);

    return 1;
}

int main()
{
    int choice;

    for (int i = 0; i < MAX_LOCATIONS; i++)
    {
        for (int j = 0; j < MAX_LOCATIONS; j++)
        {
            graph[i][j] = 0;
        }
    }

    do
    {
        printf("\n\n============================================");
        printf("\n     SMART FOOD DELIVERY MANAGEMENT SYSTEM");
        printf("\n============================================");
        printf("\n1. Add Order");
        printf("\n2. Display Pending Orders");
        printf("\n3. Process Next Order");
        printf("\n4. Add Delivery Partner");
        printf("\n5. Display Delivery Partners");
        printf("\n6. Assign Delivery Partner");
        printf("\n7. Add Road");
        printf("\n8. Display Road Network");
        printf("\n9. Find Shortest Delivery Route");
        printf("\n10. Display Order History");
        printf("\n11. Cancel Order");
        printf("\n12. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addOrder();
                break;

            case 2:
                displayOrders();
                break;

            case 3:
                processOrder();
                break;

            case 4:
                addPartner();
                break;

            case 5:
                displayPartners();
                break;

            case 6:
                assignPartner();
                break;

            case 7:
                addRoad();
                break;

            case 8:
                displayGraph();
                break;

            case 9:
                findShortestRoute();
                break;

            case 10:
                displayHistory();
                break;

            case 11:
                cancelOrder();
                break;

            case 12:
                printf("\nThank you for using the system.\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 12);

    return 0;
}
