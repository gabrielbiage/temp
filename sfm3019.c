#include "sfm3019.h"
#include "gd32vf103_i2c.c"
#include <stdio.h>
#include <stdint.h>

void sfm3019_start_continuous_measurement(uint32_t i2c_periph, uint16_t mode){

	uint16_t temp =	mode;
	uint8_t *ptr = &temp;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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

	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}    
	/* no argumments needed */

	/* no crc needed: */

   	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;
}


void sfm3019_config_concentration(uint32_t i2c_periph, uint16_t value){

	uint16_t temp =	SFM3019_COMMAND_UPDATE_C;
	uint8_t *ptr = &temp;
	uint8_t crc_value = CRC(value);

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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
	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}
	/* send arguments: */
		prt = &value;
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		  	ptr++;
		
		}
	/* send crc: */
		/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	    /*send crc */
	   	i2c_data_transmit(I2C0, crc);

	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;
}


void sfm3019_i2cptr_reset(uint32_t i2c_periph){

	uint16_t temp =	SFM3019_COMMAND_RESETPTR;
	uint8_t *ptr = &temp;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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

	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}    
	/* no argumments needed */

	/* no crc needed: */

   	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;
}


void sfm3019_stop_continuous_measurement(uint32_t i2c_periph){

	uint16_t temp =	SFM3019_COMMAND_STOP_CM;
	uint8_t *ptr = &temp;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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

	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}    
	/* no argumments needed */

	/* no crc needed: */

   	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;

}


void sfm3019_config_averaging(uint32_t i2c_periph, uint8_t N_value){

	uint16_t temp =	SFM3019_COMMAND_AVRG_CONFIG;
	uint8_t *ptr = &temp;
	uint8_t crc_value = CRC(N_value);

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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
	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}
	/* send arguments: */
		prt = &N_value;
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		  	ptr++;
		
		}
	/* send crc: */
		/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	    /*send crc */
	   	i2c_data_transmit(I2C0, crc);

	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;

}


SOF sfm3019_read_SOF(uint32_t i2c_periph, uint16_t gas_code, uint8_t readfirst){
	uint16_t temp =	SFM3019_COMMAND_AVRG_CONFIG;
	uint8_t *ptr = &temp;
	uint8_t crc_value = CRC(gas_code);

	SOF data;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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
	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}
	/* send arguments: */
		prt = &gas_code;
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		  	ptr++;
		
		}
	/* send crc: */
		/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	    /*send crc */
	   	i2c_data_transmit(I2C0, crc);

	//generate repeated start

	/* wait until the transmit bugger is empty */
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));	
	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_RECEIVER);	//This time in read mode
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    //read first data (Scala factor)

    ptr = &data;
    for (int i = 0; i < 2; i++){

    	/* wait until the receive data buffer is full */
    	while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));

    	/* disable acking if only first data is wanted */
	    if (readfirst == ENABLE && i == 1){
	    	i2c_ack_config(I2C0, I2C_ACK_DISABLE);
	    	i2c_stop_on_bus(I2C0);
	    }

    	/*read data */
    	*ptr = i2c_data_receive(I2C0);

    	ptr++;
    }

    //read crc
    /* wait until the receive data buffer is full */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
    /*read data */
    crc_value = i2c_data_receive(I2C0);

    if (readfirst == ENABLE){
    	/* wait until stop condition generate */
    	while(I2C_CTL0(I2C0)&0x0200);
    	/* set ack to enable again */
    	i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    	return data;
    }

    //read second data (Offset)

    for (int i = 0; i < 2; i++){

    	/* wait until the receive data buffer is full */
    	while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));

    	/*read data */
    	*ptr = i2c_data_receive(I2C0);

    	ptr++;

    }

    //read crc
    /* wait until the receive data buffer is full */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
    /*read data */
    crc_value = i2c_data_receive(I2C0);

    //read third data (Flow_unit)

    for (int i = 0; i < 2; i++){

    	/* wait until the receive data buffer is full */
    	while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));

    	if (i == 1){
    		i2c_ack_config(I2C0, I2C_ACK_DISABLE);
    		i2c_stop_on_bus(I2C0);
    	}

    	/*read data */
    	*ptr = i2c_data_receive(I2C0);

    	ptr++;

    }

    //read crc
    /* wait until the receive data buffer is full */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
    /*read data */
    crc_value = i2c_data_receive(I2C0);

    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);
    /* set ack to enable again */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    return data;

}


void sfm3019_generalcall_reset(uint32_t i2c_periph){
	uint16_t temp =	SFM3019_COMMAND_RESETCALL;
	uint8_t *ptr = &temp;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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

	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}    
	/* no argumments needed */

	/* no crc needed: */

   	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;

}


void sfm3019_SleepMode_enable(uint32_t i2c_periph){
	uint16_t temp =	SFM3019_COMMAND_SLEEP_MODE;
	uint8_t *ptr = &temp;

	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
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

	/* send command: */
		for (int i = 0; i < 2; i++){

		    /* wait until the transmit data buffer is empty */
		    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));

		    /*send command */
		   	i2c_data_transmit(I2C0, *ptr);

		   	ptr++;
		
		}    
	/* no argumments needed */

	/* no crc needed: */

   	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    /* wait until stop condition generate */
    while(I2C_CTL0(I2C0)&0x0200);

    return;

}


uint8_t CRC(uint16_t value){


}