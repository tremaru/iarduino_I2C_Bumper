[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_I2C\_Bumper

**This is a library for Arduino IDE. It allows to work with [Bumper](https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html) module made by iArduino.ru**

**Данная библиотека для Arduino IDE позволяет работать с модулем [Бампер с 7-ю датчиками линий](https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html) от [iArduino.ru](https://iarduino.ru)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте на нашей [wiki](https://wiki.iarduino.ru/page/line-bumper/#h3_3)


| Модель | Ссылка на магазин |
|---|---|
| <p></p> <img src="https://wiki.iarduino.ru/img/resources/1217/1217.svg" width="100px"></img>| https://iarduino.ru/shop/Expansion-payments/bamper-s-7-datchikami-liniy-i2c---flash.html |


## Назначение функций:

**Подключаем библиотеку** 

```C++
#include <iarduino_I2C_Bumper> // Подключаем библиотеку iarduino_I2C_Bumper для работы с модулем.
```

**Создаём объект** 

```C++
iarduino_I2C_Bumper ОБЪЕКТ; // Создаём объект для работы с методами библиотеки без указания адреса модуля на шине I2C.

iarduino_I2C_Bumper ОБЪЕКТ( АДРЕС ); // Создаём объект для работы с методами библиотеки указывая адрес модуля на шине I2C.
```

**Инициализация работы** 

```C++
ОБЪЕКТ.begin(); // Инициализация работы с модулем.
```

**Изменение адреса модуля** 

```C++
ОБЪЕКТ.changeAddress( АДРЕС ); // Изменение адреса модуля на шине I2C.
```

**Перезагрузка модуля**

```C++
ОБЪЕКТ.reset(); // Перезагрузка модуля.
```

**Получение текущего адреса** 

```C++
ОБЪЕКТ.getAddress(); // Получение текущего адреса модуля на шине I2C.
```

**Получение версии прошивки** 

```C++
ОБЪЕКТ.getVersion(); // Получение версии прошивки модуля.
```

**Управление фарам**

```C++
ОБЪЕКТ.setLamp( ФЛАГ ); // Управление фарами.
```

**Указание режима поворотников** 

```C++
ОБЪЕКТ.setTurnSignal( РЕЖИМ ); // Указание режима работы поворотников.
```

**Указание периода поворотников** 

```C++
ОБЪЕКТ.setTurnPeriod( ПЕРИОД ); // Указание периода мигания поворотников.
```

**Указание условий работы повортников** 

```C++
ОБЪЕКТ.settingsTurnAuto( ВКЛ , ВЫКЛ [, ФЛАГ] ); // Указание условий работы поворотников.
```

**Запуск автоматической калибровки** 

```C++
ОБЪЕКТ.setCalibrationAuto( [ ТИП ] ); // Запуск автоматической калибровки модуля.
```

**Выполнение ручной калибровки** 

```C++
ОБЪЕКТ.setCalibrationManual( ЛИНИЯ , ФОН ); // Выполнение ручной калибровки модуля.
```

**Чтение линий** 

```C++
ОБЪЕКТ.getLineDigital( НОМЕР ); // Чтение линий под датчиками модуля.
```

**Чтение значений АЦП** 

```C++
ОБЪЕКТ.getLineAnalog( НОМЕР ); // Чтение значений АЦП снятых с датчиков модуля.
```

**Запрос типа трассы** 

```C++
ОБЪЕКТ.getLineType(); // Запрос типа трассы для которой откалиброван модуль.
```

