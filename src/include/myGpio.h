#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>

#define BASEADD 0x3F200000 // rpi2/3
// #define BASEADD 0x2000000 // rpi1

#define MAPBLOCK 4096 

/* 
* GPFSEL0 GPIO0～GPIO9のFSEL
* GPFSEL1 GPIO10～GPIO19のFSEL
* GPFSEL2 GPIO20～GPIO29のFSEL
* GPFSEL3 GPIO30～GPIO39のFSEL
*/
#define GPFSEL0 *((volatile uint32_t*)(gpio_map+0x00))
#define GPFSEL1 *((volatile uint32_t*)(gpio_map+0x04))
#define GPFSEL2 *((volatile uint32_t*)(gpio_map+0x08))
#define GPFSEL3 *((volatile uint32_t*)(gpio_map+0x0C))

#define GPIOH *((volatile uint32_t*)(gpio_map+0x1c))
#define GPIOL *((volatile uint32_t*)(gpio_map+0x28))

#define GPIO1 0x01
#define GPIO2 0x02
#define GPIO3 0x03
#define GPIO4 0x04
#define GPIO5 0x05
#define GPIO6 0x06
#define GPIO7 0x07
#define GPIO8 0x08
#define GPIO9 0x09
#define GPIOA 0x0A
#define GPIOB 0x0B
#define GPIOC 0x0C
#define GPIOD 0x0D
#define GPIOE 0x0E
#define GPIOF 0x0F


static void __iomem *gpio_map; //仮想アドレスと物理アドレスのマッピング

int gpioInit(void);
int gpioExit(void);
int gpioMode(uint32_t b, uint32_t s);
int gpioSet(uint32_t g);
int gpioClear(uint32_t g);