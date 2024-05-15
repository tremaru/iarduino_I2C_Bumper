// ПРИМЕР ПОЛУЧЕНИЯ И УКАЗАНИЯ ТИПА ЛИНИИ ТРАССЫ:          // * Строки со звёздочкой являются необязательными.
                                                           //
// Бампер с 9 датчиками линий с шагом  7мм., FLASH-I2C:    //   https://iarduino.ru/shop/Sensory-Datchiki/bamper-s-9-datchikami-liniy-s-shagom-7mm-flash-i2c.html
// Бампер с 9 датчиками линий с шагом 14мм., FLASH-I2C:    //   https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html
// Информация о подключении модулей к шине I2C:            //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:              //   https://wiki.iarduino.ru/page/line-bumper/
                                                           //
#include <Wire.h>                                          //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Bumper.
#include <iarduino_I2C_Bumper.h>                           //   Подключаем библиотеку для работы с бампером I2C-flash.
iarduino_I2C_Bumper bum(0x09);                             //   Объявляем объект bum для работы с функциями и методами библиотеки iarduino_I2C_Bumper, указывая адрес модуля на шине I2C.
                                                           //   Если объявить объект без указания адреса (iarduino_I2C_Bumper bum;), то адрес будет найден автоматически.
void setup(){                                              //
    delay(500);                                            // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(9600);                                    //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                                      // * Ждём завершения инициализации шины UART.
    bum.begin(&Wire); // &Wire1, &Wire2 ...                //   Инициируем работу с бампером, указав ссылку на объект для работы с шиной I2C на которой находится бампер (по умолчанию &Wire).
}                                                          //
                                                           //
void loop(){                                               //
//  ВЫВОДИМ ИСПОЛЬЗУЕМЫЙ ТИП ЛИНИИ:                        //
    Serial.print("Модуль использовал трассу с ");          //
    switch(bum.getLineType()){                             //   Получаем используемый модулем тип линии.
      case BUM_LINE_BLACK: Serial.print("тёмной" ); break; //
      case BUM_LINE_WHITE: Serial.print("светлой"); break; //
      default:             Serial.print("неизвестной");    // * 
    }                      Serial.print(" линией, ");      //
//  УКАЗЫВАЕМ НОВЫЙ ТИП ЛИНИИ:                             //
    bum.setLineType(BUM_LINE_CHANGE);                      //   Тип линии задаётся как BUM_LINE_BLACK - тёмная / BUM_LINE_WHITE - светлая / BUM_LINE_CHANGE - сменить тип линии.
//  ВЫВОДИМ ИСПОЛЬЗУЕМЫЙ ТИП ЛИНИИ:                        //
    Serial.print("а теперь использует трассу с ");         //
    switch(bum.getLineType()){                             //   Получаем используемый модулем тип линии.
      case BUM_LINE_BLACK: Serial.print("тёмной" ); break; //
      case BUM_LINE_WHITE: Serial.print("светлой"); break; //
      default:             Serial.print("неизвестной");    // * 
    }                      Serial.print(" линией.\r\n");   //
//  добавляем задержку:                                    //
    delay(2000);                                           //
}                                                          //
                                                           //
/* Тип линии, равно как и калибровочные значения,          //
 * хранятся в энергонезависимой памяти модуля.             //
 * Значит тип линии сохранится и после отключения питания. //
 */                                                        //