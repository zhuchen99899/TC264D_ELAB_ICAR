# 注意

> [!WARNING]
>
> -  禁止在裸机中使用阻塞的消息队列操作
>
>   ```c
>         
>    if (osMessageQueueGet(mq, buffer, NULL, 1000) == osErrorTimeout) {
>        elog_info("Timeout occurred while waiting for a message.\n");
>    }
>   ```



# 测试用例

```c
#include "elab_export.h"
#include "elab_log.h"
#include "elab_device.h"
#include "cmsis_os2.h"
ELAB_TAG("test_export");
void func_TEST(uint32_t *handle)
{
    elog_debug("func_TEST");
}
INIT_EXPORT(func_TEST,EXPORT_DRVIVER);



static void test_message_queue(void);

static void _timer_func(void *argument){

    test_message_queue();

}



static const osTimerAttr_t test_timer_attr_led =
{
    .name = "test_timer",
    .attr_bits = 0,
    .cb_mem = NULL,
    .cb_size = 0,
};
static osTimerId_t test_timer;
void init_test_timer(void)
{
    test_timer = osTimerNew(_timer_func, osTimerPeriodic, NULL, &test_timer_attr_led);

    osTimerStart(test_timer, 10);
}
INIT_EXPORT(init_test_timer,EXPORT_MIDWARE);



// 测试用例函数
void test_message_queue(void) {
    // 创建一个消息队列，容量为 5，消息大小为 16 字节
    osMessageQueueId_t mq = osMessageQueueNew(5, 16, NULL);
    if (mq == NULL) {
        elog_info("Failed to create message queue.\n");
        return;
    }

    // 测试数据
    const char *msg1 = "Message1";
    const char *msg2 = "Message2";
    char buffer[16];

    // 测试消息发送
    if (osMessageQueuePut(mq, msg1, 0, 0) == osOK) {
        elog_info("Message1 sent successfully.\n");
    } else {
        elog_info("Failed to send Message1.\n");
    }

    if (osMessageQueuePut(mq, msg2, 0, 0) == osOK) {
        elog_info("Message2 sent successfully.\n");
    } else {
        elog_info("Failed to send Message2.\n");
    }

    // 测试消息接收
    if (osMessageQueueGet(mq, buffer, NULL, 0) == osOK) {
        elog_info("Received: %s\n", buffer);
    } else {
        elog_info("Failed to receive message.\n");
    }

    if (osMessageQueueGet(mq, buffer, NULL, 0) == osOK) {
        elog_info("Received: %s\n", buffer);
    } else {
        elog_info("Failed to receive message.\n");
    }

    // 禁止在裸机中使用阻塞的消息队列操作
    // if (osMessageQueueGet(mq, buffer, NULL, 1000) == osErrorTimeout) {
    //     elog_info("Timeout occurred while waiting for a message.\n");
    // }

    // 删除消息队列
    if (osMessageQueueDelete(mq) == osOK) {
        elog_info("Message queue deleted successfully.\n");
    } else {
        elog_info("Failed to delete message queue.\n");
    }
}



```

# 串口用例 

这段 `MessageQueue` 的实现可以用于串口消息队列，但需要根据串口的具体需求进行一些调整和扩展。

---

### **适用场景**
1. **串口消息队列的需求**：
   - 串口通常需要一个消息队列来存储接收到的数据（如字符或数据包）。
   - 消息队列可以用于串口发送任务和接收任务之间的通信。
   - 队列需要支持固定大小的消息（如数据包）或动态长度的消息（如字符串）。

2. **当前 `MessageQueue` 的特点**：
   - 支持固定大小的消息。
   - 提供阻塞操作（等待队列状态改变）。
   - 线程安全（在裸机环境下通过轮询实现）。

---

### **如何应用到串口消息队列**

#### **1. 接收队列**
- 串口接收任务可以将接收到的数据存储到 `MessageQueue` 中。
- 使用 `osMessageQueuePut` 将数据放入队列。
- 其他任务可以通过 `osMessageQueueGet` 从队列中读取数据。

#### 示例代码：
```c
void uart_receive_callback(const char *data, uint32_t length) {
    // 假设 mq 是串口接收队列
    if (osMessageQueuePut(mq, data, 0, 0) != osOK) {
        elog_error("Failed to enqueue received data.\n");
    }
}

void uart_process_task(void) {
    char buffer[16];
    while (1) {
        if (osMessageQueueGet(mq, buffer, NULL, osWaitForever) == osOK) {
            // 处理接收到的数据
            process_uart_data(buffer);
        }
    }
}
```

---

#### **2. 发送队列**
- 串口发送任务可以从 `MessageQueue` 中读取数据并发送到串口。
- 使用 `osMessageQueuePut` 将待发送的数据放入队列。
- 串口发送任务通过 `osMessageQueueGet` 获取数据并发送。

#### 示例代码：
```c
void uart_send_task(void) {
    char buffer[16];
    while (1) {
        if (osMessageQueueGet(mq, buffer, NULL, osWaitForever) == osOK) {
            // 发送数据到串口
            uart_send(buffer, strlen(buffer));
        }
    }
}

void enqueue_uart_send(const char *data) {
    if (osMessageQueuePut(mq, data, 0, 0) != osOK) {
        elog_error("Failed to enqueue data for sending.\n");
    }
}
```

---

### **扩展功能**

#### **1. 动态消息长度支持**
当前 `MessageQueue` 的实现要求消息大小固定。如果串口消息长度不固定（如字符串长度不同），可以通过以下方式扩展：
- 使用指针存储消息地址。
- 在队列中存储消息长度。

