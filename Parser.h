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
        static void dataStringOut(dataString Out);

        static char* ReadFile(const char* path, uint32_t& fileLength);  // text file input
        static dataString* ReadDataString(const char* path, char DividionSymbol, uint8_t timePos, uint8_t IDPos, uint8_t dataLenPos, uint8_t dataPos, uint32_t& size);

        static dataString StringToDataString(std::string BuffPtr, char DividionSymbol, const uint8_t timePos, const uint8_t IDPos, const uint8_t dataLen, const uint8_t dataPos);
};