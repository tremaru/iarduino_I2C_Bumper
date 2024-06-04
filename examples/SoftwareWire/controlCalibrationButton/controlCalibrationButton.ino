// ПРИМЕР КОНТРОЛЯ ЗА КАЛИБРОВКОЙ БАМПЕРА КНОПКОЙ:           // * Строки со звёздочкой являются необязательными.
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
    Serial.println("Установите бампер над линией трассы");   //   Выводим текст.
    Serial.println("Нажмите на кнопку «калибровка».\r\n");   //   Выводим текст.
    while(bum.getCalibrationStage()!=BUM_STAGE_READ_LN){;}   //   Ждём начала чтения показаний с линий трассы.
    Serial.println("Выполняется чтение линий трассы...");    //   Выводим текст.
    while(bum.getCalibrationStage()!=BUM_STAGE_WAITING){;}   //   Ждём перехода модуля в стадию ожидания команды на чтение фона.
    Serial.println("Чтение линий завершено.\r\n");           //   Выводим текст.
    Serial.println("Установите бампер над фоном трассы");    //   Выводим текст.
    Serial.println("Нажмите на кнопку «калибровка».\r\n");   //   Выводим текст.
    while(bum.getCalibrationStage()!=BUM_STAGE_READ_BG){;}   //   Ждём начала чтения показаний с фона трассы.
    Serial.println("Выполняется чтение фона трассы...");     //   Выводим текст.
    while(bum.getCalibrationStage()==BUM_STAGE_READ_BG){;}   //   Ждём завершения чтения показаний с фона трассы.
    Serial.println("Чтение фона завершено.\r\n");            //   Выводим текст.
    if(bum.getCalibrationStage()==BUM_STAGE_OFF_OK){         //   Если калибровка выполнена успешно, то...
        Serial.println("Калибровка выполнена успешно!");     //   Выводим текст.
    }else{                                                   //   Иначе ...
        Serial.println("Калибровка не выполнена!");          //   Выводим текст.
    }                                                        //
}                                                            //
                                                             //
void loop(){}                                                //
                                                             //
/*  ПЕРЕЧЕНЬ ЗНАЧЕНИЙ   возвращаемых функцией getCalibrationStage():
 *  BUM_STAGE_READ_LN - выполняется чтение АЦП с датчиков установленных над линией трассы.
 *  BUM_STAGE_WAITING - модуль ожидает поступления команды на чтение фона.
 *  BUM_STAGE_READ_BG - выполняется чтение АЦП с датчиков установленных над фоном трассы.
 *  BUM_STAGE_OFF_ERR - калибровка завершена провалом.
 *  BUM_STAGE_OFF_OK  - калибровка завершена успехом.
 */ 