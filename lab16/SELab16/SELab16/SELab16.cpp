#include <iostream>
#include "FST.h"  
#include <tchar.h> 

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");

    FST::FST fst1(    // недетерминированный конечный автомат (a+b)*aba
        "aaabbbaba",    // цепочка для распознавания
        4,    // количество состояний
        FST::NODE(3, FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('a', 1)), // состояние 0 (начальное)
        FST::NODE(1, FST::RELATION('b', 2)),    // состояние 1
        FST::NODE(1, FST::RELATION('a', 3)),    // состояние 2
        FST::NODE()    // состояние 3 (конечное)
    );

    if (FST::execute(fst1))   // выполнить разбор
        std::cout << "цепочка " << fst1.string << " распознана" << std::endl;
    else
        std::cout << "цепочка " << fst1.string << " не распознана" << std::endl;

    FST::FST fst2(    // недетерминированный конечный автомат (a+b)*aba
        "aaabbbab",    // цепочка для распознавания
        4,    // количество состояний
        FST::NODE(3, FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('a', 1)), // состояние 0 (начальное)
        FST::NODE(1, FST::RELATION('b', 2)),    // состояние 1
        FST::NODE(1, FST::RELATION('a', 3)),    // состояние 2
        FST::NODE()    // состояние 3 (конечное)
    );

    if (FST::execute(fst2))   // выполнить разбор
        std::cout << "цепочка " << fst2.string << " распознана" << std::endl;
    else
        std::cout << "цепочка " << fst2.string << " не распознана" << std::endl;

    system("pause");
    return 0;
}