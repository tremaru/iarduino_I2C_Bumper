// ПРИМЕР ЧТЕНИЯ АНАЛОГОВЫХ ЗНАЧЕНИЙ С ДАТЧИКОВ ЛИНИЙ:       // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h> //   https://iarduino.ru/file/627.html
                                                             //
// Бампер с 9 датчиками линий с шагом  7мм., FLASH-I2C:      //   https://iarduino.ru/shop/Sensory-Datchiki/bamper-s-9-datchikami-liniy-s-shagom-7mm-flash-i2c.html
// Бампер с 9 датчиками линий с шагом 14мм., FLASH-I2C:      //   https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html
// Информация о подключении модулей к шине I2C:              //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:                //   https://wiki.iarduino.ru/page/line-bumper/
                                                             //
#include <iarduino_I2C_Software.h>                           //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Bumper.
SoftTwoWire sWire(3,4);                                      //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                             //
#include <iarduino_I2C_Bumper.h>                             //   Подключаем библиотеку для работы с бампером I2C-flash.
iarduino_I2C_Bumper bum(0x09);                               //   Объявляем объект bum для работы с функциями и методами библиотеки iarduino_I2C_Bumper, указывая адрес модуля на шине I2C.
                                                             //   Если объявить объект без указания адреса (iarduino_I2C_Bumper bum;), то адрес будет найден автоматически.
void setup(){                                                //
    delay(500);                                              // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(9600);                                      //   Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                                        // * Ждём завершения инициализации шины UART.
    bum.begin(&sWire);                                       //   Инициируем работу с бампером, указав ссылку на объект для работы с шиной I2C на которой находится бампер (по умолчанию &Wire).
}                                                            //
                                                             //
void loop(){                                                 //
    Serial.print("Аналоговые значения датчиков 1-9: ");      //   Выводим текст.
    Serial.print( bum.getLineAnalog(1) );                    //   Выводим значение АЦП снятое с 1 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(2) );                    //   Выводим значение АЦП снятое с 2 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(3) );                    //   Выводим значение АЦП снятое с 3 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(4) );                    //   Выводим значение АЦП снятое с 4 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(5) );                    //   Выводим значение АЦП снятое с 5 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(6) );                    //   Выводим значение АЦП снятое с 6 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(7) );                    //   Выводим значение АЦП снятое с 7 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(8) );                    //   Выводим значение АЦП снятое с 8 датчика линии.
    Serial.print(", ");                                      //   Выводим текст.
    Serial.print( bum.getLineAnalog(9) );                    //   Выводим значение АЦП снятое с 9 датчика линии.
    Serial.println(".");                                     //   Завершаем строку.
    delay(200);                                              // * Задержка позволяет медленнее заполнять монитор последовательного порта.
}                                                            //
                                                             //
/*  ПРИМЕЧАНИЕ:
 *  Чем светлее объект находящийся под датчиком линии, тем выше показания АЦП.
 *  Оперируя аналоговыми значениями бампера, можно использовать трассы с разметками разных цветов.
 */