#include <iostream>
#include "movement.h"
#include "Geometry.h"

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

void BlobsFoodAction(World* myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;

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
                    getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld->blobs);   // !!!!!!! ATENCION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
                }
                else if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                    getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld->blobs);
                }
            }
        }
    }
}
// evalua la colision entre blob y comida. Conviene meterla adentro de getBlobnextDir para bajar complejidad. QUE HACEMOS CON WIDTH Y HEIGHT
void getCollisionOnFood(Blob* myBlob, Food* myFloor, Blob arr[])
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

        case GOOD_OLD_BLOB:
            if (myBlob->foodCount == GOOD_OLD_FOOD)
            {
                blobBirth(arr, *myBlob);
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


void blobBirth(Blob arr[], Blob& padre)   // Para estudiar. No tiene sentido pasar un puntero como referencia, no?
{
    int i = 0;
    while (arr[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        arr[i].age = NEW_BORN;
        arr[i].pos = padre.pos;
        arr[i].angle = rand() % 360;  // No convendra un constructor
        padre.foodCount = 0;
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
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;
        switch (myWorld->blobs[i].age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[0] * 100.0 ? false : true;
            break;
        case GROWN_BLOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[1] * 100.0 ? false : true;
            break;
        case GOOD_OLD_BLOB:
            myWorld->blobs[i].isAlive = (rand() % 100) < myWorld->params.deathProb[2] * 100.0 ? false : true;
            break;
        }
    }
}
// primero mergeo los baby, despues mergeo los grown, despues me fijo colision de baby con grown SEGUN KAMMAN{



void mergeBabyWithBaby(World* myWorld)
{
    int blobsFound = 0;
    int i = 0;
    int babyBlobs = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false && i < MAX_BLOBS) i++;
        blobsFound++;
        if (myWorld->blobs[i].age == BABY_BLOB)
            babyBlobs++;
    }
    i = 0;
    int babyBlobsFound = 0;
    while (babyBlobsFound < babyBlobs - 1)   // el ultimo babyBlob ya va a ser evaluado la colision
    {
        while (myWorld->blobs[i].isAlive == false && myWorld->blobs[i].age != BABY_BLOB && i < MAX_BLOBS) i++;
        babyBlobsFound++;

        int k = i + 1;
        int babyBlobsFound2 = babyBlobsFound;
        while (babyBlobsFound2 < babyBlobs && myWorld->blobs[i].isAlive == true)
        {
            while (myWorld->blobs[k].isAlive == false && myWorld->blobs[k].age != BABY_BLOB && i < MAX_BLOBS) k++;
            babyBlobsFound2++;

            if (myWorld->blobs[i].isCollision(myWorld->blobs[k]))   /// OJO QUE ES REFERENCIA
            {
                babyBlobs -= 1;
                mergeBlob(myWorld, myWorld->blobs + i, myWorld->blobs + k, myWorld->blobs);
            }
        }
    }
}

void mergeBlob(World* myWorld, Blob* b1, Blob* b2, Blob arr[])
{
    int i = 0;
    while (arr[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        if (b1->age == BABY_BLOB)
            arr[i].age = GROWN_BLOB;
        else if (b1->age == GROWN_BLOB)
            arr[i].age = GOOD_OLD_BLOB;

        arr[i].angle = averageDirection(b1->angle, b2->angle) + rand() % myWorld->params.randomJiggleLimit;
        arr[i].pos = b1->pos;
        arr[i].moveOffset(translatePoint(b1->pos, distanceB2Points(b1->pos, b2->pos) / 2.0, getAngleBetweenPoints(b1->pos, b2->pos))); // OJO LOS ANGULOS 
        // destruyo los b1 y b2
        b1->isAlive = false;
        b2->isAlive = false;
    }
}

void growNewBorn(World * myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;
        if (myWorld->blobs[i].age = NEW_BORN && myWorld->blobs[i].ticksAlive > UN_NUMERO)
            myWorld->blobs[i].age = GROWN_BLOB;
    }
}


double averageDirection(double angle1, double angle2)
{
    Point a, b;
    a.x = sin(angle1 * PI / 180.0);
    a.y = cos(angle1 * PI / 180.0);

    b.x = sin(angle2 * PI / 180.0);
    b.y = cos(angle2 * PI / 180.0);

    double averageAngle = atan2(a.x + b.x, a.y + b.y) * 180.0 / PI;  // OJO ATAN2
    return averageAngle;
}

void createFood(World* myWorld)
{
    myWorld->f = (Food*)realloc(myWorld->f, myWorld->params.foodCount * sizeof(Food));
    if (myWorld->f == NULL)
        return;
}

void deleteFood(World* myWorld)
{
    free(myWorld->f);
}

void createWorld(World* myWorld)
{
    createFood(myWorld);
}

