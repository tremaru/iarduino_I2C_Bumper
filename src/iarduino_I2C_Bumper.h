//	Библиотека для работы с Trema-модулем энкодер-потенциометр, I2C-flash для Arduino: https://iarduino.ru/
//  Версия: 1.0.1
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/
//  Подробное описание функций бибилиотеки доступно по ссылке: https://wiki.iarduino.ru/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел.
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_Bumper_h																											//
#define iarduino_I2C_Bumper_h																											//
																																		//
#if defined(ARDUINO) && (ARDUINO >= 100)																								//
#include		<Arduino.h>																												//
#else																																	//
#include		<WProgram.h>																											//
#endif																																	//
																																		//
#include		<iarduino_I2C_Bumper_I2C.h>																								//	Подключаем файл iarduino_I2C_Bumper.h - для работы с шиной I2C		(используя функции структуры iI2C)
																																		//
#define			DEF_CHIP_ID_FLASH			0x3C																						//	ID линейки чипов - константа для всех чипов серии Flash (позволяет идентифицировать принадлежность чипа к серии).
#define			DEF_CHIP_ID_METRO			0xC3																						//	ID линейки чипов - константа для всех чипов серии Metro (позволяет идентифицировать принадлежность чипа к серии).
#define			DEF_MODEL_BUM				0x10																						//	Идентификатор модели - константа.
//				Адреса регистров модуля:																								//
#define			REG_FLAGS_0					0x00																						//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_0					0x01																						//	Адрес регистра битов  управления для чтения и записи.
#define			REG_FLAGS_1					0x02																						//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_1					0x03																						//	Адрес регистра битов  управления для чтения и записи.
#define			REG_MODEL					0x04																						//	Адрес регистра содержащего номер типа модуля.
#define			REG_VERSION					0x05																						//	Адрес регистра содержащего версию прошивки.
#define			REG_ADDRESS					0x06																						//	Адрес регистра содержащего текущий адрес модуля на шине I2C. Если адрес указан с флагом IF-PIN-ADDRES то адрес установится (и запишется в регистр) только при наличии 1 на входе PIN_ADDRES.
#define			REG_CHIP_ID					0x07																						//	Адрес регистра содержащего ID линейки чипов «Flash». По данному ID можно определить принадлежность чипа к линейки «Flash».
#define			REG_BUM_FLG_LINE			0x10																						//	Адрес регистра флагов наличия и типа линии.
#define			REG_BUM_BIT_LAMP			0x11																						//	Адрес регистра битов управления фарами и поворотниками.
#define			REG_BUM_CALIBRATION			0x12																						//	Адрес регистра калибровки. Запись значения 0x1B, 0x09 или 0x50 запускает калибровку модуля.
#define			REG_BUM_THRESHOLD			0x13																						//	Адрес регистра хранящего значение АЦП среднее между фоном и линией   (младший байт), для первого датчика линии.
#define			REG_BUM_ANALOG				0x21																						//	Адрес регистра хранящего значение АЦП снятое с первого датчика линии (младший байт).
//				Позиция битов и флагов:																									//
#define			BUM_FLG_LINE_BLACK			0x80																						//	Флаг типа линии.			Указывает на то, что используется: «1» - тёмная линия на светлом фоне, «0» - светлая линия на тёмном фоне.
#define			BUM_FLG_LINE				0x7F																						//	Флаги налиция линии.		Указывают на то, что под датчиками, номера которых совпадают с номерами битов, присутствует линия.
#define			BUM_SET_HEAD_ON				0x80																						//	Бит управления фарами.		Установка бита включает фары.
#define			BUM_SET_TURN_AUTO_STOP		0x60																						//	Биты откл. повротников.		Определяют условие включения поворотников в автоматическом режиме:
#define			BUM_SET_TURN_AUTO_STOP_0	B00000000																					//								0 - Поворотник отключается как только линия окажется под любым датчиком кроме указанных в условии «TURN_MODE_AUTO_X».
#define			BUM_SET_TURN_AUTO_STOP_1	B00100000																					//								1 - Поворотник отключается как только линия окажется под центральным датчиком.
#define			BUM_SET_TURN_AUTO_STOP_2	B01000000																					//								2 - Как условие № 0, но продолжительнее на два мигания.
#define			BUM_SET_TURN_AUTO_STOP_3	B01100000																					//								3 - Как условие № 1, но продолжительнее на два мигания.
#define			BUM_SET_TURN_PERIOD			0x18																						//	Биты частоты поворотников.	Указывают период миганий поворотников:
#define			BUM_SET_TURN_PERIOD_100		B00000000																					//								0 - Период 100мс =>  50мс вкл /  50мс выкл.
#define			BUM_SET_TURN_PERIOD_200		B00001000																					//								1 - Период 200мс => 100мс вкл / 100мс выкл.
#define			BUM_SET_TURN_PERIOD_400		B00010000																					//								2 - Период 400мс => 200мс вкл / 200мс выкл.
#define			BUM_SET_TURN_PERIOD_800		B00011000																					//								3 - Период 800мс => 400мс вкл / 400мс выкл.
#define			BUM_SET_TURN_MODE			0x07																						//	Биты режима поворотников.	Указывают режим работы поворотников:
#define			BUM_SET_TURN_MODE_OFF		B00000000																					//								0 - Отключены.
#define			BUM_SET_TURN_MODE_LEFT		B00000001																					//								1 - Включён левый  поворотник.
#define			BUM_SET_TURN_MODE_RIGHT		B00000010																					//								2 - Включён правый поворотник.
#define			BUM_SET_TURN_MODE_EMERGENCY	B00000011																					//								3 - Включены оба поворотника (аварийка).
#define			BUM_SET_TURN_MODE_POLICE	B00000100																					//								4 - Режим полицейской машины.
#define			BUM_SET_TURN_MODE_AUTO_1	B00000101																					//								5 - Поворотник включается автоматически, при наличии линии под одним, самым крайним, датчиком со стороны поворотника.
#define			BUM_SET_TURN_MODE_AUTO_2	B00000110																					//								6 - Поворотник включается автоматически, при наличии линии под одним из двух крайних датчиков со стороны поворотника.
#define			BUM_SET_TURN_MODE_AUTO_3	B00000111																					//								7 - Поворотник включается автоматически, при наличии линии под одним из трёх крайних датчиков со стороны поворотника.
#define			BUM_FLG_CALIBRATION			0x80																						//	Флаг результата калибровки.	Указывает на то, что калибровка завершена: «0» - провалом, «1» - успехом.
#define			BUM_STAGE_CALIBRATION		0x60																						//  Биты указывают текущую стадию калибровки:
#define			BUM_STAGE_OFF				B00000000																					//								0 - Калибровка не выполняется.
#define			BUM_STAGE_READ_LN			B00100000																					//								1 - Выполняется чтение АЦП линии.
#define			BUM_STAGE_WAITING			B01000000																					//								2 - Ожидается разрешение на чтение АЦП фона трассы.
#define			BUM_STAGE_READ_BG			B01100000																					//								3 - Выполняется чтение АЦП фона.
#define			BUM_SET_CALIBRATION			0x1F																						//	Биты получения кодов запуска и завершения калибровки:
#define			BUM_CALIBR_RUN				B00011011																					//								0x1B - Код запуска    калибровки (чтение АЦП линии).
#define			BUM_CALIBR_END				B00001001																					//								0x09 - Код завершения калибровки (чтение АЦП фона и рассчёт граничных значений для каждого датчика модуля).
#define			BUM_CALIBR_END_A			B00010010																					//								0x12 - Код завершения калибровки (чтение АЦП фона и рассчёт граничного значения общего для всех датчиков модуля).
																																		//
