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

void enlarge(HashMap* map) {
  enlarge_called = 1;

  if (map == NULL) {
    return;
  }

  long new_capacity = map->capacity * 2;
  Pair** new_buckets = (Pair**)calloc(new_capacity, sizeof(Pair*));

  if (new_buckets == NULL) {
    return;
  }

  for (long i = 0; i < map->capacity; i++) {
    Pair* currentPair = map->buckets[i];
    if (currentPair != NULL && currentPair->key != NULL) {
      long pos = hash(currentPair->key, new_capacity);
      while (new_buckets[pos] != NULL) {
        pos = (pos + 1) % new_capacity;
      }
      new_buckets[pos] = currentPair;
    }
  }

  free(map->buckets);
  map->buckets = new_buckets;
  map->capacity = new_capacity;
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

void eraseMap(HashMap* map, char* key) {
  for (int i = 0; i < map->capacity; i++) {
    int pos = (hash(key, map->capacity) + i) % map->capacity;
    Pair* currentPair = map->buckets[pos];
    
    if (currentPair == NULL) {
      map->current = -1;
      return;
    }
    
    if (strcmp(currentPair->key, key) == 0) {
      currentPair->key = NULL;
      map->size--;
      map->current = pos;
      return;
    }
  }
}

Pair* searchMap(HashMap* map, char* key) {
  long i = hash(key, map->capacity);
  long original_i = i;

  while (map->buckets[i] != NULL) {
    Pair* currentPair = map->buckets[i];
    if (strcmp(currentPair->key, key) == 0) {
      map->current = i;
      return currentPair;
    }

    i = (i + 1) % map->capacity;

    if (i == original_i) {
      break;
    }
  }
    map->current = -1;
    return NULL;
}

Pair * firstMap(HashMap * map) {
  for(int i = 0; i < map->capacity;i++){
    struct Pair* currentpair = map->buckets[i];
    if(currentpair != NULL && currentpair->key != NULL){
      map->current = i;
      return currentpair;
    }
  }
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for(int i = map->current +1; i < map->capacity;i++){
    struct Pair* currentpair = map->buckets[i];
    if(currentpair != NULL && currentpair->key != NULL){
      map->current = i;
      return currentpair;
    }
  }
  map->current = -1;
  return NULL;
}
