#include <iostream>
#include "movement.h"
#include "Geometry.h"
#include <cstdlib>

#include "World.h"

// Prototipos funciones privadas:

//Calcula la posicion promedio para el caso de mergeo de blob y creacion de uno nuevo
void averagePosition(unsigned int* colarr, unsigned int size, Blob arr[], Point* mypoint);
//Se fija si un blob colisiona con un alimento
void getCollisionOnFood(Blob* myBlob, Food* myFood, World* MyWorld);
//Calcula una posicion random dentro del mapa
void randPos(Point* myPoint, World* myWorld);
void getVertixes(Point& pM, int width, int height, Point* p1, Point* p2, Point* p3, Point* p4);
//Direccion promedio de los blobs participes del mergeo
double averageDirection(unsigned int* colarr, unsigned int size, Blob arr[], unsigned int extra);

unsigned int getDifferentValues(ColPair arr[], unsigned int size);

double averageSpeed(unsigned int* colArr, unsigned int size, World* myWorld);


// Crea un blob, debido a que un blob comio el alimento suficiente
void createBirth(World* myWorld, int indexBlob);
bool inMatrix(World* world, Point& p);


template <typename T> bool isInArray(T elem, T arr[], unsigned int size) {

    unsigned int i = 0;
    while (arr[i] != elem && ++i < size);

    if (i >= size) return false;
    return true;

}


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
    if(myWorld->f != NULL )
    {
        unsigned int blobsFound = 0;
        int i = 0;
        while (blobsFound < myWorld->params.aliveBlobs)
        {
            while (myWorld->blobs[i].isAlive == false) i++;

            blobsFound++;
            bool flag = 0;
            int shortestDistance;
            for (unsigned int j = 0; j < myWorld->params.foodCount; j++)
            {
                int distance;
    
                if ((distance = (unsigned int)distanceB2Points(myWorld->blobs[i].pos, myWorld->f[j].pos)) <= myWorld->blobs[i].smellRadius)
                {
                    if (flag == 0)
                    {
                        flag++;
                        shortestDistance = distance;
                        myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                        getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld);   
                    }
                    else if (distance < shortestDistance)
                    {
                        shortestDistance = distance;
                        myWorld->blobs[i].angle = getAngleBetweenPoints(myWorld->blobs[i].pos, myWorld->f[j].pos);
                        getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld);
                    }
                }
                else
                {
                    getCollisionOnFood(&myWorld->blobs[i], &myWorld->f[j], myWorld);
                }

            }
            i++;
        }
    }
}

void makeBlobBirth(World * myWorld)
{
    int blobsFound = 0;
    int i = 0;
    int births = 0;
    int* indexBlobs = NULL;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;

        switch (myWorld->blobs[i].age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            if (myWorld->blobs[i].foodCount == BABY_FOOD)
            {
                births++;
                indexBlobs = (int*)realloc(indexBlobs, births * sizeof(int));
                indexBlobs[births - 1] = i;
            }
            break;
        case GROWN_BLOB:
            if (myWorld->blobs[i].foodCount == GROWN_FOOD)
            {
                births++;
                indexBlobs = (int*)realloc(indexBlobs, births * sizeof(int));
                indexBlobs[births - 1] = i;
            }
            break;

        case GOOD_OLD_BLOB:
            if (myWorld->blobs[i].foodCount == GOOD_OLD_FOOD)
            {
                births++;
                indexBlobs = (int*)realloc(indexBlobs, births * sizeof(int));
                indexBlobs[births - 1] = i;
            }
            break;
        }
        i++;
    }
    for (int k = 0; k < births; k++)
    {
        createBirth(myWorld, indexBlobs[k] );
    }
    free(indexBlobs);
}

