#define _DEFAULT_SOURCE

#include "test.h"

#include <unistd.h>

#include "mem_internals.h"
#include "mem.h"
#include "util.h"


void debug(const char *fmt, ...);

static void* test_heap_init();
static void allocate_pages_after_block(struct block_header *last_block);
static struct block_header* get_block_from_contents(void * data);


static const size_t INITIAL_HEAP_SIZE = 10000;


void test1(struct block_header *first_block) {
    debug("Test 1: STARTED...\n");

    void *data = _malloc(1000);
    if (data == NULL) {
        err("Test 1: FAILED. _malloc returned NULL.");
    }

    debug_heap(stdout, first_block);

    if (first_block->is_free != false || first_block->capacity.bytes != 1000) {
        err("Test 1: FAILED. capacity or is_free is not correct.");
    }
    debug("Test 1: PASSED.\n\n");
    // clearing heap before the next test
    _free(data);
}

void test2(struct block_header *first_block) {
    debug("Test 2: STARTED...\n");

    void *data1 = _malloc(1000);
    void *data2 = _malloc(1000);
    if (data1 == NULL || data2 == NULL) {
        err("Test 2: FAILED. _malloc returned NULL.");
    }

    _free(data1);

    debug_heap(stdout, first_block);

    struct block_header *data1_block = get_block_from_contents(data1);
    struct block_header *data2_block = get_block_from_contents(data2);
    if (data1_block->is_free == false) {
        err("Test 2: FAILED. status of 'free' of freed block is taken.");
    }
    if (data2_block->is_free == true) {
        err("Test 2: FAILED. status of 'free' is non freed block is free.");
    }
    debug("Test 2: PASSED.\n\n");
    // clearing heap before the next test
    _free(data1);
    _free(data2);
}

void test3(struct block_header *first_block) {
    debug("Test 3: STARTED...\n");

    void *data1 = _malloc(1000);
    void *data2 = _malloc(1000);
    void *data3 = _malloc(1000);
    if (data1 == NULL || data2 == NULL || data3 == NULL) {
        err("Test 3: FAILED. _malloc returned NULL.");
    }

    _free(data2);
    _free(data1);

    debug_heap(stdout, first_block);

    struct block_header *data1_block = get_block_from_contents(data1);
    struct block_header *data3_block = get_block_from_contents(data3);
    if (data1_block->is_free == false) {
        err("Test 3: FAILED. status of 'free' of FREED block is TAKEN.");
    }
    if (data3_block->is_free == true) {
        err("Test 3: FAILED. status of 'free' is non freed block is free.");
    }
    if (data1_block->capacity.bytes != 2000 + offsetof(struct block_header, contents)) {
        err("Test 3: FAILED. wrong size of block.");
    }
    debug("Test 3: PASSED.\n\n");
    // clearing heap before the next test
    _free(data1);
    _free(data2);
    _free(data3);
}

void test4(struct block_header *first_block) {
    debug("Test 4: STARTED...\n");

    void *data1 = _malloc(10000);
    void *data2 = _malloc(10000);
    void *data3 = _malloc(5000);
    if (data1 == NULL || data2 == NULL || data3 == NULL) {
        err("Test 4: FAILED. _malloc returned NULL.");
    }

    _free(data2);
    _free(data3);

    debug_heap(stdout, first_block);

    struct block_header *data1_block = get_block_from_contents(data1);
    struct block_header *data2_block = get_block_from_contents(data2);
    if ((uint8_t *)data1_block->contents + data1_block->capacity.bytes != (uint8_t*) data2_block){
        err("Test 4: FAILED. _block wasn't created next to first created block.");
    }
    debug("Test 4: PASSED.\n\n");
    // clearing heap before the next test
    _free(data1);
    _free(data2);
    _free(data3);
}

void test5(struct block_header *first_block) {
    debug("Test 5: STARTED...\n");

    void *data1 = _malloc(10000);
    if (data1 == NULL) {
        err("Test 5: FAILED. _malloc returned NULL.");
    }

    struct block_header *addr = first_block;
    while (addr->next != NULL) addr = addr->next;
    allocate_pages_after_block(addr);
    void *data2 = _malloc(100000);

    debug_heap(stdout, first_block);

    struct block_header *data2_block = get_block_from_contents(data2);
    if (data2_block == addr) {
        err("Test 5: FAILED. _created block next to first allocated heap.");
    }
    debug("Test 5: PASSED.\n\n");
    // clearing heap before the next test
    _free(data1);
    _free(data2);
}


void test_all() {
    struct block_header *first_block = (struct block_header*) test_heap_init();
    test1(first_block);
    test2(first_block);
    test3(first_block);
    test4(first_block);
    test5(first_block);
    debug("All tests passed.\n");
}


static void* test_heap_init() {
    debug("Initializing heap...\n");
    void *heap = heap_init(INITIAL_HEAP_SIZE);
    if (heap == NULL) {
        err("Cannot init heap for tests.");
    }
    debug("Heap inited successfully.\n\n");
    return heap;
}

static void allocate_pages_after_block(struct block_header *last_block) {
    struct block_header *addr = last_block;
    void* test_addr = (uint8_t*) addr + size_from_capacity(addr->capacity).bytes;
    test_addr = (uint8_t*) (getpagesize() * ((size_t) test_addr / getpagesize() + (((size_t) test_addr % getpagesize()) > 0)));
    test_addr = map_pages(test_addr, 1000, MAP_FIXED_NOREPLACE);
    debug(test_addr);
}

static struct block_header* get_block_from_contents(void* data) {
    return (struct block_header *) ((uint8_t *) data - offsetof(struct block_header, contents));
}