#### 示例：
```c
typedef struct {
    char *data;
    uint32_t length;
} uart_message_t;

void enqueue_uart_message(const char *data, uint32_t length) {
    uart_message_t msg = { .data = data, .length = length };
    if (osMessageQueuePut(mq, &msg, 0, 0) != osOK) {
        elog_error("Failed to enqueue UART message.\n");
    }
}

void uart_process_task(void) {
    uart_message_t msg;
    while (1) {
        if (osMessageQueueGet(mq, &msg, NULL, osWaitForever) == osOK) {
            // 处理接收到的消息
            process_uart_data(msg.data, msg.length);
        }
    }
}
```

#### 
---

#### **3. 串口中断与队列结合**
- 串口接收中断可以直接调用 `osMessageQueuePut` 将数据放入队列。
- 串口发送中断可以从队列中取出数据并发送。

---

### **注意事项**

#### **1. 队列容量**
- 根据串口的吞吐量和消息大小，合理设置队列容量。
- 如果队列满了，可能需要丢弃数据或等待队列有空闲空间。

#### **2. 串口中断与任务结合**
- 串口接收和发送通常通过中断触发。
- 中断处理函数应尽量简短，可以将数据放入队列后立即返回。

#### **3. 时间戳溢出**
- 当前实现使用 `elab_time_ms` 进行超时处理，确保时间戳溢出时逻辑仍然正确。

---

### **总结**
- 当前 `MessageQueue` 实现可以直接用于串口消息队列。
- 根据串口的具体需求，可以扩展支持动态消息长度、优先级等功能。
- 串口接收和发送任务可以通过 `MessageQueue` 实现任务间通信，确保数据处理的实时性和可靠性。

在 FreeRTOS 中，消息队列和环形缓冲区（Circular Buffer）是两种不同的数据结构，适用于不同的场景。是否需要环形缓冲区取决于你的具体需求和应用场景。以下是两者的比较和分析：

---

### **消息队列与环形缓冲区的区别**

#### **消息队列**
- **特点**：
  - FreeRTOS 的消息队列是线程安全的，支持任务间通信。
  - 可以存储固定大小的消息（如结构体、指针或简单数据）。
  - 支持阻塞操作（任务可以等待队列状态改变）。
  - 消息队列的容量是有限的，超出容量时会阻塞或返回错误。

- **适用场景**：
  - 任务间通信：任务 A 发送数据到任务 B。
  - 事件通知：任务等待某个事件发生。
  - 数据传递：传递固定大小的数据包。

#### **环形缓冲区**
- **特点**：
  - 环形缓冲区是一个简单的 FIFO 数据结构，通常用于存储连续的数据流（如字节流）。
  - 不支持任务间通信，通常用于单任务或中断与任务之间的数据传递。
  - 不支持阻塞操作，通常通过轮询或中断处理数据。
  - 可以实现覆盖旧数据（可选），适合实时性要求高的场景。

- **适用场景**：
  - 数据流处理：存储串口接收的数据流。
  - 缓冲区管理：存储传感器数据或日志。
  - 高速数据采集：实时性要求高，允许覆盖旧数据。

---

### **是否需要环形缓冲区**

#### **1. 如果已有消息队列**
- **不需要环形缓冲区的场景**：
  - 如果消息队列已经满足你的需求（如任务间通信、事件通知），通常不需要额外的环形缓冲区。
  - 消息队列已经是线程安全的，适合任务间的数据传递。

- **需要环形缓冲区的场景**：
  - 如果需要处理连续的数据流（如串口接收数据），环形缓冲区可能更适合。
  - 如果数据量较大，消息队列的容量不足以存储所有数据，可以使用环形缓冲区作为临时缓冲。
  - 如果需要覆盖旧数据（实时性要求高），环形缓冲区可以实现这一功能，而消息队列通常不会覆盖数据。

---

### **典型应用场景**

#### **1. 串口接收数据**
- **环形缓冲区**：
  - 中断将接收到的数据存储到环形缓冲区。
  - 任务从环形缓冲区读取数据并处理。
  - 适合处理高速数据流。

- **消息队列**：
  - 中断将接收到的数据发送到消息队列。
  - 任务从消息队列接收数据并处理。
  - 适合任务间通信。

#### **2. 数据采集**
- **环形缓冲区**：
  - 用于存储传感器数据或高速采集的数据。
  - 数据量较大时，允许覆盖旧数据。

- **消息队列**：
  - 用于传递固定大小的数据包到其他任务。
  - 数据量较小时，消息队列更适合。

#### **3. 日志系统**
- **环形缓冲区**：
  - 用于存储日志数据，实时性要求高时允许覆盖旧日志。
  - 适合单任务或中断与任务之间的日志存储。

- **消息队列**：
  - 用于传递日志数据到处理任务。
  - 适合任务间通信。

---

### **如何结合使用**

在某些复杂场景中，可以结合使用消息队列和环形缓冲区：
1. **环形缓冲区作为中间缓冲**：
   - 中断将数据存储到环形缓冲区。
   - 任务从环形缓冲区读取数据并发送到消息队列。
   - 消息队列用于任务间通信。

2. **消息队列用于事件通知**：
   - 环形缓冲区存储数据流。
   - 消息队列通知任务数据已准备好。

---

### **总结**

- **如果已有消息队列**：
  - 消息队列适合任务间通信和事件通知。
  - 如果需要处理连续的数据流或高速数据采集，环形缓冲区可能更适合。

- **是否需要环形缓冲区**：
  - 如果数据量较大、实时性要求高或需要覆盖旧数据，环形缓冲区是一个有效的补充。
  - 如果消息队列已经满足需求，通常不需要额外的环形缓冲区。

根据具体应用场景选择合适的数据结构。如果需要进一步帮助，请告诉我！
