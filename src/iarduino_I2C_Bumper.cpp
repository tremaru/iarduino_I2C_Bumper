#include "iarduino_I2C_Bumper.h"																								//
																																//
//		Инициализация модуля:																									//	Возвращаемое значение: результат инициализации.
bool	iarduino_I2C_Bumper::begin			(void){																				//	Параметр: отсутствует.
		//	Инициируем работу с шиной I2C:																						//
			objI2C->begin(100);																									//	Инициируем передачу данных по шине I2C на скорости 100 кГц.
		//	Если адрес не указан, то ищим модуль на шине I2C:																	//
			if(valAddrTemp==0){																									//
				for(int i=1; i<127; i++){																						//	Проходим по всем адресам на шине I2C
					if( objI2C->checkAddress(i)											){	valAddr=i; delay(2);				//	Если на шине I2C есть устройство с адресом i, то используем этот адрес для проверки найденного модуля...
					if(_readBytes(REG_MODEL,4)											){										//	Читаем 4 байта начиная с регистра «REG_MODEL» в массив «data».
					if( data[0]     == DEF_MODEL_BUM									){										//	Если у модуля с адресом i в регистре «MODEL»   (data[0]) хранится значение DEF_MODEL_BUM, то ...
					if((data[2]>>1) == i                 || data[2] == 0xFF				){										//	Если у модуля с адресом i в регистре «ADDRESS» (data[2]) хранится значение i (адрес+младший бит) или 0xFF (адрес не задавался), то ...
					if( data[3]     == DEF_CHIP_ID_FLASH || data[3] == DEF_CHIP_ID_METRO){										//	Если у модуля с адресом i в регистре «CHIP_ID» (data[3]) хранится значение DEF_CHIP_ID_FLASH (идентификатор модулей Flash), или DEF_CHIP_ID_METRO (идентификатор модулей Metro), то ...
						valAddrTemp=i; i=128;																					//	Считаем что модуль обнаружен, сохраняем значение i как найденный адрес и выходим из цикла.
					}}}}}																										//
				}																												//
			}																													//
		//	Если модуль не найден, то возвращаем ошибку инициализации:															//
			if( valAddrTemp == 0														){	valAddr=0; return false;}			//
		//	Проверяем наличие модуля на шине I2C:																				//
			if( objI2C->checkAddress(valAddrTemp) == false								){	valAddr=0; return false;}			//	Если на шине I2C нет устройств с адресом valAddrTemp, то возвращаем ошибку инициализации
			valAddr=valAddrTemp;																								//	Сохраняем адрес модуля на шине I2C.
		//	Проверяем значения регистров модуля:																				//
			if(_readBytes(REG_MODEL,4)==false											){	valAddr=0; return false;}			//	Если не удалось прочитать 4 байта в массив «data» из модуля начиная с регистра «REG_MODEL», то возвращаем ошибку инициализации.
			if( data[0]     != DEF_MODEL_BUM											){	valAddr=0; return false;}			//	Если значение  регистра «MODEL»   (data[0]) не совпадает со значением DEF_MODEL_BUM, то возвращаем ошибку инициализации.
			if((data[2]>>1) != valAddrTemp       && data[2] !=0xFF						){	valAddr=0; return false;}			//	Если значение  регистра «ADDRESS» (data[2]) не совпадает с адресом модуля и не совпадает со значением 0xFF, то возвращаем ошибку инициализации.
			if( data[3]     != DEF_CHIP_ID_FLASH && data[3] != DEF_CHIP_ID_METRO		){	valAddr=0; return false;}			//	Если значение  регистра «CHIP_ID» (data[3]) не совпадает со значением DEF_CHIP_ID_FLASH и DEF_CHIP_ID_METRO, то возвращаем ошибку инициализации.
			valVers=data[1];																									//	Сохраняем байт регистра «VERSION» (data[1]) в переменую «valVers».
		//	Перезагружаем модуль устанавливая его регистры в значение по умолчанию:												//
			reset();																											//	Выполняем программную перезагрузку.
			delay(5);																											//
		//	Читаем начальное состояние регистра битов «REG_BUM_BIT_LAMP» в переменную «valRegBitLamp»:							//
			if(_readBytes(REG_BUM_BIT_LAMP,1)==false){return false;}															//	Читаем 1 байт регистра «REG_BUM_BIT_LAMP» в массив «data».
			valRegBitLamp = data[0];																							//	Сохраняем один байт массива «data» в переменную «valRegBitLamp».
			delay(500);																											//	Даём время для стабилизации показаний АЦП модуля.
			return true;																										//	Возвращаем флаг успешной инициализаии.
}																																//
																																//
