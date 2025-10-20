#include "stdafx.h"
#include "Out.h"

#include <iostream>
using namespace std;

namespace Out {
	OUT getout(wchar_t outfile[]) {
		OUT out;
		out.streamOut = new std::ofstream;
		out.streamOut->open(outfile);
		if (!out.streamOut->is_open())
		{
			throw ERROR_THROW(110);
		}
		wcscpy_s(out.outfile, outfile);
		return out;
	}


	void WriteErrorOut(OUT out, Error::ERROR error) {
		*out.streamOut << "[ÎØÈÁÊÀ " << error.id 
			<< " : " << error.message << "; ";
		if (error.inext.line != -1)
		{
			*out.streamOut
				<< "ñòðîêà " << error.inext.line
				<< " ïîçèöèÿ " << error.inext.col;
		}
		*out.streamOut << "]" << endl;
	}


	void WriteInOut(OUT out, In::IN in) {
		*out.streamOut << in.text << endl;
	}


	void Close(OUT out) {
		out.streamOut->close();
		delete out.streamOut;
	}
};