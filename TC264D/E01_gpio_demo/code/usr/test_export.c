#include "elab_export.h"
#include "elab_log.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST,EXPORT_DRVIVER);


int sum;
void test_polling(void)
{
    // elog_debug("polling");
    // for(int i=0;i<=100;i++)
    // {
    //     sum=sum+i;

    // }
    // elog_debug("sum %d",sum);
}
POLL_EXPORT(test_polling,5);



