#include <iostream>
#include "movement.h"

#include "World.h"

bool isCollision(CollBox& box1, CollBox& box2)
{
    if (box1.p.x <= box2.p.x + box2.width &&                          // Para ver si dos cajas estan chocando
        box2.p.x <= box1.p.x + box1.width &&
        box1.p.y <= box2.p.y + box2.height &&
        box2.p.y <= box1.p.y + box1.height)
        return 1;
    return 0;
}

int distanceB2Points(Point& p1, Point& p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double getAngleBetweenPoints(Point& p1, Point& p2)
{
    double angle = atan2(p2.x - p1.x, p2.y - p1.y) * 180.0 / PI;
    if (angle < 0)
    {
        angle = 360 + angle;
    }
    return angle;
}

void getBlobNextFoodDirection(World* myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false && i < MAX_BLOBS) i++;

        blobsFound++;
        bool flag = 0;
        for (int j = 0; j < myWorld->params.foodCount; j++)
        {
            int distance, shortestDistance;
            if ((distance = distanceB2Points(myWorld->blobs[i].pos, myWorld->f[j].pos)) <= myWorld->blobs[i].smellRadius)
            {
                if (flag == 0)
                {
                    shortestDistance = distance;
                    myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                    bobFeeding(&myWorld->blobs[i], &myWorld->f[j], myWorld->blobs);   // !!!!!!! ATENCION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
                }
                else if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                    bobFeeding(&myWorld->blobs[i], &myWorld->f[j], myWorld->blobs);
                }
            }
        }
    }
}
// evalua la colision entre blob y comida. Conviene meterla adentro de getBlobnextDir para bajar complejidad. QUE HACEMOS CON WIDTH Y HEIGHT
void bobFeeding(Blob* myBlob, Food* myFloor, Blob arr[])
{
    CollBox c1(myBlob->width, myBlob->height, myBlob->pos);             // EL  HEIGHT Y WIDTH ES DISTINTO OBVIO
    CollBox c2(myFloor->width, myFloor->height, myFloor->pos);            // Habra que hacer un miembro para width y height? o DEFINE ? o PARAMETRO ?
    if (isCollision(c1, c2))
    {
        myBlob->feed();
        switch (myBlob->age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            if (myBlob->foodCount == BABY_FOOD)
            {
                blobBirth(arr, *myBlob);
                myBlob->foodCount = 0;
            }
            break;
        case GROWN_BLOB:
            if (myBlob->foodCount == GROWN_FOOD)
            {
                blobBirth(arr, *myBlob);
                myBlob->foodCount = 0;
            }
            break;

        case GOOD_OLD_BOB:
            if (myBlob->foodCount == GOOD_OLD_FOOD)
            {
                blobBirth(arr, *myBlob);
                myBlob->foodCount = 0;
            }
            break;
        }

        randPos(&myFloor->pos);
    }
}

void randPos(Point* myPoint)
{
    myPoint->x = rand() % D_WIDTH;   // ES UNA CONSTANTE ?
    myPoint->y = rand() % D_HEIGHT;  // CHEQUIAR SI CAE ARRIBA DE ALGO? POR LA FOOD LO DIGO
}

// Una vez mas, Se podra por referencia blob& padre??????? Se llama en blobfeeding
void blobBirth(Blob arr[], Blob& padre)   // Para estudiar. No tiene sentido pasar un puntero como referencia, no?
{
    int i = 0;
    while (arr[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        arr[i].age = NEW_BORN;
        arr[i].pos = padre.pos;
        arr[i].angle = RAND...    // No convendra un constructor?
    }
    else
    {
        std::cout << "UNABLE TO CREATE BLOBS" << std::endl;    // SACAR, NO ES NECESARIO
    }
}

void blobDeath(World* myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false && i < MAX_BLOBS) i++;
        blobsFound++;
        switch (myWorld->blobs[i].age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[0] ? false : true;
            break;
        case GROWN_BLOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[1] ? false : true;
            break;
        case GOOD_OLD_BOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[2] ? false : true;
            break;
        }
    }
}
// primero mergeo los baby, despues mergeo los grown, despues me fijo colision de baby con grown SEGUN KAMMAN
void blobMerge(World* myWorld)
{

}

