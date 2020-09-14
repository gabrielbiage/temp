/*
 * bps120.h
 *
 *  Created on: 3 de ago de 2020
 *      Author: mateus
 */

#ifndef BPS120_BPS120_H_
#define BPS120_BPS120_H_

#include <stdint.h>
#define BPS120_I2C_ADDRESS 0x28

typedef struct {
    uint8_t i2c_address;
    uint32_t i2c_bus;//barramento I2C0 ou I2C1
} BpsConfig;

typedef enum
{
	GOOD_PACKET = 0U,
	COMMAND_MODE = 2U,
	STALE_DATA = 3U,
	DIAG_COND = 4U
}bps_status_enum;

BpsConfig bps120_create(uint32_t i2c_bus);
int16_t bps_read_raw(BpsConfig* bps_config, int16_t* pressure_raw, bps_status_enum* status);
float bps_convert_presure_float_psi(int16_t pressure_raw);

#endif /* BPS120_BPS120_H_ */
