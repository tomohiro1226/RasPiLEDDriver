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

#include <myGpio.h>

/*ドライバ名*/
#define MYLED_DEVNAME   "myled"
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

	// ledドライバの設定
	gpioInit();

	// 2-4を出力設定
	#define GPIOMODE 011100
	// 3色LEDの設定
	gpioMode(GPFSEL0, GPIOMODE);

	/*デバイスドライバメジャー番号の登録*/
	if( register_chrdev(myled_major, myled_devname , &myled_fops) < 0 ){
		printk(KERN_ERR "myled error: register_chrdev\n");
	}
	
	printk(KERN_INFO "%s loaded. major:%0x \n", myled_devname, myled_major);


	// 実験
	gpioSet(GPIO4);
	msleep(1000);
	gpioClear(GPIO4);
	msleep(1000);
	gpioSet(GPIO4);
	msleep(1000);
	gpioClear(GPIO4);
	msleep(1000);
	gpioSet(GPIO4);
	msleep(1000);
	gpioClear(GPIO4);
	msleep(1000);
	
	
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
	
	
	//処理
	printk( KERN_INFO "myled: myled_write is called.\n" );
	
	
	/*戻り値は書き込んだ文字数にすること*/
	return 0;
}

/*read*/
static ssize_t myled_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	
	
	/*処理*/
	printk( KERN_INFO "myled: myled_read is called.\n" );
	
	
	/*戻り値は必ず読み込んだ文字数にすること。*/
	return 0;
}


module_init(myled_init);
module_exit(myled_exit);


