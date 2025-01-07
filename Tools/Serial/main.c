#include "serial_communicator.h"

int main() {

    // 打开串口
    Useserial();

    // 通过 WriteFile 发送数据
    const char* msg = "Hello, this is a test message!\n";
    SendMessageToSerial(msg);

    // 发送变量
    int value = 10;
    char message[2048];
    sprintf(message, "value=%d\r\n", value);
    SendMessageToSerial(message);

    // 关闭串口
    CloseSerial(comHandle);

    return 0;
}