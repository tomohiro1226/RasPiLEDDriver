/*デバイスドライバ サンプル 2017.12.20*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/delay.h>

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

/*ドライバ名*/
#define MYLED_DEVNAME "myled"
/*ドライバのメジャー番号*/
#define MYLED_MAJOR     201

MODULE_AUTHOR( "kcg" );
MODULE_DESCRIPTION( "Sample character driver." );
MODULE_LICENSE("Dual BSD/GPL");

static int myled_init(void);
static void myled_exit(void);
static int myled_open(struct inode *inode, struct file *filp);
static int myled_close(struct inode *inode, struct file *filp);
static ssize_t myled_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static ssize_t myled_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

int gpioInit(void);
int gpioExit(void);
int gpioMode(uint32_t b, uint32_t s);
int gpioSet(uint32_t g);
int gpioClear(uint32_t g);


/*グローバル変数の宣言*/

/*コールバック*/
static struct file_operations myled_fops = {
	owner:   THIS_MODULE,
	read:    myled_read,    /*readの時の関数*/
	write:   myled_write,   /*writeの時の関数*/
	open:    myled_open,    /*openの時の関数*/
	release: myled_close, /*closeの時の関数*/
};

static unsigned int myled_major = MYLED_MAJOR;
static char *myled_devname = MYLED_DEVNAME;

/*カーネルモジュールロード時*/
static int myled_init(void){

	
	/*デバイスドライバメジャー番号の登録*/
	if( register_chrdev(myled_major, myled_devname , &myled_fops) < 0 ){
		printk(KERN_ERR "myled error: register_chrdev\n");
	}
	
	printk(KERN_INFO "%s loaded. major:%0x \n", myled_devname, myled_major);

	// ledドライバの設定
	gpioInit();

	// 2-4を出力設定
	// 3色LEDの設定
	GPFSEL0 = 011100;

	// 実験
	// gpioSet(GPIO4);
	// msleep(1000);
	// gpioClear(GPIO4);
	// msleep(1000);
	// gpioSet(GPIO4);
	// msleep(1000);
	// gpioClear(GPIO4);
	// msleep(1000);
	// gpioSet(GPIO4);
	// msleep(1000);
	// gpioClear(GPIO4);
	// msleep(1000);
	
	return 0;
}

/*カーネルモジュールアンロード時*/
static void myled_exit(void){

	// ledドライバーの解放
	gpioExit();
	
	/*デバイスドライバメジャー番号の解放*/
	unregister_chrdev(myled_major, myled_devname);
	printk(KERN_INFO "my first driver unloaded \n");
	return ;
}


/*open*/
static int myled_open(struct inode *inode, struct file *filp){


	printk( KERN_INFO "myled: myled_open is called.\n" );

	return 0;
}

/*close*/
static int myled_close(struct inode *inode, struct file *filp)
{

	//処理
	printk( KERN_INFO "myled: myled_release is called.\n" );

	return 0;
}


#define K_BUFF_SIZE     64
static ssize_t myled_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos){
	
	char k_buf[256];

	if( copy_from_user(k_buf, buf, count) != 0 ){
		return 0;
	}
	k_buf[count] = '\0';
	
	//処理
	printk( KERN_INFO "myled: myled_write is called.\n" );
	printk( KERN_INFO "number: %s %d.\n",k_buf ,count);

	// 緑点灯
	if( !strncmp(k_buf, "HG", count) ){
		printk( KERN_INFO "HG.\n");
		gpioSet(GPIO4);
	}
	// 黄色点灯
	if( !strncmp(k_buf, "HY", count) ){
		gpioSet(GPIO3);	
	}
	// 赤色点灯
	if( !strncmp(k_buf, "HR", count) ){
		gpioSet(GPIO2);	
	}
	// 緑色消灯
	if( !strncmp(k_buf, "LG", count) ){
		gpioClear(GPIO4);
	}
	// 黄色消灯
	if( !strncmp(k_buf, "LY", count) ){
		gpioClear(GPIO3);	
	}
	// 赤色消灯
	if( !strncmp(k_buf, "LR", count) ){
		gpioClear(GPIO2);	
	}
	
	
	/*戻り値は書き込んだ文字数にすること*/
	return count;
}

/*read*/
static ssize_t myled_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){

	
	
	
	/*処理*/
	printk( KERN_INFO "myled: myled_read is called.\n" );

	


	
	
	/*戻り値は必ず読み込んだ文字数にすること。*/
	return count;
}


module_init(myled_init);
module_exit(myled_exit);

int gpioInit(void){

    printk(KERN_INFO "myGpio init\n");

    gpio_map = NULL;

    // 仮想アドレスと物理アドレスのマッピング
    gpio_map = ioremap_nocache(BASEADD, MAPBLOCK);

    return (gpio_map!=NULL) ? 0 : 1; 
}

/** 
 * GPIOポートをHighに
 * @param g ポート番号(GPIO*)
*/
int gpioSet(uint32_t g){
    GPIOH = g;

    return (GPIOH==g) ? 0 : 1;
}

/** 
 * GPIOポートをLowに
 * @param g ポート番号(GPIO*)
*/
int gpioClear(uint32_t g){
    GPIOL = g;

    return (GPIOH==g) ? 0 : 1;
}

int gpioExit(void){

    
    printk(KERN_INFO "myGpio exit\n");

    // マッピングの解放
    iounmap(gpio_map);

    return 0;
}
