#include <stdio.h>

extern void test_clock(void);
extern void test_timer_state(void);
extern void test_timer_engine(void);
extern void test_settings(void);

int main(void)
{
    printf("========================================\n");
    printf("   Timer420 Unit & Integration Test Suite\n");
    printf("========================================\n\n");

    test_clock();
    test_timer_state();
    test_timer_engine();
    test_settings();

    printf("\n========================================\n");
    printf(" ALL UNIT TESTS PASSED SUCCESSFULLY!\n");
    printf("========================================\n");
    return 0;
}
