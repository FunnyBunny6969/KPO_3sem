#include "stdafx.h"
#include "Error.h"

namespace Error
{
    // ñåğèè îøèáîê:    0 -  99 - ñèñòåìíûå îøèáêè
    //                 100 - 109 - îøèáêè ïàğàìåòğîâ
    //                 110 - 119 - îøèáêè îòêğûòèÿ è ÷òåíèÿ ôàéëîâ
    //                 ...
    ERROR errors[ERROR_MAX_ENTRY] = // òàáëèöà îøèáîê
    {
        ERROR_ENTRY(0, "Íåäîïóñòèìûé êîä îøèáêè"), // êîä îøèáêè âíå äèàïàçîíà 0 - ERROR_MAX_ENTRY
        ERROR_ENTRY(1, "Ñèñòåìíûé ñáîé"),
        ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
        ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
        ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
        ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
        ERROR_ENTRY(100, "Ïàğàìåòğ -in äîëæåí áûòü çàäàí"),
        ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
        ERROR_ENTRY(104, "Ïğåâûøåíà äëèíà âõîäíîãî ïàğàìåòğà"),
        ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
        ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
        ERROR_ENTRY(110, "Îøèáêà ïğè îòêğûòèè ôàéëà ñ èñõîäíûì êîäîì (-in)"),
        ERROR_ENTRY(111, "Íåäîïóñòèìûé ñèìâîë â èñõîäíîì ôàéëå (-in)"),
        ERROR_ENTRY(112, "Îøèáêà ïğè ñîçäàíèè ôàéëà ïğîòîêîëà (-log)"),
        ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
        ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
        ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
        ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

        ERROR_ENTRY(200, "Íåèçâåñòíàÿ ëåêñåìà"),
        ERROR_ENTRY(201, "Èäåíòèôèêàòîğ ñëèøêîì äëèííûé"),
        ERROR_ENTRY(202, "Íåäîïóñòèìûé ñèìâîë â èäåíòèôèêàòîğå"),
        ERROR_ENTRY(203, "Íåçàêğûòàÿ ñòğîêîâàÿ êîíñòàíòà"),
        ERROR_ENTRY(204, "Íåèçâåñòíûé îïåğàòîğ"),
        ERROR_ENTRY_NODEF(205),
        ERROR_ENTRY_NODEF(206),
        ERROR_ENTRY_NODEF(207),
        ERROR_ENTRY_NODEF(208),
        ERROR_ENTRY_NODEF(209),
        ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
        ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280),
        ERROR_ENTRY_NODEF10(290),

        ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
        ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
    };

    ERROR geterror(int id) {
        ERROR err;
		err = errors[0]; // êîä îøèáêè 0 – íåäîïóñòèìûé id
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