#define			BUM_STAGE_OFF_OK			B10000000																					//								0 - Калибровка не выполняется.
#define			BUM_STAGE_OFF_ERR			B00000000																					//								0 - Калибровка не выполняется.
																																		//
#ifndef			BUM_TURN_OFF																											//
#define			BUM_TURN_OFF				0																							//	setTurnSignal( отключить ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_LEFT																											//
#define			BUM_TURN_LEFT				1																							//	setTurnSignal( левый поворотник ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_RIGHT																											//
#define			BUM_TURN_RIGHT				2																							//	setTurnSignal( правый поворотник ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_EMERGENCY																										//
#define			BUM_TURN_EMERGENCY			3																							//	setTurnSignal( аварийный сигнал ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_POLICE																											//
#define			BUM_TURN_POLICE				4																							//	setTurnSignal( полицейский режим ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_AUTO																											//
#define			BUM_TURN_AUTO				5																							//	setTurnSignal( автоматический режим ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_100																											//
#define			BUM_TURN_100				6																							//	setTurnPeriod( скорость мигания ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_200																											//
#define			BUM_TURN_200				7																							//	setTurnPeriod( скорость мигания ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_400																											//
#define			BUM_TURN_400				8																							//	setTurnPeriod( скорость мигания ).
#endif																																	//
																																		//
