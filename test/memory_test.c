#include "../inc/memory.hpp"
#include <assert.h>

void test_byte();
void test_word();

int main() {
  init_mem();

 test_byte();
 test_word();
}

void test_byte() {
  uint8_t a = 24;
  uint8_t b = 0;
  write_byte(5000,a);
  read_byte(5000);

  assert(b == a);
}


