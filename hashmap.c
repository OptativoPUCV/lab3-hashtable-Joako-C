#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap* map, char* key, void* value) {
    if (map == NULL || key == NULL) return;

    long i = hash(key, map->capacity);

    while (map->buckets[i] != NULL) {
        i = (i + 1) % map->capacity;
    }

    Pair* new_pair = createPair(key, value);
    if (new_pair == NULL) return;

    map->buckets[i] = new_pair;
    map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap* createMap(long capacity) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL) {
        return NULL;
    }
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;

    map->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }
    return map;
}


void eraseMap(HashMap * map,  char * key) {
  if (map == NULL || key == NULL) {
    return;
  }

  long i = hash(key, map->capacity);

  while (map->buckets[i] != NULL) {
    Pair* current = map->buckets[i];
    if (is_equal(current->key, key)) {
      free(current->key);
      free(current->value);
      free(current);
      map->buckets[i] = NULL;
      map->size--;
      return;
    }

    i = (i + 1) % map->capacity;
        
    if (i == hash(key, map->capacity)) {
      return;
    }
  }
}


Pair* searchMap(HashMap* map, char* key) {
  long index = hash(key, map->capacity);
  long original_i = i;

  while (map->buckets[i] != NULL) {
    Pair* currentPair = map->buckets[i];
    if (strcmp(currentPair->key, key) == 0) {
      map->current = i;
      return currentPair;
    }

    i = (i + 1) % map->capacity;

    if (index == original_index) {
      break;
    }
  }
    map->current = -1;
    return NULL;
}


Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
