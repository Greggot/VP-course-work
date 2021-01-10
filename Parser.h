//**************************************************************************************************
// Includes
//**************************************************************************************************
#include <fstream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>

//**************************************************************************************************
// Definitions
//**************************************************************************************************

#define MAX_BUFFER_SIZE 0x2FFFFFFF   // 767 MB - Maximum buffer (input file) size
#define STRING_LENGTH_BYTE 8        // Average number of bytes in a string

#define MAX_SUBSTRING_NUMBER 0x20       // Maximum substring's number 
#define MAX_SUBSTRING_LENGTH 0x20       // MAximum string element's length
#define MAX_STRING_LENGTH_ASCII 0xFF    // Maximum string length - 255 symbols

#define TIME_POSITION 1         // Format:  ...TIME...ID...DATA     or      ...ID...TIME...DATA
#define ID_POSITION 3
#define DATA_LEN_POSITION 5
#define DATA_POSITION 6

#define NUMBER_OF_NON_SPN_ARGS 3    // Number of arguments at the begining of a console command that doesn't secify SPNData

#define ID_ARG_POS 1        // Stable command parts positions
#define BEGIN_ARG_POS 2
#define END_ARG_POS 3
#define OPERATION_ARG_POS 4

const uint8_t PLUS = 43;        // (int) '+'
const uint8_t MINUS = 45;       // (int) '-'
const uint8_t  MULTIPLY = 42;   // (int) '*'
const uint8_t  DIVIDE = 47;     // (int) '/'
const uint8_t _DIVIDE = 92;     // (int) '\\'

//**************************************************************************************************
// Structures and Data Types
//**************************************************************************************************

struct byteString           
{
    uint8_t bytes[STRING_LENGTH_BYTE];
};


struct dataString          // Useful data structure ID-DATA-TIME
{
    uint8_t DataLen;
    std::string ID;
    byteString Data;
    std::string TIME;
};

struct SPNData          // SPN operation's and file outut description
{
    std::string Name;   // Column's name
    std::string ID;     // ID

    std::vector <uint8_t> operation;       // Codes of operations
    std::vector <double> operationValue;   // SPN values    

    uint8_t begin;          // [begin-end] (bits) - is a useful data gap
    uint8_t end;
};

//**************************************************************************************************
// Declarations
//**************************************************************************************************

class Parser        // Class that is inherited by other Parse classes
{
    public:       
        static void byteStringOut(byteString Out);  // Console outputs
        static void dataStringOut(dataString Out);
        static void SPNDataOut(SPNData Out);

        static char* ReadFile(const char* path, uint32_t& fileLength);  // text file input
        static dataString* ReadDataString(const char* path, char DividionSymbol, uint8_t timePos, uint8_t IDPos, uint8_t dataLenPos, uint8_t dataPos, uint32_t& size);
        
        static byteString StringToByte(std::string Input);      // String to struct convertation
        static dataString StringToDataString(std::string BuffPtr);
        static dataString StringToDataString(std::string BuffPtr, char DividionSymbol, const uint8_t timePos, const uint8_t IDPos, const uint8_t dataLen, const uint8_t dataPos);
};
class SPNParser : public Parser
{
        dataString* ReadDataString(const char* path, uint32_t& stringNumber);   // Convert all strings into dataStrings
        dataString* ReadDataStringFromTable(const char* path, uint32_t& stringNumber);

        dataString* Data;       // TIME-DATA-ID
        uint32_t DataLength;

        SPNData* LogData;       // ID-operations's_data
        uint32_t LogDataColumnNumber;      

    public:
        static uint64_t getByteValue(byteString originalData, uint8_t begin, uint8_t end, bool IsBigEndian); // Get value via mask
        SPNParser(uint32_t LogDataColumnNumber,   std::string* Name,
                                                                    std::string* ID,   
                                                                    std::vector <std::vector<uint8_t>> operation,
                                                                    std::vector <std::vector<double>> operand,
                                                                    uint8_t* begin,
                                                                    uint8_t* end,
                                                                    dataString* Data,
                                                                    uint32_t DataLen);
        void CountSPNOutputIntoFile(const char* path, std::string LeadingID);
        ~SPNParser();
};