#include "binary_counter.h"

#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <stdbool.h>

#define NUM_OF_BITS 4

// Encodes 2^n.
#define EXP_2(n) (1 << n)

int main (void)
{
  initialize_pins();

  intro();

  auto_mode();

  return EXIT_SUCCESS;
}

void initialize_pins(void) {
  if (wiringPiSetup () == -1)
  {
    perror("Failed to setup wiringPi library");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < NUM_OF_BITS; ++i) {
    pinMode(pins[i], OUTPUT);
  }
}

void intro(void) {
  printf("%s", "Welcome to the binary counter!.\n");
  printf("%s", "The counter will start at 0 and increment every second. \n");
  printf("%s", "The GPIO pins 14, 15, 18, 23 represent the 1st to 4th binary "
               "bit.\n");
}

void auto_mode(void) {
  int count = 0;
  for (;;)
  {
    process_number((count++) % EXP_2(NUM_OF_BITS));
    delay(1000);
  }
}
void pin_on(int pin) {
//  printf("%s%u%s\n", "PIN ", pin, " ON");
  digitalWrite(pins[pin], HIGH);
}

void pin_off(int pin) {
//  printf("%s%u%s\n", "PIN ", pin, " OFF");
  digitalWrite(pins[pin], LOW);
}


// Prints and set pins accordingly.
void process_number(int number) {
  printf("%s%u%s", "In decimal: ", number, ". In binary: ");
  for (int i = NUM_OF_BITS - 1; i >= 0; --i) {
    bool set_pin = number & EXP_2(i) ? 1 : 0;
    set_pin ? pin_on(i) : pin_off(i);
    printf("%d", set_pin);
  }
  printf("\n");
}