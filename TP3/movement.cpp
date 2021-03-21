#include <iostream>
#include "movement.h"
#include "Geometry.h"
#include <cstdlib>

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
                    getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld);   // !!!!!!! ATENCION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
                }
                else if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                    getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld);
                }
            }
        }
        i++;
    }
}
// evalua la colision entre blob y comida. Conviene meterla adentro de getBlobnextDir para bajar complejidad. QUE HACEMOS CON WIDTH Y HEIGHT
void getCollisionOnFood(Blob* myBlob, Food* myFood, World * MyWorld)
{
    CollBox c1(myBlob->size, myBlob->size, myBlob->pos);             // EL  HEIGHT Y WIDTH ES DISTINTO OBVIO
    CollBox c2(myFood->size, myFood->size, myFood->pos);            // Habra que hacer un miembro para width y height? o DEFINE ? o PARAMETRO ?
    if (isCollision(c1, c2))
    {
        myBlob->feed();
        switch (myBlob->age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            if (myBlob->foodCount == BABY_FOOD)
            {
                blobBirth(MyWorld, *myBlob);
                myBlob->foodCount = 0;
            }
            break;
        case GROWN_BLOB:
            if (myBlob->foodCount == GROWN_FOOD)
            {
                blobBirth(MyWorld, *myBlob);
                myBlob->foodCount = 0;
            }
            break;

        case GOOD_OLD_BLOB:
            if (myBlob->foodCount == GOOD_OLD_FOOD)
            {
                blobBirth(MyWorld, *myBlob);
            }
            break;
        }

        randPos(&myFood->pos, MyWorld);
    }
}

void randPos(Point* myPoint, World * myWorld)
{
    myPoint->x = rand() % myWorld->width;   // ES UNA CONSTANTE ?
    myPoint->y = rand() % myWorld->height;  // CHEQUIAR SI CAE ARRIBA DE ALGO? POR LA FOOD LO DIGO
}


void blobBirth(World * myWorld, Blob& padre)   // Para estudiar. No tiene sentido pasar un puntero como referencia, no?
{
    int i = 0;
    while (myWorld->blobs[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        myWorld->blobs[i].age = NEW_BORN;
        myWorld->blobs[i].pos = padre.pos;
        myWorld->blobs[i].angle = rand() % 360;  // No convendra un constructor
        myWorld->blobs[i].size = myWorld->sizes.babySize;
        myWorld->blobs[i].ticksAlive = 0;
        padre.foodCount = 0;

        myWorld->params.aliveBlobs++;          // <---- RECORDAR ESTO DE INCREMENTAR LOS BLOBS SI CREAS
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
        i++;
    }
}
// primero mergeo los baby, despues mergeo los grown, despues me fijo colision de baby con grown SEGUN KAMMAN{


/*
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
}*/


void growNewBorn(World * myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;
        if (myWorld->blobs[i].age == NEW_BORN && myWorld->blobs[i].ticksAlive > 10)
            myWorld->blobs[i].age = BABY_BLOB;
        else
            myWorld->blobs[i].ticksAlive++;
    }
}


void transportateBlob(World * myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;

        Point p1, p2, p3, p4;
        getVertixes(myWorld->blobs[i].pos, myWorld->width, myWorld->height, &p1, &p2, &p3, &p4);

        if (p2.x < 0)  // se fue para la izquierda
        {
            myWorld->blobs[i].pos.x = myWorld->width + myWorld->blobs[i].size / 2 - abs(p2.x);
        }
        else if (p1.x >= myWorld->width )
        {
            myWorld->blobs[i].pos.x = - myWorld->blobs[i].size / 2 + abs(p1.x - myWorld->width);
        }
        else if (p1.y >= myWorld->height)
        {
            myWorld->blobs[i].pos.y = - myWorld->blobs[i].size / 2 + abs(p1.y - myWorld->height);
        }
        else if(p3.y < 0)
        {
            myWorld->blobs[i].pos.y = myWorld->height + myWorld->blobs[i].size / 2 - abs(p3.y);
        }
    }
}
void moveBlobs(World * myWorld)
{
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;

        transportateBlob(myWorld);
        myWorld->blobs[i].move(myWorld->params.percentSpeed);  // 1.0 percentSpeed
        i++;
    }
}

