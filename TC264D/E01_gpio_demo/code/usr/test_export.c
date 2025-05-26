#include "elab_export.h"
#include "elab_log.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST,EXPORT_DRVIVER);



void test_polling(void)
{
    elog_error("polling");
}
POLL_EXPORT(test_polling,1);



