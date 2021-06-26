// Includes --------------------------------------------------------------------
#include "onewire.h"

// Typedef ---------------------------------------------------------------------

// Macro -----------------------------------------------------------------------
#define ONEWIRE_PORT     PORTA       //
#define ONEWIRE_DDR      DDRA        //
#define ONEWIRE_PIN      PINA        //
#define ONEWIRE_PIN_MASK (1 << PA4)  //

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
static inline void OnewireHigh(void)
{
    ONEWIRE_DDR &= ~ONEWIRE_PIN_MASK;
}

static inline void OnewireLow(void)
{
    ONEWIRE_DDR |= ONEWIRE_PIN_MASK;
}

static inline uint8_t OnewireLevel(void)
{
    return ONEWIRE_PIN & ONEWIRE_PIN_MASK;
}

// Functions -------------------------------------------------------------------
void OnewireInit()
{
    ONEWIRE_PORT &= ~ONEWIRE_PIN_MASK;
    OnewireHigh();
}

// Выдаёт импульс сброса (reset), ожидает ответный импульс присутствия.
// Если импульс присутствия получен, дожидается его завершения и возвращает 1, иначе возвращает 0
uint8_t OnewireReset()
{
    OnewireLow();
    _delay_us(640);  // Пауза 480..960 мкс
    OnewireHigh();
    _delay_us(2);  // Время необходимое подтягивающему резистору, чтобы вернуть высокий уровень на шине
    // Ждём не менее 60 мс до появления импульса присутствия;
    for (uint8_t i = 80; i; i--)
    {
        if (OnewireLevel() == 0)
        {
            // Если обнаружен импульс присутствия, ждём его окончания
            while (OnewireLevel() == 0)
            {
            }  // Ждём конца сигнала присутствия
            return 1;
        }
        _delay_us(1);
    }
    return 0;
}

// Отправляет один бит
// bit отправляемое значение, 0 - ноль, любое другое значение - единица
void OnewireSendBit(uint8_t bit)
{
    OnewireLow();
    if (bit)
    {
        _delay_us(5);  // Низкий импульс, от 1 до 15 мкс (с учётом времени восстановления уровня)
        OnewireHigh();
        _delay_us(90);  // Ожидание до завершения таймслота (не менее 60 мкс)
    }
    else
    {
        _delay_us(90);  // Низкий уровень на весь таймслот (не менее 60 мкс, не более 120 мкс)
        OnewireHigh();
        _delay_us(5);  // Время восстановления высокого уровеня на шине + 1 мс (минимум)
    }
}

// Отправляет один байт, восемь подряд бит, младший бит вперёд
// data - отправляемое значение
void OnewireSend(uint8_t data)
{
    for (uint8_t i = 8; i; i--)
    {
        OnewireSendBit(data & 1);
        data >>= 1;
    }
}

// читает значение бита, передаваемое уйстройством.
// Возвращает 0 - если передан 0, отличное от нуля значение - если передана единица
uint8_t OnewireReadBit()
{
    OnewireLow();
    _delay_us(2);  // Длительность низкого уровня, минимум 1 мкс
    OnewireHigh();
    _delay_us(8);  // Пауза до момента сэмплирования, всего не более 15 мкс
    uint8_t bit = OnewireLevel();
    _delay_us(80);  // Ожидание до следующего тайм-слота, минимум 60 мкс с начала низкого уровня
    return bit;
}

// Читает один байт, переданный устройством, младший бит вперёд, возвращает прочитанное значение
uint8_t OnewireRead()
{
    uint8_t data = 0;
    for (uint8_t i = 8; i; i--)
    {
        data >>= 1;
        if (OnewireReadBit())
        {
            data |= 0x80;
        }
    }
    return data;
}

// End File --------------------------------------------------------------------