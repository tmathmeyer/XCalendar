
#include "cref/ref.h"
#include "cref/list.h"
#include "cref/types.h"

#ifndef _hashmap_h_
#define _hashmap_h_

#define _hashmap_nodelist_count 100
refstruct(hashmap, {
    list *iterator_head;
    list *nodes[_hashmap_nodelist_count];
    size_t occupancy;
});

refstruct(key_value_pair, {
    void *key;
    void *value;
});

hashmap *_hashmap(void);
key_value_pair *_key_value_pair(void *, void *);
void hashmap_destructor(hashmap *);

void *hashmap_put(hashmap *, void *, void *);
void *hashmap_get(hashmap *, void *);
void *hashmap_remove(hashmap *, void *);

#endif