void getVertixes(Point& pM, int width, int height, Point* p1, Point* p2, Point* p3, Point* p4)
{
    p1->x = pM.x - width / 2;          // Allegro sistema de coordenadas
    p3->x = pM.x - width / 2;          // curva de jordan
    p2->x = pM.x + width / 2;
    p4->x = pM.x + width / 2;

    p1->y = pM.y - height / 2;
    p3->y = pM.y + height / 2;
    p2->y = pM.y - height / 2;
    p4->y = pM.y + height / 2;
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


// FIX: ESTO NO DEBERIA ESTAR ACA:
World* createWorld(Parameters& params) {

    World* world = (World*) malloc(sizeof(World));

    world->params = params;
    
    Blob blob;
    blob.isAlive = true;
    blob.age = BABY_BLOB;
    blob.vel = params.maxSpeed;
    blob.smellRadius = params.smellRadius;
    blob.angle = rand() % 360;
    blob.foodCount = 0;
    //HARDCODEADO
    blob.size = 40;
    blob.pos.x = 100;
    blob.pos.y = 100;

    Food food;
    food.size = 20;
    food.pos.x = 200;
    food.pos.y = 200;


    for (int i = 0; i < MAX_BLOBS; i++) {
        if (i < params.aliveBlobs)
        {
            world->blobs[i] = blob;
        }
        world->blobs[i].isAlive = false;
    }
    world->f = (Food*)malloc(sizeof(food) * params.foodCount);
    for (int i = 0; i < params.foodCount; i++)
    {
        world->f[i] = food;
    }

    return world;
    // TODO: terminar

}

void deleteWorld(World* myWorld)
{
    free(myWorld->f);
    free(myWorld);
}


typedef unsigned int ColPair[2];

class ColReg {
public:
    ColPair *pairs;
    unsigned int size;
};



// TODO
/*
* Detectar pares de colisiones ~ return ColReg * ~ 
* Funcion para analizar pares de colision: checkColisions(ColReg& reg)
    - Recibe un arreglo de pares de colision
    - Llama a una callback por cada arreglo de colision encontrado
        - la callback se encarga del merge de todos los elementos
    - Devuelve algo?
*/
 
void checkColisions(ColReg& reg, colCallback callback, void *data) {

    unsigned int* colArr = malloc(2 * sizeof(unsigned int));    // Por lo menos voy a ecesitar 2 lugares
    unsigned int* checked = malloc(2 * sizeof(unsigned int));

    if (!collArr || !checked) return;

    ColPair* pairs = reg.pairs;

    unsigned int arrSize = 0;
    unsigned int checkSize = 0;
    unsigned int colCount = 0;

    unsigned int totalChecks = getDifferentValues(pairs, reg.size);

    while (checkSize != totalChecks) {

        int start = 1;  // El 0 ya lo considero

        if (colCount == 0) {    // La primera vez los agrego directamente
            colArr[0] = pairs[0][0];
            colArr[1] = pairs[0][1];

            checked[0] = pairs[0][0];     // Agrego el par que no estaba
            checked[1] = pairs[0][1];
            checkSize = 2;
        } else {
            int i = 0;
            while (!isInArray(pairs[++i][0], checked, checkSize));       // Hay al menos un par que no vi

            // El par pairs[i] no esta chequeado todavia:

            colArr[0] = pairs[i][0];                        // Agrego a los dos
            colArr[1] = pairs[i][1];
            arrSize = 2;

            checkSize += 2;     // Agrego el par a checked
            checked = realloc(checked, checkSize * sizeof(unsigned int));   // Hago lugar para mas elemento en checked
            if (!checked) return;
            checked[checkSize - 2] = pairs[i][0];     // Agrego el par que no estaba
            checked[checkSize - 1] = pairs[i][1];

            start = i+1;       // Para no volver a revisar los pares que ya tengo
        }

        colCount++;

        for (int i = start; i < reg.size; i++) {        // Arranca en start, los anteriores ya los vi
            bool inArray0 = isInArray(pairs[i][0], colArr, arrSize);
            bool inArray1 = isInArray(pairs[i][1], colArr, arrSize);

            if (inArray0 ^ inArray1) {     // Veo si hay un par que no tenia (uno esta y el otro no exclusivamente)

                arrSize++;
                colArr = realloc(colArr, arrsize * sizeof(unsigned int));   // Hago lugar para otro elemento
                if (!colArr) return;
                colArr[arrSize - 1] = inArray0 ? pairs[i][1] : pairs[i][0];     // Agrego el par que no estaba

                checkSize++;
                checked = realloc(checked, checkSize * sizeof(unsigned int));   // Hago lugar para otro elemento en checked
                if (!checked) return;
                checked[checkSize - 1] = inArray0 ? pairs[i][1] : pairs[i][0];     // Agrego el par que no estaba

                i = start-1;       // Encontre un nuevo valor, vuelvo a empezar el for para no perder colisiones
            }
        }

        // REVISAR: Tal vez el checked lo puedo modificar al final, solo hay que hacer append del colArr

        callback(colArr, arrSize, data);    // Llamo a la callback con el arreglo

    }
}

ColReg* detectPairs(World * myWorld, etaryGroupType Age)
{
    int blobsFound = 0;
    int i = 0;
    int typeBlobs = 0;
    int collisions = 0;
    ColReg * myRegister = (ColReg*)malloc(sizeof(ColReg));
    myRegister->size = 0;

    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false && i < MAX_BLOBS) i++;
        blobsFound++;
        if (myWorld->blobs[i].age == Age)
            typeBlobs++;
        i++;
    }

    i = 0;
    int typeFound = 0;
    while (typeFound < typeBlobs - 1)   // el ultimo typeBlob ya va a ser evaluado la colision
    {
        while (myWorld->blobs[i].isAlive == false && myWorld->blobs[i].age != Age && i < MAX_BLOBS) i++;
        typeFound++;

        int k = i + 1;
        int typeFound2 = typeFound;
        while (typeFound2 < typeBlobs)
        {
            while (myWorld->blobs[k].isAlive == false && myWorld->blobs[k].age != Age && k < MAX_BLOBS) k++;
            typeFound2++;

            if (myWorld->blobs[i].isCollision(myWorld->blobs[k]))   /// OJO QUE ES REFERENCIA
            {
                collisions++;
                myRegister->pairs = (ColPair*) realloc(myRegister->pairs, collisions * sizeof(ColPair));
                myRegister->pairs[collisions - 1][0] = i;
                myRegister->pairs[collisions - 1][1] = k;
                myRegister->size = collisions;
            }
            k++;
        }
        i++;
    }
    return myRegister;
}

