// ПРИМЕР ЧТЕНИЯ ЛИНИЙ ПОД ДАТЧИКАМИ БАМПЕРА:              // * Строки со звёздочкой являются необязательными.
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
    Serial.print("Линия находится под датчиком:");         //   Выводим текст.
    if(bum.getLineDigital(1)){ Serial.print(" 1");}        //   Выводим номер 1 датчика, если под ним находится линия.
    if(bum.getLineDigital(2)){ Serial.print(" 2");}        //   Выводим номер 2 датчика, если под ним находится линия.
    if(bum.getLineDigital(3)){ Serial.print(" 3");}        //   Выводим номер 3 датчика, если под ним находится линия.
    if(bum.getLineDigital(4)){ Serial.print(" 4");}        //   Выводим номер 4 датчика, если под ним находится линия.
    if(bum.getLineDigital(5)){ Serial.print(" 5");}        //   Выводим номер 5 датчика, если под ним находится линия.
    if(bum.getLineDigital(6)){ Serial.print(" 6");}        //   Выводим номер 6 датчика, если под ним находится линия.
    if(bum.getLineDigital(7)){ Serial.print(" 7");}        //   Выводим номер 7 датчика, если под ним находится линия.
    if(bum.getLineDigital(8)){ Serial.print(" 8");}        //   Выводим номер 8 датчика, если под ним находится линия.
    if(bum.getLineDigital(9)){ Serial.print(" 9");}        //   Выводим номер 9 датчика, если под ним находится линия.
    Serial.println(".");                                   //   Завершаем строку.
    delay(200);                                            // * Задержка позволяет медленнее заполнять монитор последовательного порта.
}                                                          //