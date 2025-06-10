#include "handle_cmd.h"
#include "../3rd/xfusion/xf_utils/xf_utils_log.h"

ELAB_TAG("handle_cmd");

typedef void (*command_handler_t)(uint8_t *data);

typedef struct {
    uint8_t cmd_id;
    command_handler_t handler;
} command_entry_t;



static void speed_set_cmd(uint8_t *data);
static void pid_set_cmd(uint8_t *data);

command_entry_t command_table[] = {
    {0xFA, speed_set_cmd},
    {0x02, pid_set_cmd},
    // 添加更多命令
};


typedef union {
    uint8_t data_hex[4];
    float data_float;
}speed_data_t;


static void speed_set_cmd(uint8_t *data) {
   speed_data_t  speed_convert;
   speed_convert.data_hex[0] = data[1];
   speed_convert.data_hex[1] = data[2];
   speed_convert.data_hex[2] = data[3];
   speed_convert.data_hex[3] = data[4];
   XF_LOG_BUFFER_HEX(data,5);
   
   float speed = speed_convert.data_float;
   elog_info("speed:%f",speed);
};

static void pid_set_cmd(uint8_t *data) {
    (void)data;
}


void parse_command(uint8_t *data) {
    uint8_t cmd_id = data[0]; // 命令ID在第一个字节
    for (int i = 0; i < sizeof(command_table) / sizeof(command_entry_t); i++) {
        if (command_table[i].cmd_id == cmd_id) {
            command_table[i].handler(data);
            return;
        }
    }
    // 未知命令处理
}