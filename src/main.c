#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(void)
{
	int i;

	rcc_clock_setup_in_hsi_out_64mhz();
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO1 | GPIO2);

	while (1) {
		gpio_clear(GPIOC, GPIO13);
		gpio_set(GPIOA, GPIO1);
		gpio_set(GPIOA, GPIO2);
		for (i = 0; i < 8000000; i++) __asm__("nop");

		gpio_set(GPIOC, GPIO13);
		gpio_clear(GPIOA, GPIO1);
		gpio_set(GPIOA, GPIO2);
		for (i = 0; i < 8000000; i++) __asm__("nop");

		gpio_set(GPIOC, GPIO13);
		gpio_set(GPIOA, GPIO1);
		gpio_clear(GPIOA, GPIO2);
		for (i = 0; i < 8000000; i++) __asm__("nop");
	}

	return 0;
}
