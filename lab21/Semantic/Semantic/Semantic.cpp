#include "Semantic.h"
#include <cstring> 



namespace SemanticAnalyzer
{
	void checkFuncParms(LT::LexTable& lextable, IT::IdTable& idtable, int start) {
		IT::Entry funcInfo = idtable.table[lextable.table[start].idxTI];
		int num = 0;
		char lexema;

		for (int i = 0; i < lextable.size; i++) {
			lexema = lextable.table[i].lexema[0];

			if (lexema == LEX_ID) {
				if (num > funcInfo.func_meta.n_params) throw ERROR_THROW(717);
				if (idtable.table[lextable.table[i].idxTI].iddatatype !=
					funcInfo.func_meta.params_types[num])
					throw ERROR_THROW(716);
				num++;
			}

			if (lexema == LEX_SEMICOLON) {
				if (num < funcInfo.func_meta.n_params) throw ERROR_THROW(718);
			}
		}
	}


	bool isTypeCompatible(IT::IDDATATYPE type1, IT::IDDATATYPE type2) {
		// Числовые типы (UINT и CHAR) совместимы между собой
		if (
			(type1 == IT::UINT || type1 == IT::CHAR) && 
			(type2 == IT::UINT || type2 == IT::CHAR)) {
			return true; }

		// Строки совместимы только со строками (для конкатенации)
		// if (type1 == IT::STR && type2 == IT::STR) {	return true; }
		// 
		// CHAR и STR могут быть совместимы для конкатенации (STR + CHAR).
		//if ((type1 == IT::STR && type2 == IT::CHAR) || 
		//	(type1 == IT::CHAR && type2 == IT::STR)) {
		//	return true; }

		// Все остальные комбинации несовместимы (например, STR + UINT)
		return false;
	}


	// 2. Разрешение результирующего типа (Type Promotion)
	IT::IDDATATYPE resolveResultType(IT::IDDATATYPE type1, IT::IDDATATYPE type2) {
		// UINT - самый "широкий" из числовых
		if (type1 == IT::UINT || type2 == IT::UINT) {
			return IT::UINT;
		}
		// Конкатенация с CHAR или STR всегда дает STR
		if (type1 == IT::STR || type2 == IT::STR) {
			return IT::STR;
		}
		// CHAR + CHAR = UINT (если интерпретируется как арифметическая операция)
		if (type1 == IT::CHAR && type2 == IT::CHAR) {
			return IT::UINT;
		}
		return IT::UNDEF;
	}

	// 3. Проверка, является ли лексема оператором
	bool isOperator(char lexema) {
		return lexema == LEX_PLUS || lexema == LEX_MINUS || lexema == LEX_STAR ||
			lexema == LEX_DIRSLASH || lexema == LEX_BIT_AND || lexema == LEX_BIT_OR ||
			lexema == LEX_BIT_NOT;
	}



	IT::IDDATATYPE determineExpressionType(LT::LexTable& lextable, IT::IdTable& idtable, int& i) {

		IT::IDDATATYPE currentType = IT::UNDEF;

		// ВАЖНО: Мы используем i (ссылку) для управления текущей позицией
		for (/* i уже установлен */; i < lextable.size; i++) {
			char lexema = lextable.table[i].lexema[0];
			int idxTI = lextable.table[i].idxTI;

			// 1. Конец выражения (используется для выхода из рекурсии)
			if (lexema == LEX_SEMICOLON ||
				lexema == LEX_COMMA ||
				lexema == LEX_RIGHTHESIS) {
				break;
			}

			// 2. Обработка операнда: ID, LITERAL или ФУНКЦИЯ
			if (lexema == LEX_ID || lexema == LEX_LITERAL) {

				IT::IDDATATYPE operandType = idtable.table[idxTI].iddatatype;

				// Если ID - это функция, проверяем следующий токен (
				if (lexema == LEX_ID && idtable.table[idxTI].idtype == IT::F &&
					lextable.table[i + 1].lexema[0] == LEX_LEFTHESIS) {

					// --- ВЫЗОВ ФУНКЦИИ ---
					// Тип операнда - это тип возвращаемого значения функции
					operandType = idtable.table[idxTI].iddatatype;

					// Необходимо пропустить все лексемы, составляющие вызов (аргументы)
					// Для упрощения, мы просто найдем соответствующую LEX_RIGHTHESIS.
					int openParens = 1;
					i += 2; // Пропускаем ID и (
					while (i < lextable.size && openParens > 0) {
						if (lextable.table[i].lexema[0] == LEX_LEFTHESIS) openParens++;
						if (lextable.table[i].lexema[0] == LEX_RIGHTHESIS) openParens--;
						i++;
					}
					i--; // Компенсируем i++, который будет в конце цикла for.
				}
				// --- Конец Вызова Функции ---


				if (currentType == IT::UNDEF) {
					currentType = operandType;
				}
				else {
					// Применяем правила совместимости и повышения типов
					if (!isTypeCompatible(currentType, operandType)) {
						//throw ERROR_THROW_IN(720, currentLine, "Несовместимые типы в выражении.");
					}
					currentType = resolveResultType(currentType, operandType);
				}
			}
			else if (lexema == LEX_LEFTHESIS) {
				// Это начало вложенного выражения (E)
				// Мы должны рекурсивно вызвать determineExpressionType

				i++; // Пропускаем (
				int startIndexForSubExpr = i;

				// ВАЖНО: Мы должны здесь временно сохранить i, чтобы рекурсивный вызов
				// мог обновить его, указав на конец внутреннего выражения.

				IT::IDDATATYPE subExprType = determineExpressionType(lextable, idtable, i);

				// i теперь указывает на LEX_RIGHTHESIS (или конец).

				if (currentType == IT::UNDEF) {
					currentType = subExprType;
				}
				else {
					if (!isTypeCompatible(currentType, subExprType)) {
						//throw ERROR_THROW_IN(720, currentLine, "Несовместимые типы в выражении (после скобки).");
					}
					currentType = resolveResultType(currentType, subExprType);
				}
			}
			else if (isOperator(lexema)) {
				// Оператор: просто пропускаем и ждем следующий операнд.
				continue;
			}
		}
		return currentType;
	}


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



			case LEX_ID:
				if (idtable.table[entry.idxTI].idtype == IT::F) {
					if (idtable.table[entry.idxTI].func_meta.n_params > IT_MAX_PARAMS)
						throw ERROR_THROW(706);
					if (lextable.table[i - 1].lexema[0] != LEX_FUNCTION)
						checkFuncParms(lextable, idtable, i);

				}
				break;



			case LEX_EQUALS:

				break;
			default: break;
			}

		}




		if (!mainFound) ERROR_THROW(701);
		if (mainNum != 1) ERROR_THROW(702);
	}


}
