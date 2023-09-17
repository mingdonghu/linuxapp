#include "serial_interface_linux.h"

#include <unistd.h>

#include <bitset>

std::string GetCurrentLocalTimeStamp() {
	//// 获取系统时间戳 uint is seconds
	std::string stamp_str;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp = 
		std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
	uint64_t timestamp = (uint64_t)tmp.count();
	char s_stamp[100] = {0};
#ifdef __LP64__
	snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
#ifdef _WIN64
	snprintf(s_stamp, 100, "[%lu.%lu]", (timestamp/1000000000), (timestamp%1000000000));
#else
	snprintf(s_stamp, 100, "[%llu.%llu]", (timestamp/1000000000), (timestamp%1000000000));
#endif
#endif
	stamp_str = s_stamp;

	return stamp_str;
}


int main(int argc, char const *argv[]) {

  SerialInterfaceLinux* cmd_port = new SerialInterfaceLinux;

  cmd_port->SetReadCallback([](const char* data, size_t length){
    std::cout << "read from IO:" << GetCurrentLocalTimeStamp() << " ";
    for(int i = 0; i < length; i++) {
      std::cout << "0x" << std::hex << static_cast<int>(data[i]) << "-- 0b" << std::bitset<8>{static_cast<int>(data[i])} << " ";
    }
    std::cout << std::endl;
  });

  std::string port_name("/dev/ttyUSB0");
  if(!cmd_port->Open(port_name)) {
    std::cout << "open error" << std::endl;
    return -1;
  }

  uint8_t test_data[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
  uint32_t ret_outlen;
  
  for (;;) {
    if(!cmd_port->WriteToIo(&test_data[0], 5, &ret_outlen)) {
      std::cout << "writeto IO error" << std::endl;
    }
    std::cout << "write to IO: " << ret_outlen << std::endl;
    sleep(1);
  }

  return 0;
}
