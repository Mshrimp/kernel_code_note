#include <stdio.h>
#include <generated/autoconf.h>

int main(void)
{
	int vala = 0; 
	int valb = 0;
	int valc = 0;
	void (*the_kernel)(void) = NULL;
	char *cmdline = NULL;	//bootargs
#ifdef	CONFIG_TEST_A
	vala = CONFIG_TEST_A;
#endif
#ifdef	CONFIG_TEST_B
	valb = CONFIG_TEST_B;
#endif
#ifdef	CONFIG_TEST_C
	valc = CONFIG_TEST_C;
#endif

#ifdef	CONFIG_TEST_ENTRY
	the_kernel = (void *)CONFIG_TEST_ENTRY;
#endif

#ifdef	CONFIG_TEST_CMDLINE
	cmdline = CONFIG_TEST_CMDLINE;
#endif

	printf("vala = %d valb = %d valc = %d\n", vala, valb, valc);
	printf("the_kernel = %p\n");
	printf("cmdline = %s\n", cmdline);

	return 0;
}
