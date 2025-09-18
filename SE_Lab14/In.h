#pragma once
#define IN_MAX_LEN_TEXT 1024*1024
#define IN_CODE_ENDL '\n'


/*
#define IN_CODE_TABLE {\
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::T,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::T,  IN::F,  IN::T,  IN::T,  IN::F,  IN::T,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::T,  IN::T,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
																																	\
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  \
	IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::F,  IN::F,  IN::F,  \
	IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::T,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  \
	IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F,  IN::F   \
}
*/

/*
#define IN_CODE_TABLE {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::I, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::I, IN::!, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::T, \
	IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::T, \
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::F, IN::F, IN::T, IN::F, IN::F, IN::T, IN::F, IN::F, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
	IN::T, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F  \
}
*/


#define IN_CODE_TABLE {\
/*	NUL		SOH		STX		ETX		EOT		ENQ		ACK		BEL		BS		TAB		LF		VT		FF		CR		SO		SI	*/   \
	IN::F,	 IN::F,	 IN::F,  IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		CAN		EM		SUB		ESC		FS		GS		RS		US	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	SP		!		"		#		$		%		&		'		(		)		*		+		,		-		.		/	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	0		1		2		3		4		5		6		7		8		9		:		;		<		=		>		?	*/   \
	IN::T,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	@		A		B		C		D		E		F		G		H		I		J		K		L		M		N		O	*/   \
	IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 \
/*	P		Q		R		S		T		U		V		W		X		Y		Z		[		\		]		^		_	*/   \
	IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::I,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	`		a		b		c		d		e		f		g		h		i		j		k		l		m		n		o	*/	 \
	IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 \
/*	p		q		r		s		t		u		v		w		x		y		z		{		|		}		~		DEL	*/   \
	IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::I,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
\
/*	Ђ		Ѓ		‚		ѓ		„		…		†		‡		€		‰		Љ		‹		Њ		Ќ		Ћ		Џ	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	ђ		‘		’		“		”		•		–		—		˜		™		љ		›		њ		ќ		ћ		џ	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	NBSP	Ў		ў		Ј		¤		Ґ		¦		§		Ё		©		Є		«		¬		SHY		®		Ї	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	°		±		І		і		ґ		µ		¶		·		ё		№		є		»		ј		ґ		ђ		ї	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	А		Б		В		Г		Д		Е		Ж		З		И		Й		К		Л		М		Н		О		П	*/   \
	IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 \
/*	Р		С		Т		У		Ф		Х		Ц		Ч		Ш		Щ		Ъ		Ы		Ь		Э		Ю		Я	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 \
/*	а		б		в		г		д		е		ж		з		и		й		к		л		м		н		о		п	*/   \
	IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 \
/*	р		с		т		у		ф		х		ц		ч		ш		щ		ъ		ы		ь		э		ю		я	*/   \
	IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::T,	 IN::T,	 IN::T,	 IN::F,	 IN::F,	 IN::F,	 IN::F,	 IN::F   \
}


namespace In
{
	struct IN
	{
		enum {T = 1024, F = 2048, I = 4096}; 
		int size;
		int lines;
		int ignor;
		unsigned char* text;
		int code [256];
	};
	IN getin(wchar_t infile[]);
};
