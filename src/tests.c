#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "test.h"
#include "hashmap.h"
#include "cref/ref.h"
#include "cref/types.h"
#include "cref/list.h"

void ppr(void *r) {
    printf("0x%x: %i\n", r, R(r));
}

START_TEST(hashmap_basics) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_add) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        ASSERT_REF(
                hashmap_put(map, _string("hello", false), _string("world", false))
                ,NULL
                ,"something came out of the hashmap!");
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_simple_itr) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        scoped string *key = S(_string("hello", 0));
        scoped string *value = S(_string("world", 0));
        ASSERT_REF(hashmap_put(map, key, value),NULL,"add should return NULL");

        size_t count = 0;
        key_value_pair *kvp = NULL;
        hashmap_itr(map, kvp) {
            printf("-");
            count++;
        }

        ASSERT_REF(count, 1, "should be one element in hashmap");
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_triple_itr) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());

        scoped string *Akey = S(_string("hello", 0));
        scoped string *Avalue = S(_string("world", 0));

        scoped string *Bkey = S(_string("this", 0));
        scoped string *Bvalue = S(_string("is", 0));

        scoped string *Ckey = S(_string("a", 0));
        scoped string *Cvalue = S(_string("test", 0));

        ASSERT_REF(hashmap_put(map, Akey, Avalue),NULL,"add should return NULL");
        ASSERT_REF(hashmap_put(map, Bkey, Bvalue),NULL,"add should return NULL");
        ASSERT_REF(hashmap_put(map, Ckey, Cvalue),NULL,"add should return NULL");

        size_t count = 0;
        key_value_pair *kvp = NULL;
        hashmap_itr(map, kvp) {
            count++;
        }
        ASSERT_REF(count, 3, "should be three elements in hashmap");
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_add_remove_itr) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());

        scoped string *Akey = S(_string("hello", 0));
        scoped string *Avalue = S(_string("world", 0));

        scoped string *Bkey = S(_string("this", 0));
        scoped string *Bvalue = S(_string("is", 0));

        scoped string *Ckey = S(_string("a", 0));
        scoped string *Cvalue = S(_string("test", 0));

        ASSERT_REF(hashmap_put(map, Akey, Avalue),NULL,"add should return NULL");
        ASSERT_REF(hashmap_put(map, Bkey, Bvalue),NULL,"add should return NULL");
        ASSERT_REF(hashmap_put(map, Ckey, Cvalue),NULL,"add should return NULL");

        void *d = hashmap_remove(map, Bkey);
        L(d);

        size_t count = 0;
        key_value_pair *kvp = NULL;
        hashmap_itr(map, kvp) {
            count++;
        }
        ASSERT_REF(count, 2, "should be three elements in hashmap");
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_cant_add) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        scoped list *nums = numeric_range(10, 20);
        ASSERT_REF(
                hashmap_put(map, nums, _string("world", false))
                ,(void *)1
                ,"hashmap insert didnt return 1");
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_add_dupes) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        scoped string *value1 = S(_string("test A: value1", false));
        scoped string *value2 = S(_string("test B: value2", false));
        void *should_be_null
            = hashmap_put(map, _string("hello", false), value1);
        ASSERT_REF(should_be_null, NULL, "inserting into empty map not null");
        void *should_be_value1
            = hashmap_put(map, _string("hello", false), value2);
        ASSERT_REF(should_be_value1, value1, "hashmap not returning ptr to old val");
        L(value1);
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();

}

START_TEST(hashmap_retrieve) {
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        scoped string *value1 = S(_string("test A: value1", false));
        scoped string *value2 = S(_string("test B: value2", false));
        void *should_be_null
            = hashmap_put(map, _string("hello", false), value1);
        ASSERT_REF(should_be_null, NULL, "inserting into empty map not null");
        void *should_be_value1
            = hashmap_put(map, _string("hello", false), value2);
        ASSERT_REF(should_be_value1, value1, "hashmap not returning ptr to old val");
        void *should_be_value2
            = hashmap_get(map, _string("hello", false));
        ASSERT_REF(should_be_value2, value2, "hashmap not getting correct ptr");
        L(should_be_value1);
        L(should_be_value2);
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(hashmap_removal) {
    {
        scoped hashmap *map = S(_hashmap());
        scoped string *value1 = S(_string("test A: value1", false));
        void *should_be_null
            = hashmap_put(map, _string("hello", false), value1);
        ASSERT_REF(should_be_null, NULL, "inserting into empty map not null");
        void *should_be_value1 = hashmap_remove(map, _string("hello", false));
        ASSERT_REF(should_be_value1, value1, "hashmap not returning ptr to old val");
        L(value1);
    }
    ASSERT_SUCCESS();
}

START_TEST(hashmap_many_op) {
    char *seed = "abcdefghijklmnopqrstuvwxyz";
    char *t = seed;
    string *AA;
    hashmap *a = NULL;
    struct mallinfo init = mallinfo();
    {
        scoped hashmap *map = S(_hashmap());
        a = map;
        while(*t) {
            char *strA = malloc(2 * sizeof(char));
            char *strB = malloc(2 * sizeof(char));
            strA[0] = strB[0] = t[0];
            strA[1] = strB[1] = 0;
            string *a = _string(strA, 1);
            string *b = _string(strB, 1);
            void *ret = hashmap_put(map, a, b);
            AA = a;
            t++;
        }

        key_value_pair *v = NULL;
        hashmap_itr(map, v) {
            string *s = v->value;
            printf("%s : %u\n", s->str, s->hashcode(s));
        }

        L(map);
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

int main() { }
