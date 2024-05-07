#include "stdio.h"
#include "stdlib.h"
const int kPageSize = 4096;
const int kNumPages = 1000;
const int kTotalSize = kPageSize * kNumPages;
int g_page_availabilty[kNumPages];
int g_page_size[kNumPages];
void* base_address = NULL;
int myMallocInit() {
  for (int i = 0; i < kNumPages; i++) {
    g_page_availabilty[i] = 0;
    g_page_size[i] = 0;
  }
  base_address = malloc(kTotalSize);
  if (base_address == NULL) {
    return  -1;
  } 
  return 0;
}

void* myMalloc(int size) {
  int num_pages = size / kPageSize;
  if (size % kPageSize != 0) {
    num_pages++;
  }
  int start_page = -1;
  for (int i = 0; i < kNumPages; i++) {
    if (g_page_availabilty[i] == 0) {
      int j = i;
      while (j < kNumPages && g_page_availabilty[j] == 0) {
        j++;
      }
      if (j - i >= num_pages) {
        start_page = i;
        break;
      }
      i = j;
    }
  }
  if (start_page == -1) {
    return NULL;
  }
  for (int i = start_page; i < start_page + num_pages; i++) {
    g_page_availabilty[i] = 1;
    g_page_size[i] = size;
  }
  return (void*)((char*)base_address + start_page * kPageSize);
}

void myMallocDeinit() {
  free(base_address);
}