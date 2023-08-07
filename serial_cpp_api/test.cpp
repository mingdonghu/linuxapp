#include "serial_interface_linux.h"

#include <bitset>

int main(int argc, char const *argv[]) {

  SerialInterfaceLinux* cmd_port = new SerialInterfaceLinux;

  cmd_port->SetReadCallback([](const char* data, size_t length){
    std::cout << "read from IO:";
    for(int i = 0; i < length; i++) {
      std::cout << "0x" << std::hex << static_cast<int>(data[i]) << "-- 0b" << std::bitset<8>{static_cast<int>(data[i])} << " ";
    }
    std::cout << std::endl;
  });

  std::string port_name("/dev/ttyUSB0");
  if(!cmd_port->Open(port_name, 115200)) {
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
