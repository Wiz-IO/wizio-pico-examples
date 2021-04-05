#include <Arduino.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void *perform_work(void *arguments)
{
  int index = *((int *)arguments);
  int sleep_time = 1 + rand() % NUM_THREADS;
  printf("THREAD %d: Started.\n", index);
  printf("THREAD %d: Will be sleeping for %d seconds.\n", index, sleep_time);
  sleep(sleep_time);
  printf("THREAD %d: Ended.\n", index);
  return NULL;
}

int test_pthread(void)
{
  printf("PTHREAD EXAMPLE https://en.wikipedia.org/wiki/POSIX_Threads\n\n");
  pthread_t threads[NUM_THREADS];
  int thread_args[NUM_THREADS];
  int i;
  int result_code;

  //create all threads one by one
  for (i = 0; i < NUM_THREADS; i++)
  {
    printf("IN MAIN: Creating thread %d.\n", i);
    thread_args[i] = i;
    result_code = pthread_create(&threads[i], NULL, perform_work, &thread_args[i]);
    assert(!result_code);
  }

  printf("IN MAIN: All threads are created.\n");

  //wait for each thread to complete
  for (i = 0; i < NUM_THREADS; i++)
  {
    result_code = pthread_join(threads[i], NULL);
    assert(!result_code);
    printf("IN MAIN: Thread %d has ended.\n", i);
  }

  printf("MAIN program has ended.\n");
  return 0;
}

void led(int _delay)
{
  static int led = 0;
  digitalWrite(LED, led);
  led ^= 1;
  delay(_delay);
}

void setup()
{
  Serial.begin(115200, true);
  printf("\n\nArdiuno Raspberrypi PI Pico 2021 Georgi Angelov\n");
  pinMode(LED, OUTPUT);
  test_pthread();
}

void loop()
{
  led(500);
  Serial.print("loop ");
}

/*
Ardiuno Raspberrypi PI Pico 2021 Georgi Angelov
PTHREAD EXAMPLE https://en.wikipedia.org/wiki/POSIX_Threads

IN MAIN: Creating thread 0.
IN MAIN: Creating thread 1.
IN MAIN: Creating thread 2.
IN MAIN: Creating thread 3.
IN MAIN: Creating thread 4.
IN MAIN: All threads are created.
THREAD 0: Started.
THREAD 0: Will be sleeping for 4 seconds.
THREAD 1: Started.
THREAD 1: Will be sleeping for 4 seconds.
THREAD 2: Started.
THREAD 2: Will be sleeping for 3 seconds.
THREAD 3: Started.
THREAD 3: Will be sleeping for 5 seconds.
THREAD 4: Started.
THREAD 4: Will be sleeping for 1 seconds.
THREAD 4: Ended.
THREAD 2: Ended.
THREAD 0: Ended.
IN MAIN: Thread 0 has ended.
THREAD 1: Ended.
IN MAIN: Thread 1 has ended.
IN MAIN: Thread 2 has ended.
THREAD 3: Ended.
IN MAIN: Thread 3 has ended.
IN MAIN: Thread 4 has ended.
MAIN program has ended.
loop loop loop loop loop loop loop loop loop...
*/