
#include "cref/ref.h"
#include "cref/list.h"
#include "cref/types.h"

#ifndef _hashmap_h_
#define _hashmap_h_

#define _hashmap_nodelist_count 100
refstruct(hashmap, {
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


#define hashmap_itr(__m, __k) \
    for(size_t __mi=0;__mi<_hashmap_nodelist_count;__mi++) \
        for(list *__ls=(__m->nodes)[__mi];__ls;__ls=__ls->rest) \
            if (__k=__ls->first)


#endif
