#include "stdafx.h"
#include "Error.h"

namespace Error
{
    // серии ошибок:    0 -  99 - системные ошибки
    //                 100 - 109 - ошибки параметров
    //                 110 - 119 - ошибки открытия и чтения файлов
    //                 200 - 210 - ошибки лексера
    //                 ...
    ERROR errors[ERROR_MAX_ENTRY] = 
    {
        ERROR_ENTRY(0, "Недопустимый код ошибки"), 
        ERROR_ENTRY(1, "Системный сбой"),
        ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), 
        ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), 
        ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
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
        ERROR_ENTRY_NODEF(205, "Невозможно преобразовать hex (невалидный формат)"),
        ERROR_ENTRY_NODEF(206, "Превышен размер строкового литерала"),
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

        ERROR_ENTRY(700, ""),           
        ERROR_ENTRY(701, "Отсутствует точка входа main"),           
		ERROR_ENTRY(702, "Задано более одной точки входа main"),
		ERROR_ENTRY(703, "Необъявленный идентификатор"),           
		ERROR_ENTRY(704, "Попытка переопределить уже существующий идентификатор"), 
		ERROR_ENTRY(705, "Попытка реализовать уже существующую функцию"), 
		ERROR_ENTRY_NODEF(706), 
		ERROR_ENTRY(707, "Недопустимый целочисленный литерал (выход за пределы диапазона)"), 
		ERROR_ENTRY(708, "Использование пустого строкового/символьного литерала недопустимо"),
		ERROR_ENTRY(709, "Попытка переопределить формальный параметр функции"),
		ERROR_ENTRY(710, "Несовпадение типов при присваивании (RHS не соответствует LHS)"),
		ERROR_ENTRY(711, "Типы данных в выражении не совпадают (Общая ошибка совместимости)"),
		ERROR_ENTRY(712, "Арифметические операторы не могут применяться со строковым или символьным типом"),
		ERROR_ENTRY(713, "Логические операторы могут применяться только с целочисленными и логическими типами"),
		ERROR_ENTRY(714, "Деление на ноль"),
		ERROR_ENTRY(715, "Тип функции и тип возвращаемого значения должны совпадать"),
		ERROR_ENTRY(716, "Несовпадение типов передаваемых параметров при вызове функции"), 
		ERROR_ENTRY(717, "Слишком много аргументов в вызове функции"),
		ERROR_ENTRY(718, "Слишком мало аргументов в вызове функции"),
        ERROR_ENTRY(719, "Управляющее выражение оператора switch имеет недопустимый тип (требуется целочисленный)"),
		ERROR_ENTRY(720, "Обнаружена вторая метка default (допускается только одна)"),
		ERROR_ENTRY(721, "Оператор break находится вне контекста switch"),
		ERROR_ENTRY(722, "Оператор return используется вне тела функции"),
		ERROR_ENTRY(723, "Отсутствует оператор return в непустой функции"),
		ERROR_ENTRY_NODEF(724),	ERROR_ENTRY_NODEF(725),	ERROR_ENTRY_NODEF(726),
		ERROR_ENTRY_NODEF(727),	ERROR_ENTRY_NODEF(728),	ERROR_ENTRY_NODEF(729),
		ERROR_ENTRY_NODEF10(730), ERROR_ENTRY_NODEF(740),
		ERROR_ENTRY_NODEF10(750), ERROR_ENTRY_NODEF10(760), ERROR_ENTRY_NODEF(770),
		ERROR_ENTRY_NODEF10(780), ERROR_ENTRY_NODEF10(790),

        ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
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
