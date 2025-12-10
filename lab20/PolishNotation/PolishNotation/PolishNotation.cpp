#include "PolishNotation.h"


namespace PN
{
	int GetPriority(char lexema)
	{
		switch (lexema)
		{
		case LEX_LEFTHESIS:
		case LEX_RIGHTHESIS:
			return 0;
		case LEX_COMMA:
			return 1;
		case LEX_EQUALS:
			return 2;
		case LEX_PLUS:
		case LEX_MINUS:
			return 4;
		case LEX_STAR:
		case LEX_DIRSLASH:
			return 5;
		default:
			return -1;
		}
	}


	bool Poliz(int i, LT::LexTable& lextab, IT::IdTable& idtable)
	{
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		// Создаем временную запись для заполнения пустых позиций
		LT::Entry temp;
		temp.idxTI = LT_TI_NULLIDX;
		temp.lexema[0] = '~';
		temp.sn = lextab.table[i].sn;


		LT::Entry func;
		func.idxTI = LT_TI_NULLIDX;
		func.lexema[0] = '@';
		func.sn = lextab.table[i].sn;

		int countLex = 0, countComma = 0;
		int countParm = 1;
		int posLex = i;
		char* buf = new char[1];

		bool findComma = false;
		bool findParm = false;
		bool callFunc = false;
		bool result = true;

		for (i; lextab.table[i].lexema[0] != LEX_SEMICOLON; i++, countLex++)
		{
			switch (lextab.table[i].lexema[0])
			{
			case LEX_ID:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_LITERAL:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_PLUS:
			{
				while (!stack.empty() && 
					GetPriority(lextab.table[i].lexema[0]) <= GetPriority(stack.top().lexema[0]))
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lextab.table[i]);
				continue;
			}
			case LEX_COMMA:
			{
				findComma = true;
				countComma++;
				continue;
			}
			case LEX_LEFTHESIS:
			{
				stack.push(lextab.table[i]);
				if ((lextab.table[i + 1].lexema[0] == LEX_ID || 
					(lextab.table[i + 1].lexema[0] == LEX_LITERAL || 
					lextab.table[i + 2].lexema[0] == LEX_LITERAL)) && 
					lextab.table[i - 1].lexema[0] == LEX_ID)
				{
					findParm = true;
					callFunc = true;
				}
				continue;
			}
			case LEX_RIGHTHESIS:
			{
				while (stack.top().lexema[0] != LEX_LEFTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}
				if (callFunc)
				{
					if (findComma)
						countParm = countComma + 1;
					else if (findParm)
						countParm = 1;
					else
						countParm = 0;
					countComma = 0;
					lextab.table[i] = func;
					queue.push(lextab.table[i]);
					_itoa_s(countParm, buf, 2, 10);
					stack.top().lexema[0] = buf[0];
					stack.top().idxTI = func.idxTI;
					stack.top().sn = func.sn;
					queue.push(stack.top());
					callFunc = false;
				}
				stack.pop();
				continue;
			}
			}
		}

		while (!stack.empty())
		{
			if (stack.top().lexema[0] == LEX_LEFTHESIS || 
				stack.top().lexema[0] == LEX_RIGHTHESIS)
				return false;
			queue.push(stack.top());
			stack.pop();
		}

		while (countLex)
		{
			if (!queue.empty())
			{
				lextab.table[posLex++] = queue.front();
				queue.pop();
			}
			else
			{
				lextab.table[posLex++] = temp;
			}
			countLex--;
		}

		for (int i = 0; i < posLex; i++)
		{
			if (lextab.table[i].lexema[0] == LEX_LITERAL || 
				lextab.table[i].lexema[0] == LEX_ID)
				idtable.table[lextab.table[i].idxTI].idxfirstLE = i;
		}
		return true;
	}

	bool StartPoliz(LT::LexTable& lextab, IT::IdTable& idtable)
	{

		bool result = false;
		for (int i = 0; i < lextab.size; i++)
		{
			if (lextab.table[i].lexema[0] == LEX_EQUALS ||
				(lextab.table[i].lexema[0] == LEX_ID && lextab.table[i - 1].lexema[0] != LEX_EQUALS
					&& lextab.table[i - 1].lexema[0] != LEX_PLUS/* && lextab.table[i - 1].lexema != LEX_UNARY_MINUS && lextab.table[i - 1].lexema != LEX_LESS*/
					&& idtable.table[lextab.table[i].idxTI].idtype == IT::IDTYPE::F
					&& lextab.table[i - 1].lexema[0] != LEX_FUNCTION))
			{
				result = Poliz(i + 1, lextab, idtable);
				if (!result)
					return result;
			}
		}
		return result;
	}

}