void createBirth(World * myWorld, int indexBlob)
{
    int i = 0;
    while (myWorld->blobs[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        myWorld->blobs[i].age = NEW_BORN;
        myWorld->blobs[i].pos = myWorld->blobs[indexBlob].pos;
        myWorld->blobs[i].angle = rand() % 360; 
        myWorld->blobs[i].size = myWorld->sizes.babySize;
        myWorld->blobs[i].ticksAlive = 0;
        myWorld->blobs[i].vel = myWorld->blobs[indexBlob].vel;
        myWorld->blobs[i].foodCount = 0;
        myWorld->blobs[i].smellRadius = myWorld->blobs[indexBlob].smellRadius;
        myWorld->blobs[i].isAlive = true;
        myWorld->blobs[indexBlob].foodCount = 0;

        myWorld->params.aliveBlobs++;        
    }
    else
    {
        std::cout << "UNABLE TO CREATE BLOBS" << std::endl;  
    }
}


void getCollisionOnFood(Blob* myBlob, Food* myFood, World * MyWorld)
{
    CollBox c1(myBlob->size, myBlob->size, myBlob->pos);           
    CollBox c2(myFood->size, myFood->size, myFood->pos);            
    if (isCollision(c1, c2))
    {
        myBlob->feed();

        randPos(&myFood->pos, MyWorld);
    }
}

void randPos(Point* myPoint, World * myWorld)
{
    myPoint->x = rand() % myWorld->width;  
    myPoint->y = rand() % myWorld->height;  
}


void blobDeath(World* myWorld)  
{
    int blobsFound = 0;
    int i = 0;
    int killedBlobs = 0;
    while (blobsFound < myWorld->params.aliveBlobs)
    {
        while (myWorld->blobs[i].isAlive == false) i++;
        blobsFound++;
        switch (myWorld->blobs[i].age)
        {
        case NEW_BORN:
        case BABY_BLOB:
            myWorld->blobs[i].isAlive = (double)rand() / (double)RAND_MAX < myWorld->params.deathProb[0]  ? false : true;
            if (myWorld->blobs[i].isAlive == false)
            {
                killedBlobs++;
            }
            break;
        case GROWN_BLOB:
            myWorld->blobs[i].isAlive = (double)rand() / (double)RAND_MAX < myWorld->params.deathProb[1]  ? false : true;
            if (myWorld->blobs[i].isAlive == false)
            {
                killedBlobs++;
            }
            break;
        case GOOD_OLD_BLOB:
            myWorld->blobs[i].isAlive = (double)rand() / (double)RAND_MAX < myWorld->params.deathProb[2] ? false : true;
            if (myWorld->blobs[i].isAlive == false)
            {
                killedBlobs++;
            }
            break;
        }
        i++;
    }
    myWorld->params.aliveBlobs -= killedBlobs;
}


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
        i++;
    }
}

void getVertixes(Point& pM, int width, int height, Point* p1, Point* p2, Point* p3, Point* p4)
{
    p1->x = pM.x - width / 2;          // Allegro sistema de coordenadas
    p3->x = pM.x - width / 2;          
    p2->x = pM.x + width / 2;
    p4->x = pM.x + width / 2;

    p1->y = pM.y - height / 2;
    p3->y = pM.y + height / 2;
    p2->y = pM.y - height / 2;
    p4->y = pM.y + height / 2;
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
        getVertixes(myWorld->blobs[i].pos, myWorld->blobs[0].size, myWorld->blobs[0].size, &p1, &p2, &p3, &p4);

        if (p2.x < 0)  // se fue para la izquierda
        {
            myWorld->blobs[i].pos.x = myWorld->width + myWorld->blobs[i].size / 2 - abs(p2.x);
        }
        else if (p1.x >= myWorld->width )
        {
            myWorld->blobs[i].pos.x = - (int)myWorld->blobs[i].size / 2 + abs(p1.x - myWorld->width);
        }
        else if (p1.y >= myWorld->height)
        {
            myWorld->blobs[i].pos.y = - (int)myWorld->blobs[i].size / 2 + abs(p1.y - myWorld->height);
        }
        else if(p3.y < 0)
        {
            myWorld->blobs[i].pos.y = myWorld->height + myWorld->blobs[i].size / 2 - abs(p3.y);
        }
        i++;
    }
}



