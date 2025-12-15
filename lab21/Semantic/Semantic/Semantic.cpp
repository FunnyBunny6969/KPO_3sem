#include "Semantic.h"
#include <cstring> 



namespace SemanticAnalyzer
{





    void RunSemanter(LT::LexTable& lextable, IT::IdTable& idtable)
    {
        char lexema;
		LT::Entry entry;



        bool mainFound = false;
        int mainNum = 0;



        for (int i = 0; i < lextable.size; i++)
        {
            entry = lextable.table[i];
            lexema = entry.lexema[0];


            switch (lexema)
            {
            case LEX_MAIN: mainFound = true; mainNum++; break;

            default: break;
            }

        }




        if (!mainFound) throw ERROR_THROW(701);
        if (mainNum != 1) throw ERROR_THROW(702);
    }
}
