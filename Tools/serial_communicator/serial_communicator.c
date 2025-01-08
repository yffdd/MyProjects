// serial_communicator.c

// Virtual Serial Port Driver (VSPD) Dowload: https://macxin.lanzouf.com/i9ADd0qykmib

/*
 * 使用串口通信的示例代码：
 * 这个文件封装了打开串口、发送数据和关闭串口的基本功能。
 * 使用之前，请确保已经安装并配置了虚拟串口驱动（VSPD）。
 *
 * VSPD（Virtual Serial Port Driver）是一个用于创建虚拟串口对的软件，可以模拟物理串口设备。
 * 如果你要通过串口通信，需要先启动并配置虚拟串口对（比如 "COM1" 和 "COM2"），
 * 然后程序才能通过这些虚拟串口进行通信。
 *
 * 步骤如下：
 * 1. 打开并配置虚拟串口驱动（VSPD）。
 * 2. 使用本代码打开串口（如 COM2），发送数据到串口。
 * 3. 关闭串口。
 *
 * 示例：
 * // Example main.c
 * #include "serial_communicator.h"
 * 
 * int main() {
 *     // 打开串口
 *     Useserial();
 * 
 *     // 通过 WriteFile 发送数据
 *     const char* msg = "Hello, this is a test message!\n";
 *     SendMessageToSerial(msg);
 * 
 *     // 发送变量
 *     int value = 10;
 *     char message[2048];
 *     sprintf(message, "value=%d\r\n", value);
 *     SendMessageToSerial(message);
 * 
 *     // 关闭串口
 *     CloseSerial(comHandle);
 * 
 *     return 0;
 * }
 */

#include "serial_communicator.h"

HANDLE comHandle;

int Useserial(void) {
    // 打开串口 串口2 波特率115200 停止位8 无校验 停止位1
    comHandle = OpenSerial("COM2", CBR_115200, 8, NOPARITY, ONESTOPBIT);
    if (INVALID_HANDLE_VALUE == comHandle) {
        printf("Open serial port failed.\n");
        return -1;
    }
    printf("Serial port opened successfully.\n");
}

HANDLE OpenSerial(const char *com, int baud, int byteSize, int parity, int stopBits) {

    DCB dcb;
    BOOL b = FALSE;
    COMMTIMEOUTS CommTimeouts;    
    HANDLE comHandle = INVALID_HANDLE_VALUE;

    comHandle = CreateFile(com,            // 串口名称
        GENERIC_READ | GENERIC_WRITE,      // 可读、可写                 
        0,            // No Sharing                               
        NULL,         // No Security                              
        OPEN_EXISTING,// Open existing port only                     
        FILE_ATTRIBUTE_NORMAL,            // Non Overlapped I/O                            
        NULL);        // Null for Comm Devices

    if (INVALID_HANDLE_VALUE == comHandle) {
        printf("CreateFile fail\r\n");
        return comHandle;
    }

    b = SetupComm(comHandle, 2048, 2048);
    if (!b) {
        printf("SetupComm fail\r\n");
    }

    // 设置串口状态属性
    GetCommState(comHandle, &dcb);
    dcb.BaudRate = baud; 
    dcb.ByteSize = byteSize;
    dcb.Parity = parity;
    dcb.StopBits = stopBits;
    b = SetCommState(comHandle, &dcb);
    if (!b) {
        printf("SetCommState fail\r\n");
    }

    return comHandle;
}

void CloseSerial(HANDLE comHandle) {
    CloseHandle(comHandle);
    printf("Serial port closed.\n");
}

void SendMessageToSerial(const char* message) {
    DWORD bytesWritten;
    BOOL b = WriteFile(comHandle, message, strlen(message), &bytesWritten, NULL);
    if (!b) {
        printf("WriteFile fail\r\n");
    }
}