// FIX: ESTO NO DEBERIA ESTAR ACA:
World* createWorld(Parameters& params) {

    srand(time(NULL));

    World* world = (World*) malloc(sizeof(World));

    if (!world) return NULL;

    world->params = params;
    // Seteo de parametros propios del World
    world->sizes.babySize = 40;
    world->sizes.foodSize = 20;
    world->sizes.growSize = 45;
    world->sizes.goodOldSize = 80;

    world->width = 900;
    world->height = 470;

    // Seteo de parametros comunes a todos los blobs
    Blob blob;
    blob.age = BABY_BLOB;
    blob.size = world->sizes.babySize;
    blob.smellRadius = params.smellRadius;
    blob.foodCount = 0;

    for (int i = 0; i < MAX_BLOBS; i++) {       // Inicializo todos los blobs con su propia velocidad y posicion
        world->blobs[i] = blob;
        world->blobs[i].isAlive = i < params.aliveBlobs;
        world->blobs[i].vel = world->params.mode == MODE_1 ? world->params.maxSpeed : rand() * (world->params.maxSpeed / RAND_MAX);
        world->blobs[i].angle = rand()*(360.0/RAND_MAX);
        randPos(&(world->blobs[i].pos), world);
    }

    world->f = (Food*)malloc(sizeof(Food) * params.foodCount);

    if (!world->f) return world;
    
    for (int i = 0; i < params.foodCount; i++)
    {
        world->f[i].size = 20;
        Point posF;
        randPos(&posF, world);
        world->f[i].pos = posF;
    }
    return world;
}

bool inMatrix(World* world, Point& p)	//Checkea si el robot se salio del piso
{
    bool onFloor = 0;
    if (p.x >= 0 && p.x < world->width &&
        p.y >= 0 && p.y < world->height)
        onFloor = 1;
    return onFloor;
}



void setFoodCount(World * world)
{
    world->f = (Food*)realloc(world->f, world->params.foodCount * sizeof(Food));
    if (world->f != NULL)
    {
        for (int i = 0; i < world->params.foodCount; i++)
        {
            if ( inMatrix(world, world->f[i].pos) == false )
            {
                randPos(&world->f[i].pos, world);
            }
            world->f[i].size = world->sizes.foodSize;
        }
    }
    else
    {
        free(world->f);
        std::cout << "Unable to reallocate memory" << std::endl;
    }
}
void setSmellRadius(World* world, int smellRadius)
{
    if (smellRadius < 0 )
    {
        smellRadius = 0;
    }
    int blobsFound = 0;
    int i = 0;
    while (blobsFound < world->params.aliveBlobs)
    {
        while (world->blobs[i].isAlive == false) i++;
        blobsFound++;
        world->blobs[i].smellRadius = smellRadius;
        i++;
    }
    world->params.smellRadius = smellRadius;
}

void deleteWorld(World* myWorld)
{
    free(myWorld->f);
    free(myWorld);
}
 
