

#include "gd32vf103.h"
#include "gd32vf103_i2c.c"

//sensor address:
#define	SFM3019_ADDR	0x2EU

//continuous measurement modes:
#define SFM3019_COMMAND_GAS_O2 		0x3603U
#define SFM3019_COMMAND_GAS_AIR 	0x3608U
#define SFM3019_COMMAND_GAS_O2_AIR	0x3632U

//Update concentrations commands
#define SFM3019_COMMAND_UPDATE_C	0xE17DU
#define SFM3019_COMMAND_RESETPTR	0xE000U

//stop continuous mesurements
#define SFM3019_COMMAND_STOP_CM 	0x3FF9U

//configure average
#define SFM3019_COMMAND_AVRG_CONFIG	0x366AU

//read: Scalar factor, Offset, Flow unit
#define SFM3019_COMMAND_READ_SOF	0x3661U

//general call reset
#define SFM3019_COMMAND_RESETCALL 	0x0006U 

//sleep mode
#define SFM3019_COMMAND_SLEEP_MODE	0x3677U

//read product id
#define SFM3019_COMMAND_READ_ID		0xE102


void sfm3019_start_continuous_measurement(uint32_t i2c_periph, uint16_t mode);
void sfm3019_config_concentration(uint32_t i2c_periph, uint8_t value);
void sfm3019_i2cptr_reset(uint32_t i2c_periph);
void sfm3019_stop_continuous_measurement(uint32_t i2c_periph);
void sfm3019_config_averaging(uint32_t i2c_periph, uint8_t N_value);
void sfm3019_read_SOF(uint32_t i2c_periph, uint8_t gas_code);
void sfm3019_generalcall_reset(uint32_t i2c_periph);
void sfm3019_SleepMode_enable(uint32_t i2c_periph);
void sfm3019_read_productID(uint32_t i2c_periph);
uint8_t CRC(uint16_t value);


struct typedef{
	uint16_t Scalar_factor;
	uint16_t Offset;
	uint16_t Flow_unit;

}SOF;