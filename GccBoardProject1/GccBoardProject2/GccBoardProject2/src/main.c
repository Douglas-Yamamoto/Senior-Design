/**
 * \file
 *
 * \brief Empty user application template
 *
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
#include "asf.h"
#include "led.h"
#include "sleep.h"

static void PIRHandler(const uint32_t id, const uint32_t index);
static void enablePIRIRQ(void);

int main (void)
{
	sysclk_init();
	board_init();

	ioport_set_pin_dir(LED0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(PIO_PB0_IDX, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(PIO_PB0_IDX, (IOPORT_MODE_PULLUP | IOPORT_MODE_GLITCH_FILTER));
	ioport_set_pin_sense_mode(PIO_PB0_IDX, IOPORT_SENSE_RISING);
	
	enablePIRIRQ();
	while(1){};
	//ioport_set_pin_dir(LED0_GPIO, IOPORT_DIR_OUTPUT);
	//
	//pmc_enable_periph_clk(ID_PIOB);		// Enable the peripheral clock for Peripheral Controller B
	//pio_set_input(PIOB, PIO_PB0, PIO_PULLUP);	// Setting appropriate GPIO Pin as input
//
	//NVIC_DisableIRQ(PIOB_IRQn);			// Disable all current interrupts
	//NVIC_ClearPendingIRQ(PIOB_IRQn);	// Clear any prior existing interrupts
	//pio_handler_set(PIOB, ID_PIOB, PIO_PB0, PIO_IT_RISE_EDGE, pin_edge_handler);		// Map the pin to an interrupt handler
	//NVIC_SetPriority(PIOB_IRQn, 1);		// Set the priority of interrupt in PIOB
	//pio_enable_interrupt(PIOB, PIO_PB0);	// Enable interrupt at the peripheral level
	//
	//NVIC_EnableIRQ(PIOB_IRQn);		// Enable all interrupts
}

static void PIRHandler (const uint32_t id, const uint32_t index)
{
	uint32_t bitmask = pio_get_interrupt_mask(PIOB);
	pio_disable_interrupt(PIOB, 0xFFFFFFFF);
	pio_get_interrupt_status(PIOB);
	NVIC_DisableIRQ((IRQn_Type) ID_PIOB);
	
	NVIC_ClearPendingIRQ((IRQn_Type) ID_PIOB);
	NVIC_SetPriority((IRQn_Type) ID_PIOB, 0x00);
	NVIC_EnableIRQ((IRQn_Type) ID_PIOB);
	pio_enable_interrupt(PIOB, bitmask);
	
	for (int i = 0; i < 100000000; i++)
	{
		LED_On(LED0);
	}
	
	LED_Off(LED0);
	
	return;
}

static void enablePIRIRQ(void)
{
	pmc_enable_periph_clk(ID_PIOB);
	pio_handler_set(PIOB, ID_PIOB, PIO_PB0, PIO_IT_RISE_EDGE, PIRHandler);	
	pio_handler_set_priority(PIOB, (IRQn_Type) ID_PIOB, 0x00);
	
	return;
}