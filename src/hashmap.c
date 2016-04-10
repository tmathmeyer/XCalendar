#include <stdio.h>

#include "cref/ref.h"
#include "cref/list.h"
#include "hashmap.h"

hashmap *_hashmap(void) {
    hashmap *result = ref_malloc(sizeof(hashmap), 1);
    result->iterator_head = NULL;
    result->occupancy = 0;
    result->destructor = hashmap_destructor;
    for(size_t i=0;i<_hashmap_nodelist_count;i++) {
        (result->nodes)[i] = NULL;
    }
    return result;
}

key_value_pair *_key_value_pair(void *a, void *b) {
    key_value_pair *result = ref_malloc(sizeof(key_value_pair), 2);
    result->key = S(a);
    result->value = S(b);
    result->destructor = NULL;
    return result;
}

void hashmap_destructor(hashmap *_hashmap) {
    hashmap *map = (hashmap *) _hashmap;
    for(size_t i=0;i<_hashmap_nodelist_count;i++) {
        L((map->nodes)[i]);
    }
}

void *hashmap_put(hashmap *map, void *key, void *value) {
    S(key); S(value);
    generic_type *k = (generic_type *)key;
    if (k->hashcode == NULL || k->equals == NULL) {
        L(key); L(value);
        return (void *)0x01;
    }
    ssize_t hashcode = (k->hashcode)(k) % 100;
    
    list *insert_in_array = (map->nodes)[hashcode];
    while(insert_in_array) {
        key_value_pair *each = (key_value_pair *)insert_in_array->first;
        generic_type *e_key = (generic_type *)each->key;
        if (k->equals(k, e_key)) {
            void *ret = each->value;
            each->value = value;
            L(key);
            return ret;
        }
        insert_in_array = insert_in_array->rest;
    }
    
    key_value_pair *kvp = _key_value_pair(key, value);
    (map->nodes)[hashcode] = S(_list(kvp, insert_in_array));
    L(insert_in_array);
    L(key); L(value);
    return NULL;
}

void *hashmap_get(hashmap *map, void *key) {
    S(key);
    generic_type *k = (generic_type *)key;
    if (k->hashcode == NULL || k->equals == NULL) {
        L(key);
        return (void *)0x00;
    }
    ssize_t hashcode = (k->hashcode)(k) % 100;
    
    list *insert_in_array = (map->nodes)[hashcode];
    while(insert_in_array) {
        key_value_pair *each = (key_value_pair *)insert_in_array->first;
        generic_type *e_key = (generic_type *)each->key;
        if (k->equals(k, e_key)) {
            L(key);
            return S(each->value);
        }
        insert_in_array = insert_in_array->rest;
    }
    L(key);
    return NULL;
}

void *hashmap_remove(hashmap *map, void *key) {
    S(key);
    generic_type *k = (generic_type *)key;
    if (k->hashcode == NULL || k->equals == NULL) {
        L(key);
        return (void *)0x00;
    }
    ssize_t hashcode = (k->hashcode)(k) % 100;
    
    list *previous = NULL;
    list *insert_in_array = (map->nodes)[hashcode];
    while(insert_in_array) {
        key_value_pair *each = (key_value_pair *)insert_in_array->first;
        generic_type *e_key = (generic_type *)each->key;
        if (k->equals(k, e_key)) {
            if (previous == NULL) {
                (map->nodes)[hashcode] = insert_in_array->rest;
            } else {
                previous->rest = insert_in_array->rest;
            }
            void *result = S(each->value);

            L(insert_in_array);
            L(key);

            return result;
        }
        previous = insert_in_array;
        insert_in_array = insert_in_array->rest;
    }
    L(key);
    return NULL;
}