//		Перезагрузка модуля:																									//	Возвращаемое значение:	результат перезагрузки.
bool	iarduino_I2C_Bumper::reset			(void){																				//	Параметр:				отсутствует.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Устанавливаем бит перезагрузки:																					//
				if(_readBytes(REG_BITS_0,1)==false){return false;}																//	Читаем 1 байт регистра «BITS_0» в массив «data».
				data[0] |= 0b10000000;																							//	Устанавливаем бит «SET_RESET»
				if(_writeBytes(REG_BITS_0,1)==false){return false;}																//	Записываем 1 байт в регистр «BITS_0» из массива «data».
			//	Ждём установки флага завершения перезагрузки:																	//
				do{ if(_readBytes(REG_FLAGS_0,1)==false){return false;} }														//	Читаем 1 байт регистра «REG_FLAGS_0» в массив «data».
				while( (data[0]&0b10000000) == 0);																				//	Повторяем чтение пока не установится флаг «FLG_RESET».
				return true;																									//
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Смена адреса модуля:																									//	Возвращаемое значение:	резульат смены адреса.
bool	iarduino_I2C_Bumper::changeAddress	(uint8_t newAddr){																	//	Параметр:				newAddr - новый адрес модуля (0x07 < адрес < 0x7F).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Проверяем новый адрес:																							//
				if(newAddr>0x7F){newAddr>>=1;}																					//	Корректируем адрес, если он указан с учётом бита RW.
				if(newAddr==0x00 || newAddr==0x7F){return false;}																//	Запрещаем устанавливать адрес 0x00 и 0x7F.
			//	Записываем новый адрес:																							//
				if(_readBytes(REG_BITS_0,1)==false){return false;}																//	Читаем 1 байт регистра «BITS_0» в массив «data».
				data[0] |= 0b00000010;																							//	Устанавливаем бит «SAVE_ADR_EN»
				if(_writeBytes(REG_BITS_0,1)==false){return false;}																//	Записываем 1 байт в регистр «BITS_0» из массива «data».
				data[0] = (newAddr<<1)|0x01;																					//	Готовим новый адрес к записи в модуль, установив бит «SAVE_FLASH».
				if(_writeBytes(REG_ADDRESS,1)==false){return false;}															//	Записываем 1 байт в регистр «ADDRESS» из массива «data».
				delay(200);																										//	Даём более чем достаточное время для применения модулем нового адреса.
			//	Проверяем наличие модуля с новым адресом на шине I2C:															//
				if(objI2C->checkAddress(newAddr)==false){return false;}															//	Если на шине I2C нет модуля с адресом newAddr, то возвращаем ошибку.
				valAddr     = newAddr;																							//	Сохраняем новый адрес как текущий.
				valAddrTemp = newAddr;																							//	Сохраняем новый адрес как указанный.
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Управление фарами:																										//	Возвращаемое значение:	результат применения указанного действия (true/false).
bool	iarduino_I2C_Bumper::setLamp		(bool state){																		//	Параметр:				флаг включения фар (true-вкл / false-выкл).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				if( state )	{ data[0] = valRegBitLamp |  BUM_SET_HEAD_ON; }														//	Устанавливаем бит управления фарами «BUM_SET_HEAD_ON» регистра «REG_BUM_BIT_LAMP».
				else		{ data[0] = valRegBitLamp & ~BUM_SET_HEAD_ON; }														//	Сбрасываем    бит управления фарами «BUM_SET_HEAD_ON» регистра «REG_BUM_BIT_LAMP».
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_BIT_LAMP,1)==false){return false;}														//	Записываем 1 байт в регистр «REG_BUM_BIT_LAMP» из массива «data».
				valRegBitLamp = data[0];																						//	Сохраняем 1 байт переданный в регистр «REG_BUM_BIT_LAMP».
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Управление поворотниками:																								//	Возвращаемое значение:	результат применения указанного действия (true/false).
bool	iarduino_I2C_Bumper::setTurnSignal	(uint8_t mode){																		//	Параметры:				режим работы поворотников (BUM_TURN_X).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				data[0] = valRegBitLamp;																						//	Сохраняем значение переменной «valRegBitLamp» в элемент массива «data».
				data[0] &= ~BUM_SET_TURN_MODE;																					//	Сбрасываем все биты режима управления поворотниками «BUM_SET_TURN_MODE»      регистра «REG_BUM_BIT_LAMP».
				data[0] &= ~BUM_SET_TURN_AUTO_STOP;																				//	Сбрасываем все биты условия отключения поворотников «BUM_SET_TURN_AUTO_STOP» регистра «REG_BUM_BIT_LAMP».
				switch(mode){																									//
					case BUM_TURN_OFF:											 		break;									//	Оставляем      биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» сброшенными.
					case BUM_TURN_LEFT:			data[0] |= BUM_SET_TURN_MODE_LEFT; 		break;									//	Устанавливаем  биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» в значение «BUM_SET_TURN_MODE_LEFT».
					case BUM_TURN_RIGHT:		data[0] |= BUM_SET_TURN_MODE_RIGHT; 	break;									//	Устанавливаем  биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» в значение «BUM_SET_TURN_MODE_RIGHT».
					case BUM_TURN_EMERGENCY:	data[0] |= BUM_SET_TURN_MODE_EMERGENCY; break;									//	Устанавливаем  биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» в значение «BUM_SET_TURN_MODE_EMERGENCY».
					case BUM_TURN_POLICE:		data[0] |= BUM_SET_TURN_MODE_POLICE; 	break;									//	Устанавливаем  биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» в значение «BUM_SET_TURN_MODE_POLICE».
					case BUM_TURN_AUTO:			data[0] |= valAutoStartStop; 			break;									//	Устанавливаем  биты режима управления поворотниками «BUM_SET_TURN_MODE» регистра «REG_BUM_BIT_LAMP» в значение «BUM_SET_TURN_MODE_EMERGENCY».
					default: return false;																						//	Если режим работы поворотников указан некорректно, то возвращаем false.
				}																												//
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_BIT_LAMP,1)==false){return false;}														//	Записываем 1 байт в регистр «REG_BUM_BIT_LAMP» из массива «data».
				valRegBitLamp = data[0];																						//	Сохраняем 1 байт переданный в регистр «REG_BUM_BIT_LAMP».
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Установка периода миганий поворотников:																					//	Возвращаемое значение:	результат применения нового периода (true/false).
bool	iarduino_I2C_Bumper::setTurnPeriod	(uint8_t period){																	//	Параметр:				период (BUM_SET_TURN_PERIOD_X).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				data[0] = valRegBitLamp & ~BUM_SET_TURN_PERIOD;																	//	Сбрасываем все биты установки периода миганий поворотников «BUM_SET_TURN_PERIOD» в регистре «REG_BUM_BIT_LAMP».
				switch(period){																									//
					case BUM_TURN_100: data[0] |= BUM_SET_TURN_PERIOD_100; break;												//	Устанавливаем период мигания поворотников в 100 мс.
					case BUM_TURN_200: data[0] |= BUM_SET_TURN_PERIOD_200; break;												//	Устанавливаем период мигания поворотников в 200 мс.
					case BUM_TURN_400: data[0] |= BUM_SET_TURN_PERIOD_400; break;												//	Устанавливаем период мигания поворотников в 400 мс.
					case BUM_TURN_800: data[0] |= BUM_SET_TURN_PERIOD_800; break;												//	Устанавливаем период мигания поворотников в 800 мс.
					default: return false;																						//	Если период указан некорректно, то возвращаем false.
				}																												//
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_BIT_LAMP,1)==false){return false;}														//	Записываем 1 байт в регистр «REG_BUM_BIT_LAMP» из массива «data».
				valRegBitLamp = data[0];																						//	Сохраняем 1 байт переданный в регистр «REG_BUM_BIT_LAMP».
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Настройка работы поворотников в автоматическом режиме:																	//	Возвращаемое значение:	результат применения новых настроек (true/false).
bool	iarduino_I2C_Bumper::settingsTurnAuto(uint8_t start, uint8_t stop, bool f){												//	Параметры:				условие включения (BUM_AUTO_ON_X), условие отключения (BUM_AUTO_OFF_X), [добавить 2 мигания перед отключением true/false].
			if(valAddr){																										//	Если модуль был инициализирован, то ...
				uint8_t i=0;																									//	Создаём временную переменную.
			//	Проверяем условие включения светодиодов в автоматическом режиме:												//
				switch(start){																									//
					case BUM_AUTO_ON_1: i |= BUM_SET_TURN_MODE_AUTO_1; break;													//	Поворотник включается автоматически, при наличии линии под одним, самым крайним, датчиком со стороны поворотника.
					case BUM_AUTO_ON_2: i |= BUM_SET_TURN_MODE_AUTO_2; break;													//	Поворотник включается автоматически, при наличии линии под одним из двух крайних датчиков со стороны поворотника.
					case BUM_AUTO_ON_3: i |= BUM_SET_TURN_MODE_AUTO_3; break;													//	Поворотник включается автоматически, при наличии линии под одним из трёх крайних датчиков со стороны поворотника.
					default: return false;																						//	Если условие включения поворотника указано некорректно, то возвращаем false.
				}																												//
			//	Проверяем условие отключения светодиодов в автоматическом режиме:												//
				switch(stop){																									//
					case BUM_AUTO_OFF_ANY: i |= (f?BUM_SET_TURN_AUTO_STOP_2:BUM_SET_TURN_AUTO_STOP_0); break;					//	Поворотник отключается как только линия окажется под любым датчиком кроме указанных в условии включения поворотников.
					case BUM_AUTO_OFF_CEN: i |= (f?BUM_SET_TURN_AUTO_STOP_3:BUM_SET_TURN_AUTO_STOP_1); break;					//	Поворотник отключается как только линия окажется под центральным датчиком.
					default: return false;																						//
				}																												//
				valAutoStartStop=i;																								//	Сохраняем полученную комбинацию битов в переменную «valAutoStartStop».
			//	Проверяем не работают ли сейчас поворотники в автоматическом режиме:											//
				if(	( (valRegBitLamp & BUM_SET_TURN_MODE) == BUM_SET_TURN_MODE_AUTO_1) ||										//
					( (valRegBitLamp & BUM_SET_TURN_MODE) == BUM_SET_TURN_MODE_AUTO_2) ||										//
					( (valRegBitLamp & BUM_SET_TURN_MODE) == BUM_SET_TURN_MODE_AUTO_3) ){										//
					return setTurnSignal(BUM_TURN_AUTO);																		//	Перезапускаем режим работы поворотников.
				}																												//
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Запуск автоматической калибровки модуля:																				//	Возвращаемое значение:	результат запуска автоматической калибровки (true/false).
bool	iarduino_I2C_Bumper::setCalibrationRun	(void){																			//	Параметр:				отсутствует.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				data[0] = BUM_CALIBR_RUN;																						//	Сохраняем код запуска калибровки в массив «data».
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_CALIBRATION,1)==false){return false;}													//	Записываем 1 байт в регистр «REG_BUM_CALIBRATION» из массива «data».
			//	Ждём завершения калибровки:																						//
				delay(3000);																									//	Устанавливаем задержку ( 1 сек световой анимации бегущего огня + 0,3 сек восстановления АЦП + 1.5 сек мигания при успехе + 0.03 сек записи в Flash) и того ~2.8 сек.
				uint8_t	sumtry=10;																								//	Определяем переменную для подсчёта количества оставшихся попыток чтения.
				while( sumtry>0 ){																								//	Входим в цикл ожидания завершения калибровки.
					if( _readBytes(REG_BUM_CALIBRATION, 1) == false ){return false;}											//	Читаем 1 байт регистра «REG_BUM_CALIBRATION» в массив «data».
					if( (data[0] & BUM_STAGE_CALIBRATION) == BUM_STAGE_WAITING ){return true;}									//	Если в регистре «REG_BUM_CALIBRATION» установлена стадия калибровки «BUM_STAGE_WAITING», то возвращаем true.
					delay(200); sumtry--;																						//	Устанавливаем задержку и уменьшаем количество попыток чтения.
				}																												//
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Завершение автоматической калибровки модуля:																			//	Возвращаемое значение:	результат завершения автоматической калибровки (true/false).
bool	iarduino_I2C_Bumper::setCalibrationEnd	(bool f){																		//	Параметр:				флаг установки общей границы АЦП для всех датчиков.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				if(f){data[0] = BUM_CALIBR_END_A;}																				//	Сохраняем код завершения калибровки в массив «data». Результатом калибровки станет рассчет границы АЦП общей для всех датчиков модуля.
				else {data[0] = BUM_CALIBR_END;  }																				//	Сохраняем код завершения калибровки в массив «data». Результатом калибровки станет рассчет границ АЦП для каждого датчика модуля.
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_CALIBRATION,1)==false){return false;}													//	Записываем 1 байт в регистр «REG_BUM_CALIBRATION» из массива «data».
			//	Ждём завершения калибровки:																						//
				delay(3000);																									//	Устанавливаем задержку ( 1 сек световой анимации бегущего огня + 0,3 сек восстановления АЦП + 1.5 сек мигания при успехе + 0.03 сек записи в Flash) и того ~2.8 сек.
				uint8_t	sumtry=10;																								//	Определяем переменную для подсчёта количества оставшихся попыток чтения.
				while( sumtry>0 ){																								//	Входим в цикл ожидания завершения калибровки.
					if( _readBytes(REG_BUM_CALIBRATION, 1) == false ){return false;}											//	Читаем 1 байт регистра «REG_BUM_CALIBRATION» в массив «data».
					if( data[0] & BUM_FLG_CALIBRATION ){return true;}															//	Если в регистре «REG_BUM_CALIBRATION» установлен флаг «BUM_FLG_CALIBRATION», то возвращаем true.
					delay(200); sumtry--;																						//	Устанавливаем задержку и уменьшаем количество попыток чтения.
				}																												//
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Ручная калибровка модуля:																								//	Возвращаемое значение:	результат записи калибровочных значений (true/false).
bool	iarduino_I2C_Bumper::setCalibrationManual(uint16_t adc_1, uint16_t adc_2, uint16_t adc_3, uint16_t adc_4, uint16_t adc_5, uint16_t adc_6, uint16_t adc_7){	//	Параметры: граница АЦП между линией и фоном 1 датчика, 2 датчика, ... , 7 датчика.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Готовим данные для передачи:																					//
				data[ 0] = (uint8_t)  adc_1;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_1_L».
				data[ 1] = (uint8_t) (adc_1>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_1_H».
				data[ 2] = (uint8_t)  adc_2;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_2_L».
				data[ 3] = (uint8_t) (adc_2>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_2_H».
				data[ 4] = (uint8_t)  adc_3;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_3_L».
				data[ 5] = (uint8_t) (adc_3>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_3_H».
				data[ 6] = (uint8_t)  adc_4;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_4_L».
				data[ 7] = (uint8_t) (adc_4>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_4_H».
				data[ 8] = (uint8_t)  adc_5;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_5_L».
				data[ 9] = (uint8_t) (adc_5>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_5_H».
				data[10] = (uint8_t)  adc_6;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_6_L».
				data[11] = (uint8_t) (adc_6>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_6_H».
				data[12] = (uint8_t)  adc_7;																					//	Готовим младший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_7_L».
				data[13] = (uint8_t) (adc_7>>8);																				//	Готовим старший байт АЦП среднего между фоном и линией первого датчика, для записи в регистр модуля «REG_BUM_THRESHOLD_7_H».
			//	Передаём подготовленные данные:																					//
				if(_writeBytes(REG_BUM_THRESHOLD,14)==false){return false;}														//	Записываем 14 байта из массива «data» в модуль начиная с регистра «REG_BUM_THRESHOLD_1_L».
				delay(100);																										//	Устанавливаем задержку.
				return true;																									//	Возвращаем флаг успеха.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Получение текущего статуса калибровки модуля:																			//	Возвращаемое значение:	статус (BUM_STAGE_OFF - не выполняется / BUM_STAGE_READ_LN - чтение линии / BUM_STAGE_WAITING - ожидается чтение фона / BUM_STAGE_READ_BG - чтение фона).
uint8_t	iarduino_I2C_Bumper::getCalibrationStage(void){																			//	Объявляем  функцию получения текущего статуса калибровки модуля		(Параметр:  отсутствует).
			uint8_t result=0;																									//	Определяем переменную для хранения результата чтения.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Получаем данные:																								//
				if( _readBytes(REG_BUM_CALIBRATION, 1) == false )	{return 0;}													//	Читаем 1 байт регистра «REG_BUM_CALIBRATION» в массив «data».
			//	Возвращаем результат:																							//
				result = ( data[0] & (BUM_FLG_CALIBRATION | BUM_STAGE_CALIBRATION) );											//	Получаем значение находящееся в битах «BUM_STAGE_CALIBRATION» регистра «REG_BUM_CALIBRATION».
			}	return result;																									//	Возвращаем результат.
}																																//
																																//
//		Получение флага наличия линии под датчиком:																				//	Возвращаемое значение:	флаг налияия линии (true/false), или число, каждый бит которого является флагом наличия линии.
uint8_t	iarduino_I2C_Bumper::getLineDigital	(uint8_t num){																		//	Параметр:				номер датчика (1...7, или BUM_LINE_ALL).
			uint8_t result=0;																									//	Определяем переменную для хранения результата чтения.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Получаем данные:																								//
				if( _readBytes(REG_BUM_FLG_LINE, 1) == false )	{return 0;}														//	Читаем 1 байт регистра «REG_BUM_FLG_LINE» в массив «data».
			//	Возвращаем результат:																							//
				if( (num>=1) && (num<=7) ){result = (data[0] & (1<<(num-1))) ? 1:0; }else										//	Возвращаем значение бита под номером num-1.
				if(  num == BUM_LINE_ALL ){result = (data[0] << 1); }															//	Возвращаем 1 байт из массива «data» сдвинув все его биты влево.
			}	return result;																									//	Возвращаем результат.
}																																//
																																//
//		Получение значения АЦП снятое с датчика:																				//	Возвращаемое значение:	число от 0 до 4095 соответствующее значению АЦП снятого с датчика.
uint16_t iarduino_I2C_Bumper::getLineAnalog	(uint8_t num){																		//	Параметр:				номер датчика (1...7).
			uint16_t result=0;																									//	Определяем переменную для хранения результата чтения.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Получаем данные:																								//
				if( _readBytes(REG_BUM_ANALOG + ((num-1)*2), 2) == false ){return 0;}											//	Читаем 2 байта из регистров «REG_BUM_ANALOG_X_L» и «REG_BUM_ANALOG_X_H» в массив «data».
			//	Возвращаем результат:																							//
				result=data[1]; result<<=8; result|=data[0];																	//	Сохраняем оба прочитанных байта в переменную «result».
			}	return result;																									//	Возвращаем результат.
}																																//
																																//
//		Получение типа линии для которой откалиброван модуль:																	//	Возвращаемое значение:	флаг типа линии (BUM_LINE_WHITE/BUM_LINE_BLACK).
uint8_t	iarduino_I2C_Bumper::getLineType	(void){																				//	Параметр:				отсутствует.
			uint8_t result=false;																								//	Определяем переменную для вывода результата.
			if(valAddr){																										//	Если модуль был инициализирован, то ...
			//	Получаем данные:																								//
				if( _readBytes(REG_BUM_FLG_LINE, 1) == false )	{return false;}													//	Читаем 1 байт регистра «REG_BUM_FLG_LINE» в массив «data».
			//	Определяем тип линии:																							//
				result = (data[0] & BUM_FLG_LINE_BLACK) ? BUM_LINE_BLACK:BUM_LINE_WHITE;										//	Если бит «BUM_FLG_LINE_BLACK» установлен в «1» то результатом будет «BUM_LINE_BLACK», иначе «BUM_LINE_WHITE»
			}	return result;																									//	Возвращаем результат.
}																																//
																																//
//		Указание типа линии трассы:																								//	Возвращаемое значение:	результат применения нового типа линии трассы (true/false).
bool	iarduino_I2C_Bumper::setLineType	(uint8_t line_type){																//	Параметр:				тип линии (BUM_LINE_WHITE/BUM_LINE_BLACK/BUM_LINE_CHANGE).
			if(valAddr){																										//	Если модуль был инициализирован, то ...
				uint8_t	sumtry=5;																								//	Определяем переменную для подсчёта количества оставшихся попыток записи типа линии.
			//	Получаем данные:																								//
				if( _readBytes(REG_BUM_FLG_LINE, 1) == false )	{return false;} data[0] &= BUM_FLG_LINE_BLACK;					//	Читаем 1 байт регистра «REG_BUM_FLG_LINE» в массив «data» сбрасывая все биты кроме «BUM_FLG_LINE_BLACK».
			//	Определяем тип устанавливаемой линии, если её нужно изменить:													//
				if(line_type==BUM_LINE_CHANGE){line_type = (data[0] & BUM_FLG_LINE_BLACK) ? BUM_LINE_WHITE:BUM_LINE_BLACK;}		//	
			//	Проверяем не установлен ли уже требуемый тип линии:																//
				while((((data[0])&&(line_type==BUM_LINE_WHITE))||((!data[0])&&(line_type==BUM_LINE_BLACK)))&& sumtry>0){		//	Если установленный тип линии не соответствует устанавливаемому типу линии, то ...
				//	Готовим данные для передачи:																				//
					if(line_type==BUM_LINE_BLACK){data[0] |=  BUM_FLG_LINE_BLACK;}else											//	Если нужно указать тёмную линию, то устанавливаем флаг «BUM_FLG_LINE_BLACK» в байте «data[0]» для регистра «REG_BUM_FLG_LINE».
					if(line_type==BUM_LINE_WHITE){data[0] &= ~BUM_FLG_LINE_BLACK;}else{return false;}							//	Если нужно указать светлую линию, то сбрасываем флаг «BUM_FLG_LINE_BLACK» в байте «data[0]» для регистра «REG_BUM_FLG_LINE».
				//	Передаём подготовленные данные:																				//
					if(_writeBytes(REG_BUM_FLG_LINE,1)==false){return false;}													//	Записываем 1 байт в регистр «REG_BUM_FLG_LINE» из массива «data».
					delay(1);																									//
				//	Получаем данные из того же регистра:																		//
					if( _readBytes(REG_BUM_FLG_LINE, 1) == false )	{return false;} data[0] &= BUM_FLG_LINE_BLACK;				//	Читаем 1 байт регистра «REG_BUM_FLG_LINE» в массив «data» сбрасывая все биты кроме «BUM_FLG_LINE_BLACK».
					sumtry--;																									//	Уменьшаем количество попыток установки типа линии.
				}	if(sumtry){return true;}																					//	Возвращаем результат установки типа линии.
			}	return false;																									//	Возвращаем ошибку.
}																																//
																																//
//		Чтение данных из регистров в массив data:																				//	Возвращаемое значение:	результат чтения (true/false).
bool	iarduino_I2C_Bumper::_readBytes		(uint8_t reg, uint8_t sum){															//	Параметры:				reg - номер первого регистра, sum - количество читаемых байт.
			bool	result=false;																								//	Определяем флаг       для хранения результата чтения.
			uint8_t	sumtry=10;																									//	Определяем переменную для подсчёта количества оставшихся попыток чтения.
			do{	result = objI2C->readBytes(valAddr, reg, data, sum);															//	Считываем из модуля valAddr, начиная с регистра reg, в массив data, sum байт.
				sumtry--;	if(!result){delay(1);}																				//	Уменьшаем количество попыток чтения и устанавливаем задержку при неудаче.
			}	while		(!result && sumtry>0);																				//	Повторяем чтение если оно завершилось неудачей, но не более sumtry попыток.
			delayMicroseconds(500);																								//	Между пакетами необходимо выдерживать паузу.
			return result;																										//	Возвращаем результат чтения (true/false).
}																																//
																																//
//		Запись данных в регистры из массива data:																				//	Возвращаемое значение:	результат записи (true/false).
bool	iarduino_I2C_Bumper::_writeBytes	(uint8_t reg, uint8_t sum, uint8_t num){											//	Параметры:				reg - номер первого регистра, sum - количество записываемых байт, num - номер первого элемента массива data.
			bool	result=false;																								//	Определяем флаг       для хранения результата записи.
			uint8_t	sumtry=10;																									//	Определяем переменную для подсчёта количества оставшихся попыток записи.
			do{	result = objI2C->writeBytes(valAddr, reg, &data[num], sum);														//	Записываем в модуль valAddr начиная с регистра reg, sum байи из массива data начиная с элемента num.
				sumtry--;	if(!result){delay(1);}																				//	Уменьшаем количество попыток записи и устанавливаем задержку при неудаче.
			}	while		(!result && sumtry>0);																				//	Повторяем запись если она завершилась неудачей, но не более sumtry попыток.
			delay(10);																											//	Ждём применения модулем записанных данных.
			return result;																										//	Возвращаем результат записи (true/false).
}																																//
																																//