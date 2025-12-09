#include "stdafx.h"
#include "Error.h"

namespace Error
{
    // серии ошибок:    0 -  99 - системные ошибки
    //                 100 - 109 - ошибки параметров
    //                 110 - 119 - ошибки открытия и чтения файлов
    //                 ...
    ERROR errors[ERROR_MAX_ENTRY] = // таблица ошибок
    {
        ERROR_ENTRY(0, "Недопустимый код ошибки"), // код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
        ERROR_ENTRY(1, "Системный сбой"),
        ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
        ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
        ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), 
        ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), 
        ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

        ERROR_ENTRY(100, "Параметр -in должен быть задан"),
        ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
        ERROR_ENTRY(104, "Превышена длина входного параметра"),
        ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
        ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
        ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
        ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
        ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
        ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
        ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
        ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
        ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

        ERROR_ENTRY(200, "Неизвестная лексема"),
        ERROR_ENTRY(201, "Идентификатор слишком длинный"),
        ERROR_ENTRY(202, "Недопустимый символ в идентификаторе"),
        ERROR_ENTRY(203, "Незакрытая строковая константа"),
        ERROR_ENTRY(204, "Неизвестный оператор"),
        ERROR_ENTRY_NODEF(205),
        ERROR_ENTRY_NODEF(206),
        ERROR_ENTRY_NODEF(207),
        ERROR_ENTRY_NODEF(208),
        ERROR_ENTRY_NODEF(209),
        ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
        ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280),
        ERROR_ENTRY_NODEF10(290),

        ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

        ERROR_ENTRY(600, "Неверная структура программы"), 
		ERROR_ENTRY(601, "Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY_NODEF(605),
		ERROR_ENTRY_NODEF(606),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),

        ERROR_ENTRY_NODEF100(700), 
        
		ERROR_ENTRY(800, "Ошибка преобразования в ПОЛИЗ"),         // общая/прочая
		ERROR_ENTRY(801, "Несбалансированные скобки"),             // balance != 0
		ERROR_ENTRY(802, "Пустое выражение"),                      // end <= start
		ERROR_ENTRY(803, "Неизвестная лексема"),                   // не i,l,(,),+,-,*,/,=,,
		ERROR_ENTRY(804, "Позиция за пределами таблицы лексем"),   // lextable_pos вне диапазона
		ERROR_ENTRY(805, "Не найден конец выражения"),             // нет ';'
		ERROR_ENTRY(806, "ПОЛИЗ длиннее исходного выражения"),     // output.size() > (end-start)
		ERROR_ENTRY(807, "Ошибка приоритета операторов"),          // GetPriority() вернул 0
		ERROR_ENTRY_NODEF(808),
		ERROR_ENTRY_NODEF(809),
		ERROR_ENTRY_NODEF10(810), ERROR_ENTRY_NODEF10(820), ERROR_ENTRY_NODEF10(830), 
        ERROR_ENTRY_NODEF10(840), ERROR_ENTRY_NODEF10(850), ERROR_ENTRY_NODEF10(860), 
        ERROR_ENTRY_NODEF10(870), ERROR_ENTRY_NODEF10(880), ERROR_ENTRY_NODEF10(890),

        ERROR_ENTRY_NODEF100(900)
    };

    ERROR geterror(int id) {
        ERROR err;
		err = errors[0]; // код ошибки 0 – недопустимый id
        if (id > 0 && id < ERROR_MAX_ENTRY) {
            err = errors[id];
        }
        return err;
    }

    ERROR geterrorin(int id, int line = -1, int col = -1) {
        ERROR err = geterror(id);
        err.inext.line = line;
        err.inext.col = col;
        return err;
    }
}