#ifndef			BUM_TURN_800																											//
#define			BUM_TURN_800				9																							//	setTurnPeriod( скорость мигания ).
#endif																																	//
																																		//
#ifndef			BUM_AUTO_ON_1																											//
#define			BUM_AUTO_ON_1				10																							//	settingsTurnAuto( условие включения поворотников - линия на 1 крайнем датчике, условие отключения поворотников, добавить два мигания перед выключением ).
#endif																																	//
																																		//
#ifndef			BUM_AUTO_ON_2																											//
#define			BUM_AUTO_ON_2				11																							//	settingsTurnAuto( условие включения поворотников - линия на 1 из 2 крайних датчиков, условие отключения поворотников, добавить два мигания перед выключением ).
#endif																																	//
																																		//
#ifndef			BUM_AUTO_ON_3																											//
#define			BUM_AUTO_ON_3				12																							//	settingsTurnAuto( условие включения поворотников - линия на 1 из 3 крайних датчиков, условие отключения поворотников, добавить два мигания перед выключением ).
#endif																																	//
																																		//
#ifndef			BUM_AUTO_OFF_ANY																										//
#define			BUM_AUTO_OFF_ANY			13																							//	settingsTurnAuto( условие включения поворотников, условие отключения поворотников - наличие линии под любым датчиком кроме тех которые заданы в условии включения поворотников, добавить два мигания перед выключением ).
#endif																																	//
																																		//
#ifndef			BUM_AUTO_OFF_CEN																										//
#define			BUM_AUTO_OFF_CEN			14																							//	settingsTurnAuto( условие включения поворотников, условие отключения поворотников - наличие линии под центральным датчиком, добавить два мигания перед выключением ).
#endif																																	//
																																		//
#ifndef			BUM_LINE_ALL																											//
#define			BUM_LINE_ALL				15																							//	getLineDigital( номер датчика линии ) - вернёт значение от 0 до 127.
#endif																																	//
																																		//
#ifndef			BUM_LINE_WHITE																											//
#define			BUM_LINE_WHITE				2																							//	setLineType( тип линии ). То же значение вернёт функция getLineType().
#endif																																	//
																																		//
#ifndef			BUM_LINE_BLACK																											//
#define			BUM_LINE_BLACK				3																							//	setLineType( тип линии ). То же значение вернёт функция getLineType().
#endif																																	//
																																		//
#ifndef			BUM_LINE_CHANGE																											//
#define			BUM_LINE_CHANGE				4																							//	setLineType( сменить тип линии ).
#endif																																	//
																																		//
