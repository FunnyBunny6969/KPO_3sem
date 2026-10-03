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
    TYPE_SHORT = 0x04,
    TYPE_UINT32 = 0x05 
};


struct Data {
    DataType type;
    union {
        bool boolValue;
        int16_t shortValue;
        int32_t intValue;
        uint32_t uint32Value;
        string* stringValue; 
    } value;

    Data(bool b)    : type(TYPE_BOOL) { value.boolValue = b; }
    Data(int16_t s) : type(TYPE_SHORT) { value.shortValue = s; }
    Data(int32_t i) : type(TYPE_INT) { value.intValue = i; }
    Data(uint32_t i) : type(TYPE_UINT32) { value.uint32Value = i; }
    Data(const string& s) : type(TYPE_STRING) {
        value.stringValue = new string(s);
    }
};


void serialize(const string& filename, const vector<Data>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    for (const auto& item : data) {
        file.put(static_cast<char>(item.type));

        if (item.type == TYPE_STRING) {
            uint32_t len = item.value.stringValue->length();
            // Пишем 4 байта длины (little-endian)
            for (int i = 0; i < 4; i++) {
                file.put(static_cast<char>(len & 0xFF));
                len >>= 8;
            }
        }

        switch (item.type) {
        case TYPE_BOOL:
            file.put(item.value.boolValue ? 1 : 0);
            break;

        case TYPE_SHORT: {
            int16_t val = item.value.shortValue;
            file.put(val & 0xFF);
            file.put((val >> 8) & 0xFF);
            break;
        }

        case TYPE_INT: {
            int32_t val = item.value.intValue;
            // 4 байта (little-endian)
            for (int i = 0; i < 4; i++) {
                file.put(val & 0xFF);
                val >>= 8;
            }
            break;
        }

        case TYPE_UINT32: {
            uint32_t val = item.value.uint32Value;

            for (int i = 0; i < 4; i++) {
                file.put(static_cast<char>(val & 0xFF));
                val >>= 8;
            }
            break;
        }

        case TYPE_STRING:
            file.write(
                item.value.stringValue->c_str(),
                item.value.stringValue->length()
            );
            break;
        }
    }

    file.close();
}


int main() {
    vector<Data> data;

    // ===== bool (переменная / литерал) =====
    data.push_back(Data(true));
    data.push_back(Data(false));

    // ===== short (знаковый, 2 байта) =====
    data.push_back(Data(static_cast<int16_t>(0)));
    data.push_back(Data(static_cast<int16_t>(-12345)));
    data.push_back(Data(static_cast<int16_t>(12)));
    data.push_back(Data(static_cast<int16_t>(32767)));    // SHRT_MAX
    data.push_back(Data(static_cast<int16_t>(-32768)));   // SHRT_MIN

    // ===== int (знаковый, 4 байта) =====
    data.push_back(Data(static_cast<int32_t>(0)));
    data.push_back(Data(static_cast<int32_t>(64000)));
    data.push_back(Data(static_cast<int32_t>(-2147483647 - 1)));  // INT_MIN
    data.push_back(Data(static_cast<int32_t>(2147483647)));       // INT_MAX

    // ===== uint32 (беззнаковый, 4 байта) =====
    data.push_back(Data(static_cast<uint32_t>(0)));
    data.push_back(Data(static_cast<uint32_t>(12345)));
    data.push_back(Data(static_cast<uint32_t>(0x12345678)));      // для проверки LE
    data.push_back(Data(static_cast<uint32_t>(4294967295u)));     // UINT_MAX

    // ===== string =====
    data.push_back(Data(string("Hello")));
    data.push_back(Data(string("W")));
    data.push_back(Data(string("")));   

    string outputPath = "data.bin";
    serialize(outputPath, data);
    return 0;
}