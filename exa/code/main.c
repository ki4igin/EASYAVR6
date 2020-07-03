/*
 * Exam.c
 *
 * Created: 23.06.2020 9:43
 * Author : Ярынич В.М.
 */ 

#include <avr/io.h>


int main(void)
{
	
	// Macro -----------------------------------------------------------------------
	#define SPI_PORT PORTB  // Порт на который выведен модуль SPI
	#define SPI_DDR  DDRB   //
	#define SPI_PIN  PINB   //

	#define SPI_SCK  7  // Номер вывода SCK модуля SPI
	#define SPI_MISO 6  // Номер вывода MISO модуля SPI
	#define SPI_MOSI 5  // Номер вывода MOSI модуля SPI
	#define SPI_SS   4  // Номер вывода SS модуля SPI

	#define SPI_CS 1    // Номер вывода МК к которому подключен вывод CS
	                    // расширителя портов
	
	// Тип статуса модуля SPI
	typedef enum
	{
		SPI_NOINIT = 0,  // SPI не инициализирован
		SPI_READY,       // SPI готов к обмену
		SPI_BUSY         // SPI занят
	} SpiStatus_t;
	
	// Variables -------------------------------------------------------------------
	static SpiStatus_t spiStatus = SPI_NOINIT;  // Переменная статуса модуля SPI
	static uint8_t*    pbufSpi;                 // Указатель на буфер для обмена
	static uint8_t     bufSizeSpi;              // Размер буфера для обмена
	uint8_t cnt=0;
	uint8_t temp = 0x16; // переменная для хранения данных
	
    // Инициализация выводов:
    // SCK, MOSI, SS и CS - выходы (SS устанавливается как выход чтобы
    // модуль SPI случайно не перешел в режим Slave);
    // MISO - вход
    SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_CS);
    SPI_DDR &= ~(1 << SPI_MISO);

    // Установка вывода СS в лог. 1
    SPI_PORT |= (1 << SPI_CS);

    // Установка режима, включение модуля SPI, скорость обмена F/8 = 1 МГц
    SPCR = (1 << SPR0 ) | (1 << SPE) | (1 << MSTR);

    // Установка статуса SPI_READY
    spiStatus = SPI_READY;
	
	
		
    while (1) 
    {
		   if (cnt!=2)
		{   // Чтение пришедшего байт данных из SPDR и помещение его в буфер обмена на
		    // место переданного байта
		    *pbufSpi = SPDR;
            cnt++; //увеличение счетчика байт для записи в EEPROM
		    // Уменьшение количества байт обмена
		    // Если оставшееся количество байт обмена ну равно нулю, то передаем
		    // следующий байт данных из буфера обмена;
		    // иначе, завершаем обмен: устанавливаем вывод CS и статус SPI_READY
		    
			EEAR = temp;	 // Указываем адрес для записи
			EEDR = *pbufSpi;	 // Указываем данные для записи
							
			EECR |= (1 << EEMWE); // Разрешаем запись в EEPROM на 4 такта
			EECR |= (1 << EEWE);  // Записываем данные в EEPROM
			bufSizeSpi--;
			temp++;
		    if (bufSizeSpi)
		    {
			    SPDR = *++pbufSpi;
		    }
		    else
		    {
			    SPI_PORT |= (1 << SPI_CS);
			    spiStatus = SPI_READY;
				cnt=0;
				temp=0x16;
			
		    };
		}
    }
}

