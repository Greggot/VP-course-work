#include "Parser.h"

//**************************************************************************************************
// Global variable
//**************************************************************************************************

byteString empbyteString {{0,0,0,0,0,0,0,0}}; //Empty byteString


//**************************************************************************************************
// Procedure byteStringOut()
//**************************************************************************************************

// byteString console output //

void Parser::byteStringOut(byteString Out)
{
    for (uint8_t i = 0; i < STRING_LENGTH_BYTE; i++)
        std::cout << std::hex << (int) Out.bytes[i] << '\t';
    std::cout << std::endl;
}


//**************************************************************************************************
// Procedure StringToByte()
//**************************************************************************************************

// String to byteString convertion //

byteString Parser::StringToByte(std::string Input)
{
        byteString Output = empbyteString;    //Empty string created
        
        uint8_t Count = 0;              //Byte's order in a string
        std::string buff = "";          //Buffer containing substring that's been gotten by dividion original string with '\t'

        uint8_t cycleLength = Input.size() + 1;         
        for (uint8_t i = 0; i < cycleLength; i++)       
        {
            if (Input[i] == '\t' || Input[i] == '\x0')          //If tab or an end of a string
            {
                Output.bytes[Count] = (uint8_t) std::stoi(buff, 0, 16); //Converst substring into hex number
                buff = "";                                              //Clear buffer
                Count++;
            }
            buff += Input[i];
        }
        return Output;
}


//**************************************************************************************************
// Procedure ReadFile() 
//**************************************************************************************************

//   Read file and put it into char buffer    //

char* Parser::ReadFile(const char* path, uint32_t& fileLength)
{
    std::fstream File;
    File.open(path);

    uint32_t byteCounter = 0;
    
    char* Buffer = new  char [MAX_BUFFER_SIZE] {' '}; // Create buffer for all symbols
    char* BuffPtr = Buffer;     // Current buffer location pointer

    while (File.getline(BuffPtr, MAX_STRING_LENGTH_ASCII, '\n'))    // While there's any unread strig in a text file
    {  
        BuffPtr += MAX_STRING_LENGTH_ASCII;         // Move pointer further by the number of symbols in a string. All symbols between '\x0' and next string is 0 (empty space)
        byteCounter++;                              // Count strings' number
    }

    fileLength = byteCounter;   // Output file length via left-sided reference type 
    return Buffer;              // Thus returning buffer and it's length. Structure ot these both would be pain as well as going through the file twice: 1. Count strings 2. Read data
}

//**************************************************************************************************
// Procedure dataStringOut() 
//**************************************************************************************************

//   dataString console output   //

void Parser::dataStringOut(dataString out)   // File output works the same way
{

    std::cout << out.TIME << "\t";     // Time output
    std::cout << out.ID << "\t";        // Output ID

    for (uint8_t i = 0; i < out.DataLen; i++)   // Output all data bytes
        std::cout << (int) out.Data.bytes[i] << "\t";
    if(out.DataLen < STRING_LENGTH_BYTE)        // If number of data bytes in a string is less then usual ideal number...
    {
        uint8_t differense = STRING_LENGTH_BYTE - out.DataLen;
        for (uint8_t i = 0; i < differense; i++)
            std::cout << "\t";                  //...fill the ga between data end and time
    }
    std::cout << std::endl;
}

//**************************************************************************************************
// Procedure SPNDataOut() 
//**************************************************************************************************

//   SPNData console output   //

void Parser::SPNDataOut(SPNData Out)
{
    std::cout << Out.Name << ": \n";
    std::cout << "ID: " << Out.ID << ", Operation: ";
    
    size_t operationCount = Out.operation.size();
    for (size_t i = 0; i < operationCount; i++)
        std::cout << Out.operation[i] << Out.operationValue[i] << '\n';
}

//**************************************************************************************************
// Procedure StringToDataString() 
//**************************************************************************************************

//   String to DataString convertation for a strict format log files   //

