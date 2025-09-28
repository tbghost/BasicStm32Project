#include <stdint.h>
//#include <iostream>
//#include "hal_init.h"

//void SystemClock_Config();
//void Error_Handler();

int main()
{
  //haL_init();
  
  uint32_t neverend{42};
  //std::cout << "BasicProject" << std::endl;
  while(neverend == 42)
  {

  }

  return 0;
}



//int main() {
//    HAL_Init();
//    SystemClock_Config();
//
//    while (1) {
//        // Blink LED oder weitere Logik
//    }
//}
//
//void SystemClock_Config() {
//    // Implementiere Taktkonfiguration hier
//}
//
//void Error_Handler() {
//    while (1) {
//        // Fehlerbehandlung
//    }
//}