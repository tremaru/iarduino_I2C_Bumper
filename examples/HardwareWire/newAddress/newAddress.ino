// ПРИМЕР СМЕНЫ АДРЕСА МОДУЛЯ:                             // * Строки со звёздочкой являются необязательными.
                                                           //
// Бампер с 9 датчиками линий с шагом  7мм., FLASH-I2C:    //   https://iarduino.ru/shop/Sensory-Datchiki/bamper-s-9-datchikami-liniy-s-shagom-7mm-flash-i2c.html
// Бампер с 9 датчиками линий с шагом 14мм., FLASH-I2C:    //   https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html
// Информация о подключении модулей к шине I2C:            //   https://wiki.iarduino.ru/page/i2c_connection/
// Информация о модуле и описание библиотеки:              //   https://wiki.iarduino.ru/page/line-bumper/
                                                           //
// Данный скетч демонстрирует не только                    //
// возможность смены адреса на указанный в                 //
// переменной newAddress, но и обнаружение,                //
// и вывод текущего адреса модуля на шине I2C.             //
                                                           //
// Библиотека <iarduino_I2C_Bumper.h> предназначена        //
// для работы с бамперами и меняет только их адреса.       //
// Но, в разделе Файл/Примеры/.../findSortDevices          //
// находятся примеры смены и сортировки адресов            //
// любых модулей iarduino серии Flash I2C.                 //
                                                           //
uint8_t newAddress = 0x09;                                 //   Назначаемый модулю адрес (0x07 < адрес < 0x7F).
                                                           //
#include <Wire.h>                                          //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Bumper.
#include <iarduino_I2C_Bumper.h>                           //   Подключаем библиотеку для работы с бампером I2C-flash.
iarduino_I2C_Bumper bum;                                   //   Объявляем объект bum для работы с функциями и методами библиотеки iarduino_I2C_Bumper.
                                                           //   Если при объявлении объекта указать адрес, например, bum(0xBB), то пример будет работать с тем модулем, адрес которого был указан.
void setup(){                                              //
    delay(500);                                            // * Ждём завершение переходных процессов связанных с подачей питания.
    Serial.begin(9600);                                    //
    while(!Serial){;}                                      // * Ждём завершения инициализации шины UART.
    if( bum.begin(&Wire) ){                                //   Инициируем работу с энкодером, указав ссылку на объект для работы с шиной I2C на которой находится энкодер (Wire, Wire1, Wire2 ...).
        Serial.print("Найден бампер с адресом 0x");        //
        Serial.println( bum.getAddress(), HEX );           //   Выводим текущий адрес модуля.
        if( bum.changeAddress(newAddress) ){               //   Меняем адрес модуля на newAddress.
            Serial.print("Адрес изменён на 0x");           //
            Serial.println(bum.getAddress(),HEX );         //   Выводим текущий адрес модуля.
        }else{                                             //
            Serial.println("Адрес не изменён!");           //
        }                                                  //
    }else{                                                 //
        Serial.println("Бампер не найден!");               //
    }                                                      //
}                                                          //
                                                           //
void loop(){                                               //
}                                                          //
