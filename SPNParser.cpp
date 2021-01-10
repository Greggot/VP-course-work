#include "Parser.h"

std::vector <double> op;
std::vector <uint8_t> opr;
SPNData emptySPNData = {"None", "None", opr, op, 0, 0};     // Empty operation description - does nothing to the none excisting id
//**************************************************************************************************
// Procedure SPNParser() 
//**************************************************************************************************

//   Constructor   //

SPNParser::SPNParser(uint32_t LogDataColumnNumber, std::string* Name,   std::string* ID,   
                                                                        std::vector <std::vector<uint8_t>> operation,
                                                                        std::vector <std::vector<double>> operand,
                                                                        uint8_t* begin,
                                                                        uint8_t* end,
                                                                        
                                                                        dataString* Data, uint32_t DataLen)
{
    this->Data = Data;
    this->DataLength = DataLen;

    this->LogDataColumnNumber = LogDataColumnNumber;        // Number of columns initialization
    this->LogData = new SPNData [LogDataColumnNumber];      // Methods' descriprions

    Parser::dataStringOut(Data[0]);
    std::cout<< "...\n";
    Parser::dataStringOut(Data[DataLength - 1]);

    for (uint8_t i = 0; i < LogDataColumnNumber; i++)
    {
        LogData[i] = emptySPNData;          // Fill empty descriptions with empty objects

        LogData[i].Name = Name[i];          // Change empty data for the values that've been translated from main()
        LogData[i].ID = ID[i];

        LogData[i].begin = begin[i];
        LogData[i].end = end[i];

        LogData[i].operation = operation[i];
        LogData[i].operationValue = operand[i];
    }

}

//**************************************************************************************************
// Procedure ~SPNParser() 
//**************************************************************************************************

//   Class destructor   //

SPNParser::~SPNParser()
{

}

//**************************************************************************************************
// Procedure ReadDataString() 
//**************************************************************************************************

//   Create dataString buffer from text file   //

dataString* SPNParser::ReadDataString(const char* path, uint32_t& stringNumber)
{
    uint32_t BufferLength = 0;
    
    char* Buffer = Parser::ReadFile(path, BufferLength);    // Read symbols
    char* BuffPtr = Buffer;

    dataString * Data = new dataString [BufferLength];  // Create buffer

    for (uint32_t i = 0; i < BufferLength; i++)     // For every string in data file
    {
        Data[i] = Parser::StringToDataString(BuffPtr);  // Translate usual string into dataString
        BuffPtr += MAX_STRING_LENGTH_ASCII; 
    } 
    delete Buffer;                  // There's no more need for symbol data
    stringNumber = BufferLength;    // Set link-variable equal string number
    return Data;
}

dataString* SPNParser::ReadDataStringFromTable(const char* path, uint32_t& stringNumber)
{
    uint32_t BufferLength = 0;
    
    char* Buffer = Parser::ReadFile(path, BufferLength);    // Read symbols
    char* BuffPtr = Buffer;

    dataString * Data = new dataString [BufferLength];  // Create buffer

    for (uint32_t i = 0; i < BufferLength; i++)     // For every string in data file
    {
        Data[i] = Parser::StringToDataString(BuffPtr, '\t', 0, 1, 255, 2);  // Translate usual string into dataString
        BuffPtr += MAX_STRING_LENGTH_ASCII; 
    } 
    delete Buffer;                  // There's no more need for symbol data
    stringNumber = BufferLength;    // Set link-variable equal string number
    return Data;
}

//**************************************************************************************************
// Procedure getByteValue() 
//**************************************************************************************************

//   Value generation via mask   //

uint64_t SPNParser::getByteValue(byteString originalData, uint8_t begin, uint8_t end, bool IsBigEndian)
{
    uint64_t out = 0;
    uint64_t mask = 0;
    uint8_t shift = 63 - end;   // How many bits to shift from right end

    for (uint8_t i = begin; i < end; i++)
    {
        mask += 1;              // Add one bit at the end
        mask = mask << 1;       // Move it to the left
    }
    mask = mask << shift;       // Shift 111...11 to the right position 

    if (IsBigEndian)        // If big endian 
    { 
        for (uint8_t i = 0; i < STRING_LENGTH_BYTE; i++)    // From left to the right 
        {
            out += originalData.bytes[i];           // Make a number from byteString
            if  (i != STRING_LENGTH_BYTE - 1)       // Shift only if it's not the last number in a string
                out = out << 8;
        }
        out = out & mask;               // Bit AND with a mask to erase garbage
        out = out >> (shift + 1);       // Move saved bits to the right (big endian)
    }
    else 
    {
        for (int i = STRING_LENGTH_BYTE - 1; i >= 0; i--)   // From right to the left
        {                                                   
            out += originalData.bytes[i];
            if (i != 0)
                out = out << 8;
        }
        out = out & mask;
        if (shift != 0)     // Had worked incorrect if last bit was needed before this 
            out = out >> (shift + 1);   
    }

    return out;

}

//**************************************************************************************************
// Procedure CountSPNOutputIntoFile() 
//**************************************************************************************************

//   Count SPN and output into file   //

void SPNParser::CountSPNOutputIntoFile(const char* path, std::string LeadingID)
{
    setlocale( LC_ALL, "Russian" );

    std::ofstream File;     // Output file
    File.open(path);

    double* values = new double [LogDataColumnNumber] {0};  // Buffer for values' synchroniztion

    File << "\t";       // tab is a dividion symbol
    for (uint32_t i = 0; i < LogDataColumnNumber; i++)
    {
        std::cout << LogData[i].Name << " ";    // Fill the first string with columns' names
        File << LogData[i].Name << '\t';
    }
    File << '\n';

    for (uint32_t i = 0; i < DataLength; i++)   // For every string in a Log
    {
        for (uint32_t j = 0; j < LogDataColumnNumber; j++)  // For every method
        {
            double value = 0;
            if (Data[i].ID == LogData[j].ID)    // If method is intended for this secific id
            {
                value = getByteValue (Data[i].Data, LogData[j].begin, LogData[j].end, false);   // Get value via mask
                for (size_t k = 0; k < LogData[j].operation.size(); k++)                        // Apply all k operatons
                {
                    switch(LogData[j].operation[k])                         // Has a numeric value converted from a char var, e.g. (int)'-'  
                    {
                        case PLUS:                                          
                            value += LogData[j].operationValue[k];
                            break;
                        case MINUS:
                            value -= LogData[j].operationValue[k];
                            break;
                        case MULTIPLY:
                            value *= LogData[j].operationValue[k];
                            break;
                        case DIVIDE:                                        // slash and reversed slash is used for dividion
                            value /= LogData[j].operationValue[k];
                            break;
                        case _DIVIDE:
                            value /= LogData[j].operationValue[k];
                            break;
                        default:
                            break;
                    }
                    values[j] = value;      // Input values in the buffer. Old values refresh by the new ones before leading id

                    if (Data[i].ID == LeadingID)    // If this id is leading, output all temporary values in a file
                    {
                        File << Data[i].TIME << '\t';
                        for (uint32_t k = 0; k < LogDataColumnNumber; k++)
                            File << values[k] << '\t';
                        File << '\n';
                    }
                }
            }
        }
    }

    File.close();
}