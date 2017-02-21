/*
 * DIYReflowOvenV1.c
 *
 * Created: 2016-12-13 22:13:06
 * Author : Branden
 */ 
/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
//#define __AVR_ATxmega128A4U__
#include <asf.h>
#include <avr/io.h>
#include <stdio.h>
#include <nvm.h>
#include <string.h>

#include "CommandDecoder.h"
#include "Oven.h"
#include "sp_driver.h"

volatile uint8_t count = 0;
volatile uint16_t index = 0;
volatile Bool doFlashTest = false;


/* Buffers for testing Load/Read Flash Page. */
uint8_t WriteBuffer[FLASH_PAGE_SIZE];
uint8_t ReadBuffer[FLASH_PAGE_SIZE];



void uart_putchar(char c, FILE * stream)
{
	udi_cdc_putc(c);
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static bool my_flag_autorize_cdc_transfert = false;

bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}
void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}
void task(void)
{
	if (my_flag_autorize_cdc_transfert) {
		udi_cdc_putc('A');
		udi_cdc_getc();
	}
}

void FlashTest(uint8_t pagesToWrite)
{
	
	if (pagesToWrite > 16)
		pagesToWrite = 16;
	
	printf("Running test for %d page(s)\r\n", pagesToWrite);

	for (uint8_t p = 0; p < pagesToWrite; p++)
	{

		memset(ReadBuffer, 0, sizeof(ReadBuffer));
		
		/* Fill up a test buffer with 512 bytes with other values. */
		for (uint16_t i = 0; i < FLASH_PAGE_SIZE; i++) {
			WriteBuffer[i] = (uint8_t) i;
			printf("Page %d WriteBuffer[%d] = %d\r\n", p, i, WriteBuffer[i] );
		}
				
		/* Load the flashbuffer with the test buffer. */
		SP_LoadFlashPage(WriteBuffer);
			
		/* Do a Erase-Write of the page. */
		EraseWriteAppTablePage(p);

		/* Read a flashpage into the read buffer. */
		ReadFlashPage(ReadBuffer, p);


		printf("Verify\r\n");
		/* Verify Flash contents. */
		for (uint16_t i = 0; i < FLASH_PAGE_SIZE; i++) {
			printf("Page %d ReadBuffer[%d] = %d\r\n", p, i, ReadBuffer[i] );
		}
	}
	
	printf("Done\r\n");
}


void my_callback_rx_notify(uint8_t port)
{
	char c = udi_cdc_getc();
	
	doFlashTest = true;
	
	ProcessInput(c);
}

void my_callback_tx_empty_notify(uint8_t port)
{
	
	
}

ISR(TCC0_OVF_vect)
{
	count++;
	
	if (count == 125)
	{
		PORTB.OUTTGL = PIN0_bm;
		count = 0;		
		
		if (_logEnabled)
		{
			printf("%d: %02x\r\n", index, nvm_eeprom_read_byte(index));
			index++;
			
			if (index >= 1280)
				index = 0;
		}
	}
}


void init_timer()
{	
	sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_TC0);
	
	TCC0.PER = 750;                 // Period register
		
	TCC0.INTCTRLA = TC_TC0_OVFINTLVL_LO_gc; // Overflow, LOW Priority
	
	TCC0.INTCTRLB = 0;              // Compare
	TCC0.INTFLAGS = 0;              // Overflow Flag in bit 0
	
	TCC0.CTRLA =  TC_TC0_CLKSEL_DIV64_gc; // PortC T/C 0, Source = Clock, Div by 8
	TCC0.CTRLB = 0;                 // Normal Mode, Update on Top
	TCC0.CTRLC = 0;                 // Waveform
	TCC0.CTRLD = 0;                 // Event System
	TCC0.CTRLE = 0;                 // 8-Bit Mode

}


int main(void)
{


	PORTB.DIR = PIN0_bm | PIN1_bm | PIN2_bm;
	
	cli();

	sysclk_init();

	init_timer();
	udc_start();
	
	stdout = &mystdout;	
	irq_initialize_vectors();
	
	_ovenStatus = Running;
	eeprom_enable_mapping();


	sei();
		
		
    /* Replace with your application code */
    while (1) 
    {
		if (doFlashTest)
		{
			FlashTest(3);
			doFlashTest = false;
		}
    }
}

