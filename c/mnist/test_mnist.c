


#include "mnist.h"

#include <CUnit/Basic.h>

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>

#include <gmp.h>

// For certain tests, COUNT^2 (binary ops) are tested
#define COUNT 100


// Largest number generated for the test (in digits) *before*
// performing operation.
#define VERY_LARGE 800


#define MAX(a,b) ((a) > (b) ? (a) : (b))


/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int init_suite(void)
{
   return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int clean_suite(void)
{
   return 0;
}



static void test_open()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 5},
      {"t10k", 7},
   };
   //check if the label of the first image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);

      mnist_free(x);
   }
}


static void test_count()
{
  struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 60000},
      {"t10k", 10000},
   };
   //check if the number of images matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; int a;
      x=mnist_open(tests[c].s);
      a=mnist_image_count(x)
      CU_ASSERT_EQUAL(a, tests[c].s2);

      mnist_free(x);
   }
}

static void test_size()
{
   struct E
   {
      const char * s;
      const int s2;
      const int s3;
   };
   struct E tests[] = {
      {"train", 28,28},
      {"t10k", 28,28},
   };
   //check if the number of images matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; unsigned int a,b;
      x=mnist_open(tests[c].s);
      mnist_image_size(x,&a,&b)
      CU_ASSERT_EQUAL(a,tests[c].s2);
      CU_ASSERT_EQUAL(b,tests[c].s3);

      mnist_free(x);
   }
}

static void test_begin()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 5},
      {"t10k", 7},
   };
   //check if the label of the first image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);

      mnist_free(x);
   }
}

static void test_data()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 5},
      {"t10k", 7},
   };
   //check if the label of the first image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);

      mnist_free(x);
   }
}

static void test_label()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 5},
      {"t10k", 7},
   };
   
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);

      mnist_free(x);
   }
}

static void test_set_label()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 7},
      {"t10k", 5},
   };
   
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      if(mnist_set_image_label(h,tests[c].s2)>=0){
         a=mnist_image_label(h);
         CU_ASSERT_EQUAL(a, tests[c].s2);
      }
      
      mnist_free(x);
   }
}


static void test_next()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 0},
      {"t10k", 2},
   };
   //check if the label of the second image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      h=mnist_image_next(h);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);
      
      mnist_free(x);
   }
}

static void test_previous()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 5},
      {"t10k", 7},
   };
   //check if the label of the second image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      h=mnist_image_next(h);
      h=mnist_image_prev(h);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);
      
      mnist_free(x);
   }
}


static void test_add_after()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 0},
      {"t10k", 0},
   };
   //check if the label of the newly added image matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;const unsigned char *b; int a;
      unsigned int c,d;
      x=mnist_open(tests[c].s);
      mnist_image_size(x,&c,&d)
      h=mnist_image_begin(x);
      b=mnist_image_data(h);
      h=mnist_image_add_after(x,h,b,c,d);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);
      
      mnist_free(x);
   }
}


static void test_remove()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train", 0},
      {"t10k", 2},
   };
   //check if the label of the image after the removed one matches.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h; int a;
      x=mnist_open(tests[c].s);
      h=mnist_image_begin(x);
      h=mnist_image_remove(x,h);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s2);
      
      mnist_free(x);
   }
}

static void test_create()
{
   struct E
   {
      const int s2;
      const int s3;
   };
   struct E tests[] = {
      {28, 28},
      {1, 1},
   };
   
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h; int a;
      unsigned int c,d;
      x=mnist_create(tests[c].s2,tests[c].s3);
      mnist_image_size(x,&c,&d);
      CU_ASSERT_EQUAL(c,tests[c].s2);
      CU_ASSERT_EQUAL(d,tests[c].s3);
      
      mnist_free(x);
   }
}

static void test_save()
{
   struct E
   {
      const char * s;
      const char * s2;
      const int s3;
   };
   struct E tests[] = {
      {"train", "test1",5},
      {"t10k", "test2",7},
   };
   //open the saved file and test the label of the first image.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h; int a;
      x=mnist_open(tests[c].s);
      if(mnist_save(x,tests[c].s2)==true){
         x=mnist_open(tests[c].s2);
         h=mnist_image_begin(x);
         a=mnist_image_label(h);
         CU_ASSERT_EQUAL(a, tests[c].s3);
      }     
      
      mnist_free(x);
   }
}

static void test_open_sample()
{
   struct E
   {
      const char * s;
      const int s2;
   };
   struct E tests[] = {
      {"train",50},
      {"t10k",50},
   };
   
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x; mnist_image_handle h;int a;
      x=mnist_open_sample(tests[c].s);
      a=mnist_image_count(x)
      CU_ASSERT_EQUAL(a, tests[c].s2);
      
      mnist_free(x);
   }
}

static void test_free()
{
   struct E
   {
      const char * s;
      const char * s2;
      const int s3;
   };
   struct E tests[] = {
      {"train","t10k",7},
      {"t10k","train",5},
   };
   //check if mnist_free only frees what we want it to free.
   for (unsigned int c=0; c<sizeof(tests)/sizeof(tests[0]); ++c)
   {
      mnist_dataset_handle x,y; mnist_image_handle h;int a;
      x=mnist_open_sample(tests[c].s);y=mnist_open_sample(tests[c].s2);
      mnist_free(x);
      h=mnist_image_begin(x);
      a=mnist_image_label(h);
      CU_ASSERT_EQUAL(a, tests[c].s3);
      
      mnist_free(y);
   }
 
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   srand(time(NULL));
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
   if (NULL == pSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
       (NULL == CU_add_test(pSuite, "mnist_open()", test_open))
    || (NULL == CU_add_test(pSuite, "mnist_image_count()", test_count))
    || (NULL == CU_add_test(pSuite, "mnist_set_image_label", test_set_label))
    || (NULL == CU_add_test(pSuite, "mnist_image_label()", test_label))
    || (NULL == CU_add_test(pSuite, "mnist_free()", test_free))
    || (NULL == CU_add_test(pSuite, "mnist_image_size()", test_size))
    || (NULL == CU_add_test(pSuite, "mnist_image_begin()", test_begin))
    || (NULL == CU_add_test(pSuite, "mnist_image_data()", test_data))
    || (NULL == CU_add_test(pSuite, "mnist_open_sample", test_open_sample))
    || (NULL == CU_add_test(pSuite, "mnist_image_next", test_next))
    || (NULL == CU_add_test(pSuite, "mnist_image_prev", test_previous))
    || (NULL == CU_add_test(pSuite, "mnist_image_add_after()", test_add_after))
    || (NULL == CU_add_test(pSuite, "mnist_image_remove()", test_remove))
    || (NULL == CU_add_test(pSuite, "mnist_create()", test_create))
    || (NULL == CU_add_test(pSuite, "mnist_save()", test_save))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}


