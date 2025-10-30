#include "Automata.h"

namespace Automata {
    char KEYWORD_integer[] = "integer";
    char EMPTYstring[] = "";




    const FST::FST INTEGER (
        EMPTYstring, 6,
        FST::NODE(1, FST::RELATION('i', 1)),
        FST::NODE(1, FST::RELATION('n', 2)),
        FST::NODE(1, FST::RELATION('t', 3)),
        FST::NODE(1, FST::RELATION('e', 4)),
        FST::NODE(1, FST::RELATION('g', 5)),
        FST::NODE(1, FST::RELATION('e', 5)), 
        FST::NODE(1, FST::RELATION('r', 6)), 
        FST::NODE()
    );

    const FST::FST FUNCTION (
        EMPTYstring, 9,
        FST::NODE(1, FST::RELATION('f', 1)),
        FST::NODE(1, FST::RELATION('u', 2)),
        FST::NODE(1, FST::RELATION('n', 3)),
        FST::NODE(1, FST::RELATION('c', 4)),
        FST::NODE(1, FST::RELATION('t', 5)),
        FST::NODE(1, FST::RELATION('i', 6)),
        FST::NODE(1, FST::RELATION('o', 7)),
        FST::NODE(1, FST::RELATION('n', 8)),
        FST::NODE()
    );


    // Автомат для идентификаторов [a-z]+
    const FST::FST IDENTIFIER (
        EMPTYstring, 2,
        FST::NODE(26,
            FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('c', 0),
            FST::RELATION('d', 0), FST::RELATION('e', 0), FST::RELATION('f', 0),
            FST::RELATION('g', 0), FST::RELATION('h', 0), FST::RELATION('i', 0),
            FST::RELATION('j', 0), FST::RELATION('k', 0), FST::RELATION('l', 0),
            FST::RELATION('m', 0), FST::RELATION('n', 0), FST::RELATION('o', 0),
            FST::RELATION('p', 0), FST::RELATION('q', 0), FST::RELATION('r', 0),
            FST::RELATION('s', 0), FST::RELATION('t', 0), FST::RELATION('u', 0),
            FST::RELATION('v', 0), FST::RELATION('w', 0), FST::RELATION('x', 0),
            FST::RELATION('y', 0), FST::RELATION('z', 0)),
        FST::NODE()
    );

    // Автомат для точки с запятой
    const FST::FST SEMICOLON (
        EMPTYstring, 2,
        FST::NODE(1, FST::RELATION(';', 1)),
        FST::NODE()
    );
}