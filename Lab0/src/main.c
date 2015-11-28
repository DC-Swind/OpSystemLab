#include "common.h"
#include "x86.h"
#include "device.h"
#include "nes/fce.h"
#include "nes/rom.h"
#include "palette.h"
#include "stdbool.h"
static char rom[1024576];
void play(void);
void litenes(void);
/* 主循环，bootloader载入game二进制文件后，就跳转到这里执行 */
void
game_init(void) {
	init_timer();
	init_idt();
	init_intr();
	enable_interrupt();
        reset_palette();
	//play(); /* 这里是演示代码，显示浮动的文字和键盘扫描码 */
        litenes();
	assert(0); /* 此处永远不应该返回，为什么？ */
}


#define PI 3.1415926
int last_key = 0;
volatile int timers = 0, live = 0;

/* 这是键盘中断处理程序。
   每当键盘中断进入系统后，do_irq.S和irq_handle.c中的代码会调用这个函数。
   code是按键的扫描码。
   中断处理是原子的(不会用重入中断)。 */
bool status_left =false;
bool status_right=false;
bool status_up   =false;
bool status_down =false;
bool status_u    =false;
bool status_i    =false;
bool status_j    =false;
bool status_k    =false;
void
handle_keyboard(int code) {
	last_key = code;
	live = 20;
        switch (code){
           case 0x1e: status_left = true; break;
           case 0x9e: status_left = false; break;
           case 0x11: status_up   = true; break;
           case 0x91: status_up   = false; break;
          case  0x1f: status_down = true; break;
          case 0x9f: status_down = false; break;
          case  0x20: status_right= true; break;
          case 0xa0: status_right= false; break;
          case  0x17: status_i    = true; break;
          case 0x97: status_i    = false; break;
          case  0x16: status_u    = true; break;
          case 0x96: status_u    = false; break;
          case  0x24: status_j    = true; break;
          case 0xa4: status_j    = false; break;
          case  0x25: status_k    = true; break;
          case 0xa5: status_k    = false; break;
           default: ;
        }
}

/* 这是时钟中断处理程序。
   每当时钟中断进入系统后，do_irq.S和irq_handle.c中的代码会调用这个函数。 */
void
handle_timer(void) { // 时钟中断处理函数
	timers ++;
}

float sin(float x) {
	float sign = 1;
	while (x > PI) x -= 2 * PI;
	while (x < -PI) x += 2 * PI;
	if (x < -PI / 2) { sign = -1; x += PI; }
	if (x > PI / 2) { sign = -1;  x -= PI; }
	float x2 = x * x;
	float x3 = x * x2;
	float x5 = x3 * x2;
	float x7 = x5 * x2;
	return sign * (x - x3 / 5.0 + x5 / 120.0 - x7 / 5040.0);
}

void
play(void) { //主循环
	const char *text = "http://cslab.nju.edu.cn/opsystem";
	static char buf[2];
	int text_len = 32;
	int w = SCR_WIDTH / 8;
	int color = 0, start_pos = 0, clk = 0;

	for (; ; clk = (clk + 1) % 5) { // 主循环是一个死循环
		int i;
		prepare_buffer(); // 在绘图之前，先需要准备缓冲区

		if (clk == 0) {
			start_pos = (start_pos + 1) % w;
		}
		color = (color + 1) % 72;
		for (i = 0; i < text_len; i ++) { // 计算每个字符的位置 然后显示在屏幕上
			int j = (i + start_pos) % w;
			int d = 50 * sin(2 * PI * j / w);
			buf[0] = text[i];
			draw_string(buf, 8 * j, SCR_HEIGHT / 2 - 4 + d, 32 + color);
		}
		
		// 在左下角显示键盘扫描码
		draw_string(itoa(last_key), 0, SCR_HEIGHT - 8, live > 0 ? 10: 7);
		if (live > 0) live --;

		i = HZ / 60;	
		while (i) {
			wait_for_interrupt();
			disable_interrupt(); // 关闭中断是为了防止数据竞争(data race)。
			if (timers > 0) {
				timers --;
				i --;
			}
			enable_interrupt();
		}

		display_buffer(); // 绘图结束后，调用这个函数将绘制的图像显示到屏幕上
	}
}

void litenes(void){
    int i;
    for (i =0 ; i<len; i++) rom[i] = (char)table[i];
    rom[len] = '\0';
    if (fce_load_rom(rom) != 0){}
    fce_init();
    fce_run();
}

