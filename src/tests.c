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
    struct mallinfo init = mallinfo();
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
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

int main() { }
