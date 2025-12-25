#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

#include <iostream>
using namespace std;


enum DataType : uint8_t {
    TYPE_INT = 0x01,
    TYPE_STRING = 0x02,
    TYPE_BOOL = 0x03,
    TYPE_SHORT = 0x04
};

struct Data {
    DataType type;
    union {
        bool boolValue;
        int16_t shortValue;
        int32_t intValue;
        string* stringValue;
    } value;

    Data() : type(TYPE_BOOL) { value.boolValue = false; }


    Data(const Data& other) {
        type = other.type;
        switch (type) {
        case TYPE_BOOL:
            value.boolValue = other.value.boolValue;
            break;
        case TYPE_SHORT:
            value.shortValue = other.value.shortValue;
            break;
        case TYPE_INT:
            value.intValue = other.value.intValue;
            break;
        case TYPE_STRING:
            if (other.value.stringValue) {
                value.stringValue = new string(*other.value.stringValue);
            }
            else {
                value.stringValue = nullptr;
            }
            break;
        }
    }
};


vector<Data> deserialize(const string& filename) {
    vector<Data> result;
    ifstream file(filename, ios::binary);

    if (!file) {
        cerr << "Ошибка открытия файла" << endl;
        return result;
    }

    while (file) {
        uint8_t typeByte;
        if (!file.read(reinterpret_cast<char*>(&typeByte), 1)) {
            break; 
        }

        Data item;
        item.type = static_cast<DataType>(typeByte);

        switch (item.type) {
        case TYPE_BOOL: {
            uint8_t boolVal;
            if (!file.read(reinterpret_cast<char*>(&boolVal), 1)) {
                cerr << "Ошибка чтения bool" << endl;
                return result;
            }
            item.value.boolValue = (boolVal != 0);
            break;
        }

        case TYPE_SHORT: {
            int16_t val = 0;
            uint8_t low, high;
            if (!file.read(reinterpret_cast<char*>(&low), 1) ||
                !file.read(reinterpret_cast<char*>(&high), 1)) {
                cerr << "Ошибка чтения short" << endl;
                return result;
            }
            val = low | (high << 8);
            item.value.shortValue = val;
            break;
        }

        case TYPE_INT: {
            int32_t val = 0;
            for (int i = 0; i < 4; i++) {
                uint8_t byte;
                if (!file.read(reinterpret_cast<char*>(&byte), 1)) {
                    cerr << "Ошибка чтения int" << endl;
                    return result;
                }
                val |= (byte << (i * 8));
            }
            item.value.intValue = val;
            break;
        }

        case TYPE_STRING: {
            uint32_t len = 0;
            for (int i = 0; i < 4; i++) {
                uint8_t byte;
                if (!file.read(reinterpret_cast<char*>(&byte), 1)) {
                    cerr << "Ошибка чтения длины строки" << endl;
                    return result;
                }
                len |= (byte << (i * 8));
            }

            string str(len, '\0');
            if (len > 0) {
                if (!file.read(&str[0], len)) {
                    cerr << "Ошибка чтения строки" << endl;
                    return result;
                }
            }
            item.value.stringValue = new string(str);
            break;
        }

        default:
            cerr << "Неизвестный тип: " << (int)typeByte << endl;
            return result;
        }

        result.push_back(item);
    }

    file.close();
    return result;
}


void printData(const vector<Data>& data) {
    cout << "Десериализованные данные:" << endl;
    cout << "=========================" << endl;

    for (size_t i = 0; i < data.size(); i++) {
        const Data& item = data[i];
        cout << i << ". ";

        switch (item.type) {
        case TYPE_BOOL:
            cout << "BOOL: " << (item.value.boolValue ? "true" : "false");
            break;

        case TYPE_SHORT:
            cout << "SHORT: " << item.value.shortValue;
            break;

        case TYPE_INT:
            cout << "INT: " << item.value.intValue;
            break;

        case TYPE_STRING:
            if (item.value.stringValue) {
                cout << "STRING: \"" << *item.value.stringValue << "\"";
            }
            else {
                cout << "STRING: (null)";
            }
            break;

        default:
            cout << "UNKNOWN: ???";
            break;
        }
        cout << endl;
    }
}


void generateAssembler(const string& filename, const vector<Data>& data) {
    ofstream asmFile(filename);
    if (!asmFile) {
        cerr << "Ошибка создания файла " << filename << endl;
        return;
    }

    asmFile << ".586\n";
    asmFile << ".model flat, stdcall\n";
    asmFile << "ExitProcess PROTO :DWORD\n";
    asmFile << "includelib kernel32.lib\n\n";
    //asmFile << "option casemap:none\n\n";
    asmFile << ".data\n\n";
    for (size_t i = 0; i < data.size(); i++) {
        const Data& item = data[i];

        asmFile << "; Element " << i << "\n";

        switch (item.type) {
        case TYPE_BOOL:
            asmFile << "data" << i << "_type db 03h      ; TYPE_BOOL\n";
            asmFile << "data" << i << "_value db " << (item.value.boolValue ? "1" : "0") << "\n";
            break;

        case TYPE_SHORT:
            asmFile << "data" << i << "_type db 04h      ; TYPE_SHORT\n";
            asmFile << "data" << i << "_value dw " << item.value.shortValue << "\n";
            break;

        case TYPE_INT:
            asmFile << "data" << i << "_type db 01h      ; TYPE_INT\n";
            asmFile << "data" << i << "_value dd " << item.value.intValue << "\n";
            break;

        case TYPE_STRING:
			asmFile << "data" << i << "_type db 02h      ; TYPE_STRING\n";
            if (item.value.stringValue) {
                asmFile << "data" << i << "_length dd " << item.value.stringValue->length() << "\n";
                asmFile << "data" << i << "_value db '" << *item.value.stringValue << "'\n";
            }
            else {
                asmFile << "data" << i << "_length dd 0\n";
                asmFile << "data" << i << "_value db 0\n";
            }
            break;
        }
        asmFile << "\n";
    }

    asmFile << ".code\n";
    asmFile << "start:\n";
    asmFile << "call ExitProcess\n";
    asmFile << "end start\n";   
    asmFile.close();
}


int main() {
    setlocale(LC_CTYPE, "Rus");
    string inputPath = "data.bin";
    vector<Data> data = deserialize(inputPath);

    if (data.empty()) {
        cerr << "Не удалось десериализовать данные" << endl;
        return 1;
    }

    printData(data);
    generateAssembler("asm.asm", data);
    return 0;
}