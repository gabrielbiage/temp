
#include <stdint.h>
#include "gd32f10x_libopt.h"
#include "sensirion_common.h"

#define I2C0_OWN_ADDRESS7      0x72
#define I2C1_OWN_ADDRESS7      0x72

void my_i2c_init(void);
void my_i2c_release(void);
int8_t my_i2c_read(uint32_t bus, uint8_t address, uint8_t *data, uint16_t count);
int8_t my_i2c_write(uint32_t bus, uint8_t address, const uint8_t *data, uint16_t count);
