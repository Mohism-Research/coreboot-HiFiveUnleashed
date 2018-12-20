#define CCACHE_CTRL_ADDR 0x2010000
#define CCACHE_ENABLE	0x008

void exit_car(void);

void exit_car(void)
{
	unsigned int * enable;
	enable = (unsigned int*)(CCACHE_CTRL_ADDR + CCACHE_ENABLE);
	__sync_lock_test_and_set(enable, 14);
}

