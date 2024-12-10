#ifndef BASICS_H__
#define BASICS_H__

#ifdef __cplusplus

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cctype>
#include <queue>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>

#else

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#endif

#include <getopt.h>

#ifdef _WIN64
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define CREATE_OBJ(type) ((type *) malloc(sizeof(type)))

#define FREE_EVENT_BODY(event) do {\
  if (event->event_body)\
    free(event->event_body); \
} while(0)

#define FREE_EVENT(event) do {\
  if (event->event_body)\
    free(event->event_body); \
  free(event);\
} while(0)

#endif