void checkColisions(ColReg& reg, colCallback callback, void *data) {

//DEBUG//////

    World* prueba = (World *)data;

////////////////
    unsigned int* colArr = (unsigned int *)malloc(2 * sizeof(unsigned int));    // Por lo menos voy a ecesitar 2 lugares
    unsigned int* checked = (unsigned int*)malloc(2 * sizeof(unsigned int));

    if (!colArr || !checked) return;

    ColPair* pairs = reg.pairs;

    unsigned int arrSize = 0;   // Tamano del arreglo de colisiones actual
    unsigned int checkSize = 0; // Tamano del arreglo checked
    unsigned int colCount = 0;

    unsigned int totalChecks = getDifferentValues(pairs, reg.size);

    while (checkSize != totalChecks) {

        int start = 1;  // El 0 ya lo considero

        if (colCount == 0) {    // La primera vez los agrego directamente
            colArr[0] = pairs[0][0];
            colArr[1] = pairs[0][1];
            arrSize = 2;

            checked[0] = pairs[0][0];     // Agrego el par que no estaba
            checked[1] = pairs[0][1];
            checkSize = 2;
        } else {
            int i = 0;
            while (isInArray(pairs[++i][0], checked, checkSize));       // Hay al menos un par que no vi

            // El par pairs[i] no esta chequeado todavia:

            colArr[0] = pairs[i][0];                        // Agrego a los dos
            colArr[1] = pairs[i][1];
            arrSize = 2;

            checkSize += 2;     // Agrego el par a checked
            checked = (unsigned int*)realloc(checked, checkSize * sizeof(unsigned int));   // Hago lugar para mas elemento en checked
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
                colArr = (unsigned int*)realloc(colArr, arrSize * sizeof(unsigned int));   // Hago lugar para otro elemento
                if (!colArr) return;
                colArr[arrSize - 1] = inArray0 ? pairs[i][1] : pairs[i][0];     // Agrego el par que no estaba

                checkSize++;
                checked = (unsigned int*)realloc(checked, checkSize * sizeof(unsigned int));   // Hago lugar para otro elemento en checked
                if (!checked) return;
                checked[checkSize - 1] = inArray0 ? pairs[i][1] : pairs[i][0];     // Agrego el par que no estaba

                i = start-1;       // Encontre un nuevo valor, vuelvo a empezar el for para no perder colisiones
            }
        }

        // REVISAR: Tal vez el checked lo puedo modificar al final, solo hay que hacer append del colArr

        callback(colArr, arrSize, data);    // Llamo a la callback con el arreglo

    }

    free(colArr);
    free(checked);

}

