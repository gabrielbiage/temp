/*
 * bps120.c
 *
 *  Created on: 3 de ago de 2020
 *      Author: mateus
 */

#include "bps120.h"
#include "i2c_implementation.h"

BpsConfig bps120_create(uint32_t i2c_bus){
	BpsConfig bps_config = {
        BPS120_I2C_ADDRESS,
		i2c_bus
    };
    return bps_config;
}

int16_t bps_read_raw(BpsConfig* bps_config, int16_t* pressure_raw, bps_status_enum* status){
	uint8_t data;
	uint8_t error = my_i2c_read(bps_config->i2c_bus, bps_config->i2c_address, &data, 2);
	if (error){
		return STATUS_FAIL;
	}
	*pressure_raw = data|0x3FFF;
	*status = data>>14;

	return 0;
}

// retorna pressão em psi
float bps_convert_presure_float_psi(int16_t pressure_raw){
	return (1.-0.15)*(pressure_raw-0.1*16384)/(0.8*16384)+0.15;
}


