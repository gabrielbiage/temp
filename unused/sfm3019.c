#include "sfm3019.h"

uint8_t sfm3019_state = SFM3019_STANDART_MODE;

uint8_t sfm3019_write_command(uint8_t i2c_periph, uint8_t command){
    /* Check if i2c line is busy or CM mode is set and disables the command */
    if (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
    	return I2C_BUSY;
    }
    if (command != SFM3019_RESET_I2C_ADDR_POINTER && SFM3019_state == SFM3019_CONTINUOUS_MEASUREMENT_MODE){
    	return I2C_BUSY;
    }

    uint8_t *buff = (uint8_t *) malloc(sizeof(uint8_t) * 8);
    uint8_t temp;

    /* fill buffer to send data */
    temp = sfm3019_fill_buffer(command, buff, 0, 0);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    /* send data */
    for (int i = 0; i < temp; i++){
    	/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		/*send command */
		i2c_data_transmit(I2C0, *buff);

		buff++;

    }

    /* wait until all data is sent */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);

    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    /* update sensor state */
    switch(command){
    	case SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_O2:
    		sfm3019_state = SFM3019_CONTINUOUS_MEASUREMENT_MODE;
    		break;
    	case SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR:
    		sfm3019_state = SFM3019_CONTINUOUS_MEASUREMENT_MODE;
    		break;
    	case SFM3019_CMD_ENTER_SLEEP_MODE:
    		sfm3019_state = SFM3019_SLEEP_MODE
    		break;
    	case SFM3019_CMD_STOP_CONTINUOUS_MEASUREMENT:
    		sfm3019_state = SFM3019_STANDART_MODE;
    		break;
    	default:
    		break;
    }

    return STATUS_OK;
}

uint8_t sfm3019_write_command_with_args(uint8_t i2c_periph, uint8_t command, uint8_t *args){
    /* Check if i2c line is busy or CM mode is set and disables the command */
    if (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
		return I2C_BUSY;
    }
    if (command != SFM_CMD_TRANSMIT_CONCENTRATION && SFM3019_state == SFM3019_CONTINUOUS_MEASUREMENT_MODE){
		return I2C_BUSY;
	}

    uint8_t *buff = (uint8_t *) malloc(sizeof(uint8_t) * 8);
    uint8_t temp;
    uint8_t N_args = 2;

    /* fill buffer to send data */
    temp = sfm3019_fill_buffer(command, buff, args, N_args);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    /* send data */
    for (int i = 0; i < temp; i++){
    	/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		/*send command */
		i2c_data_transmit(I2C0, *buff);

		buff++;

    }

    /* wait until all data is sent */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);

    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    /* update sensor state */
    switch(command){
    	case SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR_O2_MIX:
    		sfm3019_state = SFM3019_CONTINUOUS_MEASUREMENT_MODE;
    		break;
    	default:
    		break;
    }

    return STATUS_OK;
}

uint8_t sfm3019_read_command(uint8_t i2c_periph, uint8_t command, uint8_t *Rxbuffer, uint8_t N_reads){
    /* Check if i2c line is busy or CM mode is set and disables the command */
    if (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
    	return I2C_BUSY;
    }
    if (sfm3019_state == SFM3019_CONTINUOUS_MEASUREMENT_MODE){
    	return I2C_BUSY;
    }

    uint8_t buffer[8];
    uint8_t *buff = buffer;
    uint8_t temp;

    /* fill buffer to send data */
    temp = sfm3019_fill_buffer(command, buff, 0, 0);

	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    /* send data */
    for (int i = 0; i < temp; i++){
    	/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		/*send command */
		i2c_data_transmit(I2C0, *buff);

		buff++;

    }

    /* wait until all data is sent */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a (repeated) start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_RECEIVER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND flag and read data */
    if (N_reads == 1){
    	/* disable acking */
    	i2c_ack_config(I2C0, I2C_ACK_DISABLE);

    	/* clear ADDSEND flag */
    	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    	/* wait for RXNE flag */
    	while(!i2c_flag_get(I2C0, I2C_FLAG_RXNE));

    	/* generate stop condition */
    	i2c_stop_on_bus(I2C0);

    	/* read data */
    	*(Rxbuffer) = i2c_data_receive(I2C0);
    }
    else if (N_reads > 1){
    	/* clear ADDSEND flag */
    	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    	for (int i = N_reads; i > 0; i--){
    		/* wait for RXNE flag */
    		while(!i2c_flag_get(I2C0, I2C_FLAG_RXNE));

    		if (i == 2){
    			/* disable acking */
    			i2c_ack_config(I2C0, I2C_ACK_DISABLE);

    			/* generate stop condition */
    			i2c_stop_on_bus(I2C0);
    		}

    		/* read data */
    		*(Rxbuffer) = i2c_data_receive(I2C0);

    		Rxbuffer++;
    	}
    }

    /* enable acking */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return STATUS_OK;

}

