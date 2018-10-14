#include "../inc/memory.hpp"
#include <assert.h>
#include <iostream>

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
  b = read_byte(5000);

  assert(b == a);
}

void test_word() {
  uint16_t a = 32768 + 128;
  uint16_t b = 0;
  uint8_t c1 = 0;
  uint8_t c2 = 0;
  write_word(5000,a);
  c1 = read_byte(5000);
  c2 = read_byte(5001);
  b = read_word(5000);
  std::cout << "c1: " << unsigned(c1) << " c2: " << unsigned(c2) << " b: " << b << std::endl; 
  assert(c1 == 128);
  assert(c2 == 128);
  assert(b == (32768+128));
}


