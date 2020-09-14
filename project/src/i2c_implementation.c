/*
 * i2c_implementation.c
 *
 *  Created on: 3 de ago de 2020
 *      Author: mateus
 */


/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "i2c_implementation.h"
#include "gd32f10x_libopt.h"

#define I2C0_OWN_ADDRESS7      0x72
#define I2C1_OWN_ADDRESS7      0x72

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void my_i2c_init(void) {
	//TODO adicionar inicialização dos pinos para I2C1

    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C0 and I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C0);
    rcu_periph_clock_enable(RCU_I2C1);
    /* connect PB6 to I2C0_SCL */
    /* connect PB7 to I2C0_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    /* I2C clock configure */
    i2c_clock_config(I2C0, 400000, I2C_DTCY_2);
    i2c_clock_config(I2C1, 400000, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_OWN_ADDRESS7);
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_OWN_ADDRESS7);
    /* enable I2Cx */
    i2c_enable(I2C0);
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);

    while(gpio_output_bit_get(GPIOB, GPIO_PIN_6) == SET){
    	asm("nop");
    }

    while(gpio_output_bit_get(GPIOB, GPIO_PIN_7) == SET){
        	asm("nop");
    }

//    if (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
//    	i2c_stop_on_bus(I2C0);
//    	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
//    }


    return;
}

/**
 * Release all resources initialized by sensirion_i2c_init().
 */
void my_i2c_release(void) {
	//TODO ver necessidade dos deinit
	/* deinit I2Cx */
    i2c_deinit(I2C0);
    i2c_deinit(I2C1);
    /* deinit GPIOB */
	gpio_deinit(GPIOB);
	/* disable I2Cx clock */
	rcu_periph_clock_disable(RCU_I2C0);
	rcu_periph_clock_disable(RCU_I2C1);
	/* disable GPIOB clock */
	rcu_periph_clock_disable(RCU_GPIOB);
    return;
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param bus     I2C bus to send transaction
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t my_i2c_read(uint32_t bus, uint8_t address, uint8_t *data, uint16_t count) {
	if(count==2){
	    /* send a NACK for the next data byte which will be received into the shift register */
	    i2c_ackpos_config(bus, I2C_ACKPOS_NEXT);
	}
    /* wait until I2C bus is idle */
    while(i2c_flag_get(bus, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(bus);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(bus, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(bus, address, I2C_RECEIVER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(bus, I2C_FLAG_ADDSEND));

    if(count==1){
	    /* N=1,reset ACKEN bit before clearing ADDRSEND bit */
	    i2c_ack_config(bus, I2C_ACK_DISABLE);
    }

    /* clear ADDSEND bit */
    i2c_flag_clear(bus, I2C_FLAG_ADDSEND);

	if(count==1){
	    /* N=1,send stop condition after clearing ADDRSEND bit */
	    i2c_stop_on_bus(bus);
	    /* wait until the RBNE bit is set */
	    while(!i2c_flag_get(bus, I2C_FLAG_RBNE));
	    /* read a data from I2C_DATA */
	    data[0] = i2c_data_receive(bus);
	}
	else if(count==2){
	    /* Wait until the last data byte is received into the shift register */
	    while(!i2c_flag_get(bus, I2C_FLAG_BTC));
	    /* wait until the RBNE bit is set */
	    while(!i2c_flag_get(bus, I2C_FLAG_RBNE));

	    /* read a data from I2C_DATA */
	    data[0] = i2c_data_receive(bus);
	    /* wait until the RBNE bit is set */
	    while(!i2c_flag_get(bus, I2C_FLAG_RBNE));
	    /* read a data from I2C_DATA */
	    data[1] = i2c_data_receive(bus);
	    /* send a stop condition */
	    i2c_stop_on_bus(bus);
	}
	else{
	    for(int i=0; i<count; i++){
	        if(i==count-3){
	            /* wait until the second last data byte is received into the shift register */
	            while(!i2c_flag_get(bus, I2C_FLAG_BTC));
	            /* disable acknowledge */
	            i2c_ack_config(bus, I2C_ACK_DISABLE);
	        }
	        /* wait until the RBNE bit is set */
	        while(!i2c_flag_get(bus, I2C_FLAG_RBNE));
	        /* read a data from I2C_DATA */
	        data[i] = i2c_data_receive(bus);
	    }
	    /* send a stop condition to I2C bus */
	    i2c_stop_on_bus(bus);
	}

    /* wait until stop condition generate */
    while(I2C_CTL0(bus)&0x0200);
	if(count==2){
	    i2c_ackpos_config(bus, I2C_ACKPOS_CURRENT);
	}
    /* enable acknowledge */
    i2c_ack_config(bus, I2C_ACK_ENABLE);

    return 0;

    //TODO implementar retorno de erro caso device não envie ACK
    //return STATUS_FAIL;
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param bus     I2C bus to send transaction
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t my_i2c_write(uint32_t bus, uint8_t address, const uint8_t *data,
                           uint16_t count) {
    /* wait until I2C bus is idle */
    while(i2c_flag_get(bus, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(bus);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(bus, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(bus, address, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(bus, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(bus, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(bus, I2C_FLAG_TBE));

    for(int i=0; i<count; i++){
        /* data transmission */
        i2c_data_transmit(bus, data[i]);
        /* wait until the TBE bit is set */
        while(!i2c_flag_get(bus, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(bus);
    while(I2C_CTL0(bus)&0x0200);

    return 0;

    //TODO implementar retorno de erro caso device não envie ACK
    //return STATUS_FAIL
}