dataString Parser::StringToDataString(std::string BuffPtr)  
{
    dataString Data;                // Output
    uint8_t subStringCounter = 0;   // Numer of read elements in a string
    std::string buff = "";          // Buffer for an element

    uint8_t dataCounter = 0;        

    uint8_t j = 0;
    while (true)
    {
            
        bool IsSpace = BuffPtr[j] == ' ' || BuffPtr[j] == '\x0';    // If dividion symbol
            
        while (BuffPtr[j] == ' ')   // Skip all of them
           j++;

        if (IsSpace)
        {
            switch (subStringCounter)   // If element's number is target element number, construct dataString
            {
                case TIME_POSITION:         // Optimised using char and definitions in a Parser.h
                    Data.TIME = buff;
                    break;
                case ID_POSITION:
                    Data.ID = buff;
                    break;
                case DATA_LEN_POSITION:
                    Data.DataLen = std::stoi(buff, 0, 16);
                    break;
                default:
                    if (subStringCounter >= DATA_POSITION)  // Data is the last elemet in a string
                    {
                        Data.Data.bytes[dataCounter] = std::stoi(buff, 0, 16);
                        dataCounter++;
                    }
            }
            subStringCounter++;
            buff = "";
        }
        buff += BuffPtr[j];
            
        if (BuffPtr[j] == '\x0')    // End cycle if current symbol is the end of a string
            break;

        j++;
    }
    
    return Data;
}

//**************************************************************************************************
// Procedure StringToDataString() 
//**************************************************************************************************

//   String to DataString user-defined-format convertation  //

dataString Parser::StringToDataString(std::string BuffPtr, char DividionSymbol, uint8_t timePos, uint8_t IDPos, uint8_t dataLenPos, uint8_t dataPos)  
{                                    // Procedure operate with one string, dividion symbol, given symbol positions
    dataString Data;                // Output

    uint8_t subStringCounter = 0;   // Numer of read elements in a string
    std::string buff = "";          // Buffer for an element
    std::vector <std::string> Strings;

    uint8_t dataCounter = 0;        

    uint8_t j = 0;

    while (true)
    {
            
        bool IsSpace = BuffPtr[j] == DividionSymbol || BuffPtr[j] == '\x0';    // If dividion symbol
            
        while (BuffPtr[j] == DividionSymbol)   // Skip all of them
           j++;

        if (IsSpace)
        {
            if (subStringCounter == timePos)
                Data.TIME = buff;
            
            if (subStringCounter == IDPos)
                Data.ID = buff;

            if (dataLenPos == 255)
                Data.DataLen = STRING_LENGTH_BYTE;
            else if(subStringCounter == dataLenPos)
                    Data.DataLen = std::stoi(buff, 0, 16);
            
            if (subStringCounter >= dataPos && dataCounter < Data.DataLen)  // Data is the last elemet in a string
            {
                Data.Data.bytes[dataCounter] = std::stoi(buff, 0, 16);
                dataCounter++;
            }
            subStringCounter++;
            Strings.push_back(buff);
            buff = "";
        }
        buff += BuffPtr[j];
            
        if (BuffPtr[j] == '\x0')    // End cycle if current symbol is the end of a string
            break;

        j++;
    }

    if(Data.DataLen < STRING_LENGTH_BYTE)
    {
        uint8_t diff = STRING_LENGTH_BYTE - Data.DataLen;
        uint8_t startPos = Data.DataLen;
        Data.DataLen = STRING_LENGTH_BYTE;
        
        Parser::byteStringOut(Data.Data);
        std::cout << (int) diff << std::endl;

        for (uint8_t i = startPos; i < STRING_LENGTH_BYTE; i++)
            Data.Data.bytes[i] = 0;
        if (timePos > dataPos)
            Data.TIME = Strings[timePos - diff];
        if (IDPos > dataPos)
            Data.TIME = Strings[IDPos - diff];
    }
    
    return Data;
}

dataString* Parser::ReadDataString(const char* path, char DividionSymbol, uint8_t timePos, uint8_t IDPos, uint8_t dataLenPos, uint8_t dataPos, uint32_t& size)
{
    uint32_t StringNumber = 0;
    char* Buffer = Parser::ReadFile(path, StringNumber);
    char* BuffPtr = Buffer;

    dataString* Data = new dataString [StringNumber];

    for (uint32_t i = 0; i < StringNumber; i++)
    {
        Data[i] = Parser::StringToDataString(BuffPtr, DividionSymbol, timePos, IDPos, dataLenPos, dataPos);
        BuffPtr += MAX_STRING_LENGTH_ASCII;
    }

    size = StringNumber;
    return Data;
}