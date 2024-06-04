// ПРИМЕР УПРАВЛЕНИЯ ПОВОРОТНИКАМИ НА БАМПЕРЕ:               // * Строки со звёздочкой являются необязательными.
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
    bum.begin(&sWire);                                       //   Инициируем работу с бампером, указав ссылку на объект для работы с шиной I2C на которой находится бампер (по умолчанию &Wire).
    bum.setTurnPeriod(BUM_TURN_400);                         // * Задаём скорость мигания поворотников в 400мс. Возможные значения: BUM_TURN_100, BUM_TURN_200, BUM_TURN_400 и BUM_TURN_800.
}                                                            //
                                                             //
void loop(){                                                 //
    bum.setTurnSignal(BUM_TURN_LEFT);                        //   Включаем левый поворотник.
    delay(5000);                                             //   Ждём 5 секунд.
    bum.setTurnSignal(BUM_TURN_RIGHT);                       //   Включаем правый поворотник.
    delay(5000);                                             //   Ждём 5 секунд.
    bum.setTurnSignal(BUM_TURN_EMERGENCY);                   //   Включаем поворотниками аварийный сигнал.
    delay(5000);                                             //   Ждём 5 секунд.
    bum.setTurnSignal(BUM_TURN_POLICE);                      //   Включаем поворотники в полицейский режим.
    delay(5000);                                             //   Ждём 5 секунд.
    bum.setTurnSignal(BUM_TURN_OFF);                         //   Отключаем поворотники.
    delay(5000);                                             //   Ждём 5 секунд.
}                                                            //