/*
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
        Point p1 = translatePoint(b1->pos, distanceB2Points(b1->pos, b2->pos) / 2.0, getAngleBetweenPoints(b1->pos, b2->pos));
        arr[i].moveOffset(p1); // OJO LOS ANGULOS 
        // destruyo los b1 y b2
        b1->isAlive = false;
        b2->isAlive = false;
    }
}*/

void mergeBlobs (unsigned int* colArr, unsigned int size, World * myWorld )
{
    int i = 0;
    while (myWorld->blobs[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        if (myWorld->blobs[*colArr].age == BABY_BLOB)
            myWorld->blobs[i].age = GROWN_BLOB;
        else if (myWorld->blobs[*colArr].age == GROWN_BLOB)
            myWorld->blobs[i].age = GOOD_OLD_BLOB;

        Point p = { 0, 0 };
        averagePosition(colArr, size, myWorld->blobs, &p);
        myWorld->blobs[i].pos = p;
        myWorld->blobs[i].angle = averageDirection();
        
        for (int i = 0; i < size; i++)
        {
            myWorld->blobs[colArr[i]].isAlive = false;
        }
        myWorld->params.aliveBlobs -= (size - 1);
    }
}

void averagePosition(unsigned int *colArr, unsigned int size, Blob arr[], Point * myPoint)
{
    for (int i = 0; i < size; i++)
    {
        myPoint->x += arr[colArr[i]].pos.x;
        myPoint->y += arr[colArr[i]].pos.y;
    }
    myPoint->x /= size;
    myPoint->y /= size;
}


double averageDirection(void)
{
    /*Point a, b;
    a.x = sin(angle1 * PI / 180.0);
    a.y = cos(angle1 * PI / 180.0);          // TODO

    b.x = sin(angle2 * PI / 180.0);
    b.y = cos(angle2 * PI / 180.0);

    double averageAngle = atan2(a.x + b.x, a.y + b.y) * 180.0 / PI;  // OJO ATAN2

    return averageAngle;*/
    return rand() % 360;                     
}
