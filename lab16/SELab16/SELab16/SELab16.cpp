#include "FST.h"  
#include <tchar.h> 
#include <cstdlib> 
#include <cstring>

#include <iostream>
using namespace std;


void testChain(char[]);


int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");
    char str1[] = "aaabbbaba";
    char str2[] = "aaabbbabba";


    FST::FST fst1(    // недегерминированный конечный автомат (a+b)*aba
        str1,    // цепочка для распознавания
        4,    // количество состояний
        FST::NODE(3, FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('a', 1)), // состояние 0 (начальное )
        FST::NODE(1, FST::RELATION('b', 2)),    // состояние 1
        FST::NODE(1, FST::RELATION('a', 3)),    // состояние 2
        FST::NODE()    // состояние 3 (конечное)
        );
	if (FST::execute(fst1))  // выполнить разбор
		std::cout << "Цепочка " << fst1.string << " распознана" << std::endl;
	else std::cout << "Цепочка " << fst1.string << " не распознана" << std::endl;


	FST::FST fst2(    // недегерминированный конечный автомат (a+b)*aba
		str2,    // цепочка для распознавания
		4,    // количество состояний
		FST::NODE(3, FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('a', 1)), // состояние 0 (начальное )
		FST::NODE(1, FST::RELATION('b', 2)),    // состояние 1
		FST::NODE(1, FST::RELATION('a', 3)),    // состояние 2
		FST::NODE()    // состояние 3 (конечное)
		);
	if (FST::execute(fst2))  // выполнить разбор
		std::cout << "Цепочка " << fst2.string << " распознана" << std::endl;
	else std::cout << "Цепочка " << fst2.string << " не распознана" << std::endl;


    char test[] = "open    write;read;seek;         close";
    testChain(test);

    strncpy_s(test, "open     write; close", sizeof(test) - 1);
    testChain(test);

    strncpy_s(test, "open write; close", sizeof(test) - 1);
    testChain(test);

    strncpy_s(test, "open   read;seek;seek;write; close", sizeof(test) - 1);
    testChain(test);
    
    strncpy_s(test, "open  seek;seek;read;    close", sizeof(test) - 1);
    testChain(test);

    strncpy_s(test, "open write; clos", sizeof(test) - 1);
    testChain(test);
    strncpy_s(test, "open  write; seek; close", sizeof(test) - 1);
    testChain(test);
    strncpy_s(test, "Open     write; close", sizeof(test) - 1);
    testChain(test);



	system("pause");
	return 0;
}


void testChain(char testString[]) {
    FST::FST fst(
        testString,
        24,

        FST::NODE(1, FST::RELATION('o', 1)),
        FST::NODE(1, FST::RELATION('p', 2)),
        FST::NODE(1, FST::RELATION('e', 3)),
        FST::NODE(1, FST::RELATION('n', 4)),

        FST::NODE(1, FST::RELATION(' ', 5)),

        FST::NODE(4,
            FST::RELATION(' ', 5),
            FST::RELATION('w', 6),
            FST::RELATION('r', 10),
            FST::RELATION('s', 13)
        ),

        FST::NODE(1, FST::RELATION('r', 7)),
        FST::NODE(1, FST::RELATION('i', 8)),
        FST::NODE(1, FST::RELATION('t', 9)),
        FST::NODE(1, FST::RELATION('e', 16)),

        FST::NODE(1, FST::RELATION('e', 11)),
        FST::NODE(1, FST::RELATION('a', 12)),
        FST::NODE(1, FST::RELATION('d', 16)),

        FST::NODE(1, FST::RELATION('e', 14)),
        FST::NODE(1, FST::RELATION('e', 15)),
        FST::NODE(1, FST::RELATION('k', 16)),

        FST::NODE(1, FST::RELATION(';', 17)),

        FST::NODE(4,
            FST::RELATION('w', 6),
            FST::RELATION('r', 10),
            FST::RELATION('s', 13),
            FST::RELATION(' ', 18)
        ),

        FST::NODE(2,
            FST::RELATION(' ', 18),
            FST::RELATION('c', 19)
        ),

        FST::NODE(1, FST::RELATION('l', 20)),
        FST::NODE(1, FST::RELATION('o', 21)),
        FST::NODE(1, FST::RELATION('s', 22)),
        FST::NODE(1, FST::RELATION('e', 23)),

        FST::NODE()

    );

    if (FST::execute(fst))  cout << "цепочка \"" << fst.string << "\" распознана" << endl;
    else                     cout << "цепочка \"" << fst.string << "\" не распознана" << endl;
}