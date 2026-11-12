#include "stdafx.h"
#include "Out.h"

#include <sstream>
#include <iostream>
using namespace std;

namespace Out {
	string Convert1251ToUTF8(const string& str) {
		if (str.empty()) return "";

		static const wchar_t cp1251_to_unicode[64] = {
			// 0xC0-0xCF
			0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
			0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
			// 0xD0-0xDF
			0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
			0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
			// 0xE0-0xEF
			0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
			0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
			// 0xF0-0xFF
			0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
			0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F
		};

		std::string result;
		result.reserve(str.length() * 2);

		for (unsigned char c : str) {
			if (c < 0x80) {
				result += c;
			}
			else if (c >= 0xC0 && c <= 0xFF) {
				wchar_t unicode = cp1251_to_unicode[c - 0xC0];

				if (unicode < 0x0800) {
					result += static_cast<char>(0xC0 | (unicode >> 6));
					result += static_cast<char>(0x80 | (unicode & 0x3F));
				}
				else {
					result += static_cast<char>(0xE0 | (unicode >> 12));
					result += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
					result += static_cast<char>(0x80 | (unicode & 0x3F));
				}
			}
			else {
				result += '?';
			}
		}

		return result;
	}





	OUT getout(wchar_t outfile[]) {
		OUT out;
		out.streamOut = new std::ofstream;
		out.streamOut->open(outfile, std::ios::binary);
		if (!out.streamOut->is_open())
		{
			throw ERROR_THROW(110);
		}

		// Пишем BOM для UTF-8
		const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
		out.streamOut->write((const char*)bom, 3);

		wcscpy_s(out.outfile, outfile);
		return out;
	}


	void WriteErrorOut(OUT out, Error::ERROR error) {
		stringstream ss;
		ss << "[ОШИБКА " + error.id << " : " << error.message << "; ";
		if (error.inext.line != -1) {
			ss << "строка " << error.inext.line << " позиция " << error.inext.col;
		}
		ss << "]";

		*out.streamOut << Convert1251ToUTF8(ss.str()) << endl;
	}


	void WriteInOut(OUT out, In::IN in) {
		*out.streamOut << Convert1251ToUTF8(string((const char*)in.text)) << std::endl;
	}


	void WriteString(OUT out, string str) {
		*out.streamOut << Convert1251ToUTF8(str);
	}


	void Close(OUT out) {
		out.streamOut->close();
		delete out.streamOut;
	}
};