// ДАННЫЙ ПРИМЕР ИЩЕТ ВСЕ МОДУЛИ НА ШИНЕ I2C И ВЫВОДИТ ИНФОРМАЦИЮ О НИХ.    // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>                //   https://iarduino.ru/file/627.html
// Требуется установить библиотеку <iarduino_I2C_Address.h>                 //   https://iarduino.ru/file/558.html
//                                                                          //
// - Пример findDevices_Small  выводит короткую информацию о модулях.       //
// - Пример findDevices_Medium выводит количество и информацию о модулях.   //
// - Пример findDevices_Big    работает с модулями не выводящими название.  //
                                                                            //
const uint8_t sum = 10;                                                     //   Определяем количество модулей на шине I2C. Допускается указывать значение превышающее реальное количество модулей.
                                                                            //
#include <iarduino_I2C_Software.h>                                          //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Address.
SoftTwoWire sWire(3,4);                                                     //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                            //
#include <iarduino_I2C_Address.h>                                           //   Подключаем библиотеку для работы с адресами модулей линейки I2C-flash.
iarduino_I2C_Address obj[sum];                                              //   Объявляем  массив объектов (obj), указав количество (sum) модулей I2C-flash на шине I2C. Адреса модулей будут определены автоматически.
                                                                            //   Если вам известны адреса модулей, то массив (obj) можно определить адресами, например: iarduino_I2C_Address obj[5]={10,20,12,54,67}.
void setup(){                                                               //
     delay(500);                                                            // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                                                    //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
     while(!Serial){;}                                                      // * Ждём завершения инициализации шины UART.
                                                                            //
     Serial.println( F("-----------------") );                              // *
     Serial.print  ( F("Чтение... ") );                                     // *
                                                                            //
//   Находим модули:                                                        //
     for(uint8_t i=0; i<sum; i++){ obj[i].begin(&sWire); } // &Wire...      //   Функция begin() принимает ссылку на объект для работы с шиной I2C и проверяет существование на этой шине очередного модуля.
                                                                            //   После begin(), результат обнаружения модуля остаётся доступным для чтения: if( obj[i] ){/*модуль был обнаружен*/;}else{/*нет*/;}
//   Подсчитываем количество найденных модулей:                             //
     uint8_t num        = 0;                                                //
     uint8_t sumFlash   = 0;                                                // * Количество найденных модулей    из линейки flash-I2C.
     uint8_t sumUnknown = 0;                                                // * Количество найденных модулей НЕ из линейки flash-I2C.
     uint8_t sumDouble  = 0;                                                // * Количество адресов занятых несколькими модулями flash-I2C.
     while(  obj[num] ){                                                    // *
         if( obj[num].getDevice()==DEVICE_I2C_FLASH     ){ sumFlash++;   }  // * Подсчитываем количество модулей    из линейки flash-I2C.
         if( obj[num].getDevice()==DEVICE_I2C_FLASH_OLD ){ sumFlash++;   }  // * Подсчитываем количество модулей    из линейки flash-I2C без поддержки вывода названия и сортировки адреса.
         if( obj[num].getDevice()==DEVICE_I2C_UNKNOWN   ){ sumUnknown++; }  // * Подсчитываем количество модулей НЕ из линейки flash-I2C.
         if( obj[num].getDevice()==DEVICE_I2C_DOUBLE    ){ sumDouble++;  }  // * Подсчитываем количество адресов занятых несколькими модулями flash-I2C.
         num++; if( num>=sum ){ break; }                                    // *
     }                                                                      // *
                                                                            //
//   Выводим количество найденных модулей:                                  //
     Serial.println( F("выполнено!")                           );           // *
     Serial.print  ( F("Найдено модулей: ")                    );           // *
     Serial.print  ( num                                       );           // *
     Serial.println( '.'                                       );           // *
     Serial.print  ( F("Из них: Flash-I2C=")                   );           // *
     Serial.print  ( sumFlash                                  );           // *
     Serial.print  ( F(", неизвестные=")                       );           // *
     Serial.print  ( sumUnknown                                );           // *
     Serial.print  ( F(", дублированные адреса=")              );           // *
     Serial.print  ( sumDouble                                 );           // *
     Serial.println( '.'                                       );           // *
                                                                            //
//   Выводим информацию о найденных модулях:                                //
     for(uint8_t i=0; i<sum; i++){                                          //   Вместо цикла for можно воспользоваться циклом while, как сделано выше, при подсчёте количества найденных модулей.
         if( obj[i] ){                                                      //   Если модуль не существует, то obj[i] вернёт false.
         //  Выводим порядковый номер модуля:                               //
             Serial.print( i+1                                 );           // *
             Serial.print( F(".) ")                            );           // *
         //  Если модуль принадлежит линейке Flash I2C:                     //
             if( obj[i].getDevice()==DEVICE_I2C_FLASH                       //
             ||  obj[i].getDevice()==DEVICE_I2C_FLASH_OLD ){                //
             //  Выводим название модуля:                                   //
                 Serial.print( F("Модуль Flash I2C")           );           // *
                 if( obj[i].getDevice()==DEVICE_I2C_FLASH )                 // * Если модуль Flash I2C поддерживает вывод своего названия, то ...
                 Serial.print((String)" «"+obj[i].getName()+"»");           // * Выводим название модуля.
             //  Выводим адрес модуля на шине I2C:                          //
                 Serial.print( F(", адрес на шине I2C = 0x")   );           // 
                 Serial.print( obj[i].getAddress(), HEX        );           // 
             //  Выводим номер модели:                                      //
                 Serial.print( F(", идентификатор = 0x")       );           // *
                 Serial.print( obj[i].getModel(), HEX          );           // *
             //  Выводим версию прошивки модуля:                            //
                 Serial.print( F(", версия прошивки ")         );           // *
                 Serial.print( obj[i].getVersion()             );           // *
             //  Выводим состояние внутрисхемной подтяжки модуля:           //
                 Serial.print( F(", подтяжка линий шины I2C ") );           // *
                 Serial.print( obj[i].getPullI2C()? "в":"от"   );           // *
                 Serial.print( F("ключена")                    );           // *
             }else                                                          //
         //  Если модуль не принадлежит линейке Flash I2C:                  //
             if( obj[i].getDevice()==DEVICE_I2C_UNKNOWN ){                  //
             //  Выводим адрес модуля не принадлежащего к линейе Flash I2C: //
                 Serial.print( F("Неизвестный модуль")         );           // 
                 Serial.print( F(", адрес на шине I2C = 0x")   );           // 
                 Serial.print( obj[i].getAddress(), HEX        );           // 
             }else                                                          //
         //  Если на адресе обнаружено несколько модулей Flash I2C:         //
             if( obj[i].getDevice()==DEVICE_I2C_DOUBLE ){                   //
             //  Выводим адрес занятый несколькими модулями Flash I2C:      //
                 Serial.print( F("Адрес на шине I2C = 0x")     );           // 
                 Serial.print( obj[i].getAddress(), HEX        );           // 
                 Serial.print( F(" занят несколькими модулями"));           // 
             }                                                              //
         //  Переходим к следующему модулю:                                 //
             Serial.println('.');                                           //
         }                                                                  //
     }                                                                      //
}                                                                           //
                                                                            //
void loop(){                                                                //
}                                                                           //