uint8_t sfm3019_read_command_with_args(uint8_t i2c_periph, uint8_t command, uint8_t *Rxbuffer, uint8_t N_reads, uint8_t *args){
    /* Check if i2c line is busy or CM mode is set and disables the command */
    if (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
    	return I2C_BUSY;
    }
    if (sfm3019_state == SFM3019_CONTINUOUS_MEASUREMENT_MODE){
    	return I2C_BUSY;
    }
    uint8_t buffer[8];
    uint8_t *buff = buffer;
    uint8_t temp;
    uint8_t N_args = 2;

    /* fill buffer to send data */
    temp = sfm3019_fill_buffer(command, buff, args, N_args);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    /* send data */
    for (int i = 0; i < temp; i++){
    	/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		/*send command */
		i2c_data_transmit(I2C0, *buff);

		buff++;

    }

    /* wait until all data is sent */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a (repeated) start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_RECEIVER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND flag and read data */
    if (N_reads == 1){
    	/* disable acking */
    	i2c_ack_config(I2C0, I2C_ACK_DISABLE);

    	/* clear ADDSEND flag */
    	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    	/* wait for RXNE flag */
    	while(!i2c_flag_get(I2C0, I2C_FLAG_RXNE));

    	/* generate stop condition */
    	i2c_stop_on_bus(I2C0);

    	/* read data */
    	*(Rxbuffer) = i2c_data_receive(I2C0);
    }
    else if (N_reads > 1){
        uint8_t count = 0;
    	/* clear ADDSEND flag */
    	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    	for (int i = N_reads; i > 0; i--){
    		/* wait for RXNE flag */
    		while(!i2c_flag_get(I2C0, I2C_FLAG_RXNE));

    		if (i == 2){
    			/* disable acking */
    			i2c_ack_config(I2C0, I2C_ACK_DISABLE);

    			/* generate stop condition */
    			i2c_stop_on_bus(I2C0);
    		}

    		/* read data */
    		*(Rxbuffer) = i2c_data_receive(I2C0);

            count++

            /* if needed check crc */
            if (count % 3 == 0){
                if ( sfm3019_check_crc((Rxbuffer-2), *Rxbuffer) == I2C_CRC_ERROR){
                    return  I2C_CRC_ERROR;  
                }
            }

            /* incremment buffer */
    		Rxbuffer++;
    	}
    }

    /* enable acking */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return STATUS_OK;
}

uint8_t sfm3019_fill_buffer(uint16_t command, uint8_t *buff, uint8_t *args, uint8_t N_args){
    uint8_t count = 0;
    buff[count++] = (command && 0x00FFU);
    buff[count++] = (command << 8);

    for (int i = 0; i < N_args; i++){
        buff[count++] = (*args++);
        buff[count++] = (*args++);
        buff[count++] = sfm3019_generate_crc(buff[count-2]);
    }

    return count;
}

uint8_t sfm3019_generate_crc(uint8_t *data){
    uint16_t byte;
    uint8_t crc = CRC8_INIT;
    uint8_t bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (byte = 0; byte < SFM3019_WORD_SIZE; ++byte) {
        crc ^= (data[byte]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;

}
uint8_t sfm3019_check_crc(uint8_t *data, uint8_t checksum){
    if (sfm3019_generate_crc(data) == checksum){
        return STATUS_OK;
    }

    return I2C_CRC_ERROR;
}