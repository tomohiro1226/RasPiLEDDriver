#include<myGpio.h>

int gpioInit(void){

    gpio_map = NULL;

    // 仮想アドレスと物理アドレスのマッピング
    gpio_map = ioremap_nocache(BASEADD, MAPBLOCK);

    return (gpio_map!=NULL) ? 0 : 1; 
}

/**
 * GPIOのポートの初期設定を行う
 * 8進数で登録すると良い(例 : 011100 //GPIO2-4を出力にする)
 * @param b GPFSEL*
 * @param s ポートの設定(1 : 出力, 2 : 入力)
 * @return 0 正常終了
 * @return 1 異常終了
*/
int gpioMode(uint32_t b, uint32_t s){

    b = s;
    return (b==s) ? 0 : 1;
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

    // マッピングの解放
    iounmap(gpio_map);
}