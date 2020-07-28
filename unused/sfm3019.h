#include <gd32vf103_i2c.h>


//SFM3019 ADDRESS & WORLD SIZE
#define SFM3019_ADDRESS    0x2EU
#define SFM3019_WORD_SIZE  0x02U	
//FLAGS
#define I2C_BUSY      0xFFU
#define I2C_ERROR     0x01U
#define STATUS_OK     0x00U
#define I2C_CRC_ERROR 0xFEU 
//COMMANDS
#define SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_O2             0x3603U
#define	SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR            0x3608U
#define SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR_O2_MIX     0x3632U
#define SFM3019_CMD_TRANSMIT_CONCENTRATION                      0xE17DU
#define	SFM3019_RESET_I2C_ADDR_POINTER                          0xE000U
#define SFM3019_CMD_STOP_CONTINUOUS_MEASUREMENT                 0x3FF9U
#define	SFM3019_CMD_CONFIGURE_AVERAGING                         0x366AU
#define SFM3019_CMD_READ_SOF                                    0x3661U 
#define SFM3019_CMD_GENERAL_CALL_RESET                          0x0006U 
#define SFM3019_CMD_ENTER_SLEEP_MODE                            0x3677U
#define SFM3019_CMD_READ_PRODUCT_ID                             0xE102U
//CRC RELATED
#define CRC8_POLYNOMIAL 0x31U
#define CRC8_INIT 0xFFU
#define CRC8_LEN 0x1U
#define CRC8_REFLECT_INPUT FALSE
#define CRC8_REFLECT_OUTPUT FALSE
#define CRC8_FINAL_XOR 0x00U
//STATE FLAGS
#define SFM3019_CONTINUOUS_MEASUREMENT_MODE 0
#define SFM3019_IDLE_MODE                   1
#define SFM3019_SLEEP_MODE                  2
#define SFM3019_STANDART_MODE               3

extern sfm3019_state;

/* sfm3019_write_command: write command to the sensor             */
/* @i2c_periph: I2Cx(x = 1, 2)                                    */
/* @command: sensor command                                       */
/*   \command SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_O2         */
/*   \command SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR        */
/*   \command SFM3019_RESET_I2C_ADDR_POINTER                      */
/*   \command SFM3019_CMD_GENERAL_CALL_RESET                      */
/*   \command SFM3019_CMD_STOP_CONTINUOUS_MEASUREMENT             */
/*   \command SFM3019_CMD_ENTER_SLEEP_MODE                        */
/* @return:                                                       */
/*   \return  I2C_BUSY                                            */
/*   \return  I2C_ERROR                                           */
/*   \return  STATUS_OK                                           */														
uint8_t sfm3019_write_command(uint8_t i2c_periph, uint8_t command);

/* sfm3019_write_command_with_args: write command+arguments+crc to the sensor*/
/* @i2c_periph: I2Cx(x = 1, 2)                                    */
/* @command: sensor command                                       */
/*   \command SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR_O2_MIX */
/*   \command SFM3019_CMD_TRANSMIT_CONCENTRATION                  */
/*   \command SFM3019_CMD_CONFIGURE_AVERAGING                     */
/* @args: pointer to the arguments                                */
/* @return:                                                       */
/*   \return  I2C_BUSY                                            */
/*   \return  I2C_ERROR                                           */
/*   \return  STATUS_OK                                           */
uint8_t sfm3019_write_command_with_args(uint8_t i2c_periph, uint8_t command, uint8_t *args);

/* sfm3019_read_command: send command followed by a repeated start read*/
/* @i2c_periph: I2Cx(x = 1, 2)                                    */
/* @command: sensor command                                       */
/*   \command SFM3019_CMD_READ_PRODUCT_ID                         */
/* @Rxbuffer: pointer to receive data                             */
/* @N_reads: Number of bytes to read                              */
/* @return: even with an error data may have been received        */
/*   \return  I2C_BUSY                                            */
/*   \return  I2C_ERROR                                           */
/*   \return  STATUS_OK                                           */
/*   \return  I2C_CRC_ERROR                                       */
uint8_t sfm3019_read_command(uint8_t i2c_periph, uint8_t command, uint8_t *Rxbuffer, uint8_t N_reads);

/* sfm3019_read_command_with_args: send command+arguments+crc followed by a repeated start to read*/
/* @i2c_periph: I2Cx(x = 1, 2)                                    */
/* @command: sensor command                                       */
/*   \command SFM3019_CMD_READ_SOF                                */
/* @Rxbuffer: pointer to receive data                             */
/* @N_reads: Number of bytes to read                              */
/* @args: pointer to the arguments                                */
/* @return: even with an error data may have been received        */
/*   \return  I2C_BUSY                                            */
/*   \return  I2C_ERROR                                           */
/*   \return  STATUS_OK                                           */
/*   \return  I2C_CRC_ERROR                                       */
uint8_t sfm3019_read_command_with_args(uint8_t i2c_periph, uint8_t command, uint8_t *Rxbuffer, uint8_t N_reads, uint8_t *args);

/* sfm3019_fill_buffer: fill a buffer with command+arguments+crc  */
/* @command: command to be put in the buffer                      */
/* @buff: pointer to the buffer                                   */
/* @args: pointer to the arguments                                */
/* @N_args: Number of arguments (x= 0, 1, 2,...)                  */
/* @return: number of bytes in the buffer                         */
uint8_t sfm3019_fill_buffer(uint8_t command, uint8_t *buff, uint8_t *args, uint8_t N_args);

/* smf3019_generate_crc: generate crc of a word (2 bytes)         */
/* @data: pointer to the word                                     */
/* @return: CRC value of the word pointed at                      */
uint8_t sfm3019_generate_crc(uint8_t *data);

/* smf3019_check_crc: check if crc generated is equal to checksum */
/* @data: pointer to the word                                     */
/* @return:                                                       */
/*   \return STATUS_OK                                            */
/*   \return I2C_CRC_ERROR                                        */
uint8_t sfm3019_check_crc(uint8_t *data, uint8_t checksum);