class iarduino_I2C_Bumper{																												//
	public:																																//
	/**	Конструктор класса **/																											//
		iarduino_I2C_Bumper								(uint8_t address=0){															//	Конструктор класса													(Параметр: адрес модуля на шине I2C, если не указан (=0), то адрес будет определён).
															if(address>0x7F){ address>>=1; }											//	Корректируем адрес, если он указан с учётом бита RW.
								valAddrTemp				=	address;																	//	Сохраняем переданный адрес модуля.
								objI2C					=	new iarduino_I2C;															//	Переопределяем указатель objI2C на объект производного класса iarduino_I2C.
		}																																//
	/**	Пользовательские функции **/																									//
		bool					begin					(void							);												//	Объявляем  функцию инициализации модуля								(Параметр:  отсутствует).
		bool					reset					(void							);												//	Объявляем  функцию перезагрузки модуля								(Параметр:  отсутствует).
		bool					changeAddress			(uint8_t						);												//	Объявляем  функцию смены адреса модуля на шине I2C					(Параметр:  новый адрес модуля).
		uint8_t					getAddress				(void							){ return valAddr;	}							//	Определяем функцию возвращающую текущий адрес модуля на шине I2C	(Параметр:  отсутствует).
		uint8_t					getVersion				(void							){ return valVers;	}							//	Определяем функцию возвращающую текущую версию прошивки модуля		(Параметр:  отсутствует).
		bool					setLamp					(bool							);												//	Объявляем  функцию управления фарами								(Параметр:  true-вкл / false-выкл).
		bool					setTurnSignal			(uint8_t						);												//	Объявляем  функцию управления поворотниками							(Параметры: режим работы поворотников).
		bool					setTurnPeriod			(uint8_t						);												//	Объявляем  функцию установки периода мигания поворотников			(Параметр:  период).
		bool					settingsTurnAuto		(uint8_t, uint8_t, bool=false	);												//	Объявляем  функцию навтройки работы поворотников в авто-режиме		(Параметры: условие включения, условие отключения, [добавить 2 мигания перед отключением true/false]).
		bool					setCalibrationRun		(void							);												//	Объявляем  функцию запуска    автоматической калибровки модуля		(Параметр:  отсутствует).
		bool					setCalibrationEnd		(bool=false						);												//	Объявляем  функцию завершения автоматической калибровки модуля		(Параметр:  флаг установки общей границы АЦП для всех датчиков).
		bool					setCalibrationManual	(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);				//	Объявляем  функцию запуска ручной калибровки модуля					(Параметры: граница АЦП между линией и фоном 1 датчика, 2 датчика, ... , 7 датчика).
		bool					setCalibrationManual	(uint16_t i						){return setCalibrationManual(i,i,i,i,i,i,i);}	//	Объявляем  функцию запуска ручной калибровки модуля					(Параметры: граница АЦП между линией и фоном для всех датчиков модуля).
		uint8_t					getCalibrationStage		(void							);												//	Объявляем  функцию получения текущего статуса калибровки модуля		(Параметр:  отсутствует).
		uint8_t					getLineDigital			(uint8_t						);												//	Объявляем  функцию возвращающую флаг наличия линии под датчиком		(Параметр:  номер датчика).
		uint16_t				getLineAnalog			(uint8_t						);												//	Объявляем  функцию возвращающую значение АЦП снятое с датчика		(Параметр:  номер датчика).
template<typename T> uint8_t	getLineSum				(T &a){uint8_t i,j=0; a=(T)getLineDigital(BUM_LINE_ALL); for(i=1;i<8;i++){j+=a&bit(i)?1:0;} return j;} //												(Параметр:  переменная для получения флагов наличия линий).
		uint8_t					getLineSum				(void							){ uint8_t a=0; return getLineSum(a); }			//	Определяем функцию возвращающую количество датчиков над линией		(Параметр:  отсутствует).
		uint8_t					getLineType				(void							);												//	Объявляем  функцию возвращающую тип линии для которой откалиброван	(Параметр:  отсутствует).
		bool					setLineType				(uint8_t						);												//	Объявляем  функцию установки типа линии трассы						(Параметр:  тип линии).
	private:																															//
	/**	Внутренние переменные **/																										//
		uint8_t					valAddrTemp				=	0;																				//	Определяем переменную для хранения адреса модуля на шине I2C который был указан, но не был проверен.
		uint8_t					valAddr					=	0;																				//	Определяем переменную для хранения адреса модуля на шине I2C.
		uint8_t					valVers					=	0;																				//	Определяем переменную для хранения версии прошивки модуля.
		uint8_t					valAutoStartStop		=	BUM_SET_TURN_MODE_AUTO_1 | BUM_SET_TURN_AUTO_STOP_3;							//	Определяем переменную для хранения условий включения и выключения поворотников.
		uint8_t					valRegBitLamp;																								//	Объявляем  переменную для резервного хранения состояния битов регистра «REG_BUM_BIT_LAMP», это ускорит процесс управления фарами и поворотниками.
		uint8_t					data[14];																									//	Объявляем  массив     для хранения получаемых/передаваемых данных.
		iarduino_I2C_BASE*		objI2C;																										//	Объявляем  указатель  на  объект полиморфного класса iarduino_I2C_BASE, но в конструкторе данного класса этому указателю будет присвоена ссылка на производный класс iarduino_I2C.
	/**	Внутренние функции **/																											//
		bool					_readBytes				(uint8_t	, uint8_t				);												//	Объявляем  функцию чтения данных в  массив  data					(Параметры:  номер первого регистра, количество байт).
		bool					_writeBytes				(uint8_t	, uint8_t, uint8_t=0	);												//	Объявляем  функцию записи данных из массива data					(Параметры:  номер первого регистра, количество байт, номер первого элемента массива data).
};																																		//
																																		//
#endif																																	//