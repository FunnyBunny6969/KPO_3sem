#pragma once
#include <stack>
#include "GRB.h"
#include "LT.h"
#include "LEX.h"


#define MFST_TRACE_START \
	cout << setw(4) <<  std::setfill(' ') << left << "Шаг:"\
		<< setw(20) << left << " Правило" \
		<< setw(30) << left << " Входная лента" \
		<< setw(20) << left << " Стек" << endl;

#define MFST_TRACE1	cout << setw(4) << left << ++FST_TRACE_n << ": "\
					     << setw(20) << left << rule.getCRule(rbuf, nrulechain)\
					     << setw(30) << left << getCLenta(lbuf, lenta_position)\
					     << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE2	 cout << setw(4) << left << FST_TRACE_n << ": "\
					     << setw(20) << left << " "\
					     << setw(30) << left << getCLenta(lbuf, lenta_position)\
					     << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE3	cout << setw(4) << left << ++FST_TRACE_n << ": "\
					     << setw(20) << left << " "\
					     << setw(30) << left << getCLenta(lbuf, lenta_position)\
					     << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE4(c)	cout << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << c << endl;

#define MFST_TRACE5(c)	cout << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << c << endl;

#define MFST_TRACE6(c, k)	cout << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << c << k << endl;

#define MFST_TRACE7	cout << setw(4) << left << state.lenta_position<< ": "\
					     << setw(20) << left << rule.getCRule(rbuf, state.nrulechain) << endl;


#define MFST_RUN          \
		MFST_TRACE_START\
		MFST::Mfst mfst(tables, GRB::getGreibach()); \
		mfst.start(); \
		mfst.savededucation(); 
		//mfst.printrules();                 



#define MFST_DIAGN_MAXSIZE		2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3



class MFSTSTSTACK :public std::stack<short> { //стек автомата
public:
	using std::stack<short>::c;
};
//typedef std::stack<short> MFSTSTSTACK;    // стек автомата



namespace MFST
{
    struct MfstState    // состояние автомата (для сохранения)
    {
        short lenta_position;    // позиция на ленте
		short nrule;			 // номер текущего правила
        short nrulechain;        // номер текущей цепочки, текущего правила
        MFSTSTSTACK st;          // стек автомата

        MfstState();

        MfstState(
            short pposition,      // позиция на ленте
            MFSTSTSTACK pst,      // стек автомата
            short pnrulechain     // номер текущей цепочки, текущего правила
        );

		MfstState(
			short pposition,      // позиция на ленте
            MFSTSTSTACK pst,      // стек автомата
			short pnrule,         // номер текущего правила
			short pnrulechain     // номер текущей цепочки, текущего правила
		);

    };


	class MFSTSTATE :public std::stack<MfstState> { //стек автомата
	public:
		using std::stack<MfstState>::c;
	};



	struct Mfst    // магазинный автомат
	{
		enum RC_STEP {
			NS_OK,                 // шаг выполнен успешно
			NS_NORULE,             // найдено правило и цепочка, цепочка записана в стек
			NS_NORULECHAIN,        // не найдено правило грамматики (ошибка в грамматике)
			NS_ERROR,              // не найдена подходящая цепочка правила (ошибка в исходном коде)
			TS_OK,                 // тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,                // тек. символ ленты != вершине стека, восстановлено состояние
			LENTA_END,             // текущая позиция ленты >= lenta_size
			SURPRISE               // неизвестная ошибка
		};

		struct MfstDiagnosis    // диагностика
		{
			short lenta_position;    // позиция на ленте
			RC_STEP rc_step;         // код завершения шага
			short nrule;             // номер правила
			short nrule_chain;       // номер цепочки правила

			MfstDiagnosis();

			MfstDiagnosis(           // диагностика
				short plenta_position,  // позиция на ленте
				RC_STEP prc_step,       // код завершения шага
				short pnrule,           // номер правила
				short pnrule_chain      // номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER];    // последние самые глубокие сообщения



		GRBALPHABET* lenta;                 // перекодированная (T5/NS) лента (из LEX)
		short lenta_position;               // текущая позиция на ленте
		short nrule;                        // номер текущего правила
		short nrulechain;                   // номер текущей цепочки, текущего правила
		short lenta_size;                   // размер ленты
		GRB::Greibach grebach;              // грамматика Грейбах
		LEX::LEX lex;                       // результат работы лексического анализатора
		MFSTSTSTACK st;                     // стек автомата
		std::stack<MfstState> storestate;   // стек для сохранения состояний

		Mfst();
		Mfst(
			LEX::LEX plex,                  // результат работы лексического анализатора
			GRB::Greibach pgrebach         // грамматика Грейбах
		);

		char* getCSt(char* buf);                           // получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25); // лента: n символов с pos
		char* getDiagnosis(short n, char* buf);           // получить n-ую строку диагностики или 8x80
		bool savestate();                                 // сохранить состояние автомата
		bool reststate();                                 // восстановить состояние автомата
		bool push_chain(                                  // поместить цепочку правила в стек
			GRB::Rule::Chain chain                        // цепочка правила
		);
		RC_STEP step();                                   // выполнить шаг автомата
		bool start();                                     // запустить автомат
		bool savediagnosis(
			RC_STEP pprc_step                             // код завершения шага
		);

		void printrules();

		struct Deducation {
			short size;              // количество шагов в выводе
			short* nrules;           // номера правил грамматики
			short* nrulechains;      // номера цепочек правил грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;
		bool savededucation();                // сохранить дерево вывода
	};
};
