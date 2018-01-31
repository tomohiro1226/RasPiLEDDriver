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

#define	GREEN	2	 
#define YELLOW	3
#define RED		4
#define A_7SEG	5
#define B_7SEG	6
#define C_7SEG	7
#define D_7SEG	8
#define E_7SEG	9
#define F_7SEG	10
#define G_7SEG	11
#define D_P_7SEG	12


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
void gpioExit(void);

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

	// GPIOピンの出力設定
	// 3色LEDの設定 and 7セグの設定
	GPFSEL0 |= 01111111100;
	GPFSEL1 |= 0111;

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

	if( k_buf[0]=='P' ){

	}else	
	if( k_buf[0]=='X' ){

	}else
	if( k_buf[0]=='H' || k_buf[0]=='L' ){

		def(k_buf[0], k_buf[1]);
		
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

void gpioExit(void){

    
    printk(KERN_INFO "myGpio exit\n");

    // マッピングの解放
    iounmap(gpio_map);

    return;
}

/**
 * @func 7セグの全消灯
*/
void segClear(void){
	gpioSet(A_7SEG);	
	gpioSet(B_7SEG);
	gpioSet(C_7SEG);
	gpioSet(D_7SEG);
	gpioSet(E_7SEG);
	gpioSet(F_7SEG);
	gpioSet(G_7SEG);
	gpioSet(D_P_7SEG);
}

/**
 * @func LEDの全消灯
*/
void ledClear(void){
	gpioClear(YELLOW);
	gpioClear(GREEN);
	gpioClear(RED);
}

void  def(char a, char b){

	u_int32_t *base;
	#define PBASE *base;

	*base = (a == "H") ? GPIOH : GPIOL;	

	switch(b){

		// LED
		case 'G':
			PBASE = (1<<GREEN);
			break;
		
		case 'Y':
			PBASE = (1<<YELLOW);
			break;

		case 'R':
			PBASE = (1<<RED);
			break;

		// 7セグ
		case 'A':
			PBASE = (1<<A_7SEG);
			break;
		
		case 'B':
			PBASE = (1<<B_7SEG);
			break;

		case 'C':
			PBASE = (1<<C_7SEG);
			break;

		case 'D':
			PBASE = (1<<D_7SEG);
			break;

		case 'E':
			PBASE = (1<<E_7SEG);
			break;

		case 'F':
			PBASE = (1<<F_7SEG);
			break;

		case 'H':
			PBASE = (1<<G_7SEG);
			break;

		case 'I':
			PBASE = (1<<D_P_7SEG);
			break;
		
	}

}