ColReg* detectPairs(World * myWorld, etaryGroupType Age)
{
    int blobsFound = 0;
    int i = 0;
    int typeBlobs = 0;
    int collisions = 0;
    ColReg * myRegister = (ColReg*)malloc(sizeof(ColReg));
    myRegister->pairs = NULL;
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
        while (myWorld->blobs[i].isAlive == false || myWorld->blobs[i].age != Age && i < MAX_BLOBS) i++;
        typeFound++;

        int k = i + 1;
        int typeFound2 = typeFound;
        while (typeFound2 < typeBlobs)
        {
            while (myWorld->blobs[k].isAlive == false || myWorld->blobs[k].age != Age && k < MAX_BLOBS) k++;
            typeFound2++;

            if (myWorld->blobs[i].isCollision(myWorld->blobs[k]))   /// OJO QUE ES REFERENCIA
            {
                collisions++;
                myRegister->pairs = (ColPair*) realloc(myRegister->pairs, collisions * sizeof(ColPair));
                if (!myRegister->pairs) return myRegister;
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


void mergeBlobs (unsigned int* colArr, unsigned int size, void * var )
{
    World * myWorld = (World*)var;
    int i = 0;
    while (myWorld->blobs[i].isAlive == true && i < MAX_BLOBS) i++;
    if (i != MAX_BLOBS)
    {
        if (myWorld->blobs[*colArr].age == BABY_BLOB)
        {
            myWorld->blobs[i].age = GROWN_BLOB;
            myWorld->blobs[i].size = myWorld->sizes.growSize;
        }
        else if (myWorld->blobs[*colArr].age == GROWN_BLOB)
        {
            myWorld->blobs[i].age = GOOD_OLD_BLOB;
            myWorld->blobs[i].size = myWorld->sizes.goodOldSize;
        }

        Point p = { 0, 0 };
        averagePosition(colArr, size, myWorld->blobs, &p);
        myWorld->blobs[i].pos = p;
        myWorld->blobs[i].angle = averageDirection(colArr, size, myWorld->blobs, myWorld->params.randomJiggleLimit);
        
        myWorld->blobs[i].vel = averageSpeed(colArr, size, myWorld);
        myWorld->blobs[i].foodCount = 0;
        myWorld->blobs[i].smellRadius = myWorld->params.smellRadius;
        myWorld->blobs[i].isAlive = true;


        // TEST --------------------------
        for (int i = 0; i < size; i++) {
            std::cout << "Pos = "<< colArr[i] << std::endl;
        }
        std::cout << i << std::endl;

        for (int i = 0; i < size; i++)
        {
            myWorld->blobs[colArr[i]].isAlive = false;
        }
        myWorld->params.aliveBlobs -= (size - 1);
    }
}

double averageSpeed(unsigned int* colArr, unsigned int size, World * myWorld)
{
    double averageSpeed = 0;
    for (int i = 0; i < size; i++)
    {
        averageSpeed += myWorld->blobs[colArr[i]].vel;
    }
    averageSpeed /= size;
    if (averageSpeed == 0)
    {
        averageSpeed = myWorld->blobs[colArr[0]].vel;
    }
    return averageSpeed;
}



void averagePosition(unsigned int *colarr, unsigned int size, Blob arr[], Point * mypoint)
{
    for (int i = 0; i < size; i++)
    {
        mypoint->x += arr[colarr[i]].pos.x;
        mypoint->y += arr[colarr[i]].pos.y;
    }
    mypoint->x /= size;
    mypoint->y /= size;
}

double averageDirection(unsigned int* colarr, unsigned int size, Blob arr[], unsigned int extra)
{
    double angle = 0;
    float ejex = 0, ejey = 0;
    printf("extra = %i\n", extra);
    for (int i = 0; i < size; i++)
    {
        ejey += cos((arr[colarr[i]].angle)* PI /180);
        ejex += sin((arr[colarr[i]].angle)* PI /180);
        printf("angle inicial = %i\n", arr[colarr[i]].angle);
    }

    if (ejex == 0)
    {
        if (ejey > 0)
            angle = 0;
        else
            angle = 180;
    }
    else if (ejey == 0)
    {
        if (ejex > 90)
            angle = 0;
        else
            angle = 270;
    }
    else
    {
        angle = atan(ejex / ejey) * 180.0 / PI;

        if (ejey >= 0 && ejex >= 0)
            angle = angle;
        if (ejey < 0 && ejex >= 0)
            angle = angle + (double)180;
        if (ejey < 0 && ejex < 0)
            angle = angle + (double)180;
        if (ejey >= 0 && ejex < 0)
            angle = angle + (double)360;
    }

    angle = angle + extra;

    if (angle >= 360)
        angle = angle - 360;

    return angle;
}

void moveBlobs(World& world) {

    int blobCount = 0;
    int i = 0;
    while (blobCount < world.params.aliveBlobs) {
        if (world.blobs[i].isAlive) {
            blobCount++;
            world.blobs[i].move(world.params.percentSpeed);
        }
        i++;
    }
}

unsigned int getDifferentValues(ColPair arr[], unsigned int size) {

    if (size > 0) {
        unsigned int* checked = (unsigned int*)malloc(2 * sizeof(unsigned int));
        if (!checked) return 0;
        checked[0] = arr[0][0];
        checked[1] = arr[0][1];
        unsigned int checkSize = 2;

        for (int i = 1; i < size; i++) {

            for (int j = 0; j < 2; j++) {
                int h = 0;
                while (arr[i][j] != checked[h] && ++h < checkSize);
                if (h == checkSize) {
                    checkSize++;
                    checked = (unsigned int*)realloc(checked, checkSize * sizeof(unsigned int));
                    if (!checked) return 0;
                    checked[checkSize - 1] = arr[i][j];
                }

            }
        }

        free(checked);
        return checkSize;
    }
    return 0;
}