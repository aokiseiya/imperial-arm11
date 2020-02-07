#ifndef ARM11_35_BINARY_COUNTER_H
#define ARM11_35_BINARY_COUNTER_H

// wiringPi library uses different numbering of pins.
// These are equivalent to GPIO pins 14, 15, 18, 23.
const int pins[] = {15, 16, 1, 4};

void initialize_pins(void);

void intro(void);

void auto_mode(void);

void pin_on(int pin);

void pin_off(int pin);

void process_number(int number);

#endif //ARM11_35_BINARY_COUNTER_H

