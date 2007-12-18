#include "cut.h"
#include <stdlib.h>
#include <stdio.h>

#include "../pq.h"

static pq q;
static job j1, j2, j3a, j3b, j3c;

void
__CUT_BRINGUP__pq()
{
    /* When CUT 3.0 comes out it will fix this design flaw. For now we will
     * just leak some queues during test. */
    /*q = make_pq(2, job_pri_cmp);*/
    j1 = make_job(1, 0, 1, 0);
    j2 = make_job(2, 0, 1, 0);
    j3a = make_job(3, 0, 1, 0);
    j3b = make_job(3, 0, 1, 0);
    j3c = make_job(3, 0, 1, 0);
    /*ASSERT(!!q, "Allocation should work");*/
    ASSERT(!!j1, "Allocation should work");
    ASSERT(!!j2, "Allocation should work");
    ASSERT(!!j3a, "Allocation should work");
    ASSERT(!!j3b, "Allocation should work");
    ASSERT(!!j3c, "Allocation should work");
}

void
__CUT__pq_test_empty_queue_should_have_no_items()
{
    q = make_pq(2, job_pri_cmp);
    ASSERT(q->used == 0, "q should be empty.");
}

void
__CUT__pq_test_insert_one()
{
    q = make_pq(2, job_pri_cmp);
    pq_give(q, j1);
    ASSERT(q->used == 1, "q should contain one item.");
}

void
__CUT__pq_test_insert_and_remove_one()
{
    int r;
    job j;

    q = make_pq(2, job_pri_cmp);
    r = pq_give(q, j1);
    ASSERT(r, "insert should succeed");

    j = pq_take(q);
    ASSERT(j == j1, "j1 should come back out");
    ASSERT(q->used == 0, "q should be empty.");
}

void
__CUT__pq_test_priority()
{
    int r;
    job j;

    q = make_pq(3, job_pri_cmp);
    r = pq_give(q, j2);
    ASSERT(r, "insert should succeed");

    r = pq_give(q, j3a);
    ASSERT(r, "insert should succeed");

    r = pq_give(q, j1);
    ASSERT(r, "insert should succeed");

    j = pq_take(q);
    ASSERT(j == j1, "j1 should come out first.");

    j = pq_take(q);
    ASSERT(j == j2, "j2 should come out second.");

    j = pq_take(q);
    ASSERT(j == j3a, "j3a should come out third.");
}

void
__CUT__pq_test_fifo_property()
{
    int r;
    job j;

    q = make_pq(3, job_pri_cmp);
    r = pq_give(q, j3a);
    ASSERT(r, "insert should succeed");
    ASSERT(q->heap[0] == j3a, "j3a should be in pos 0");

    r = pq_give(q, j3b);
    ASSERT(r, "insert should succeed");
    ASSERT(q->heap[1] == j3b, "j3b should be in pos 1");

    r = pq_give(q, j3c);
    ASSERT(r, "insert should succeed");
    ASSERT(q->heap[2] == j3c, "j3c should be in pos 2");

    j = pq_take(q);
    ASSERT(j == j3a, "j3a should come out first.");

    j = pq_take(q);
    ASSERT(j == j3b, "j3b should come out second.");

    j = pq_take(q);
    ASSERT(j == j3c, "j3c should come out third.");
}

#define HOW_MANY 20
void
__CUT__pq_test_many_jobs()
{
    unsigned int last_pri;
    int r, i;
    job j;

    q = make_pq(HOW_MANY, job_pri_cmp);

    for (i = 0; i < HOW_MANY; i++) {
        j = make_job(1 + rand() % 8192, 0, 1, 0);
        ASSERT(!!j, "allocation");
        r = pq_give(q, j);
        ASSERT(r, "insert should succeed");
    }

    last_pri = 0;
    for (i = 0; i < HOW_MANY; i++) {
        j = pq_take(q);
        /*printf("j->pri=%d last_pri=%d\n", j->pri, last_pri);*/
        ASSERT(j->pri >= last_pri, "should come out in order.");
        last_pri = j->pri;
    }
}

void
__CUT__pq_test_find_match()
{
    job j;

    q = make_pq(2, job_pri_cmp);
    pq_give(q, j1);

    j = pq_find(q, j1->id);
    ASSERT(j == j1, "j1 should come out.");
}

void
__CUT__pq_test_find_miss()
{
    job j;

    q = make_pq(2, job_pri_cmp);
    pq_give(q, j1);

    j = pq_find(q, j1->id + 1);
    ASSERT(j == NULL, "no job should match.");
}

void
__CUT_TAKEDOWN__pq()
{
    free(j1);
    free(j2);
    free(j3a);
    free(j3b);
    free(j3c);
    free(q);
}

