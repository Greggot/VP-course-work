#include "Parser.h"

uint8_t nulloperator [1] = {0};
double nulloperand [1] = {0.f};

int main(int argc, char *argv[])    // Format: PROGRAM_NAME PATH_TO_FILE {NAME-ID-BEGIN-END-SIGN_OPERAND-SIGN_OPERAND...} {NAME-ID-BEGIN-END-SIGN_OPERAND...} ... SAVE_PATH LEADING_ID
{                                   // ATTENTION! All operations are executed not by algebra rules - by console given order
    char* temp = new char [16];    // 16 is a length of a maximum possible SPN value
    uint32_t columnNumber = std::stoi(argv[2], 0, 10); // Maximum column number without program name and path to a file

    std::string LeadingID = argv[argc - 1];
    const char* outputPath = argv[argc - 2];

    std::string* Name = new std::string [columnNumber] {""};    // Buffers for SPNData identification 
    std::string* ID = new std::string [columnNumber] {""};   
    
    std::vector <std::vector<uint8_t>> operation;       // Vectors for simlicity and not so big amount of operations. This vector has all operation symbol codes for every ID
    std::vector <std::vector<double>> operand;          // Vector for SPN values. In ideal world these vectors're the same size

    uint8_t* begin = new uint8_t [columnNumber] {0};    // Positions for a mask to get the right value from a string
    uint8_t* end = new uint8_t [columnNumber] {0};

    uint32_t argCounter = NUMBER_OF_NON_SPN_ARGS;       // Index of current argument location
    for (uint32_t i = 0; i < columnNumber; i++)         
    {
        Name[i] = argv[argCounter];
        ID[i] = argv[argCounter + ID_ARG_POS];
        begin[i] = std::stoi (argv[argCounter + BEGIN_ARG_POS], 0, 10);
        end[i] = std::stoi (argv[argCounter + END_ARG_POS], 0, 10);

        argCounter += END_ARG_POS;      // Move it for a usual stable part of operation description
        argCounter++;

        std::vector <uint8_t> tempOprtn;    // Buffers for operation instructions part
        std::vector <double> tempOprnd;

        for (int j = argCounter; j < argc; j++)    // "while" was replaced with "for" to avoide out of array runtime error. Also we know for sure this cycle's iteration
        {                                          // number is less then argument count
            if(argv[argCounter][0] != '+' && argv[argCounter][0] != '-' &&      // If any next argument's first symbol isn't oeration symbol, this cycle brakes
               argv[argCounter][0] != '*' && argv[argCounter][0] != '\\' &&
               argv[argCounter][0] != '/')
               break;
            
            uint8_t sign = (int) argv[argCounter][0];                       // char to uin8_t convertion 
            strncpy(temp, argv[argCounter] + 1, sizeof(argv[argCounter]));  // and copying string's symbols from 1 to last. E.g. +23 -> 43 23.f -> "summ value with 23" in a CountSPN...()
            
            tempOprtn.push_back(sign);
            tempOprnd.push_back(std::stod(temp, 0));

            argCounter++;
        }
        operation.push_back(tempOprtn);
        operand.push_back(tempOprnd);

        tempOprnd.clear();
        tempOprtn.clear();
    }

    bool IsRealID = false;  //If lead ID will be corrupted output fill probably will be empty

    for (uint32_t i = 0; i < columnNumber; i++)
    {
        if (ID[i] == LeadingID)
            IsRealID = true;
    }

    if (!IsRealID)
    {
        exit(1);
    }
    
    char DivSymb = ' ';
    if (std::stoi(argv[argc - 7], 0, 10) != 0)
        DivSymb = '\t';

    uint32_t FileLen = 0;
    dataString* Data = Parser::ReadDataString(argv[1], DivSymb, std::stoi(argv[argc - 6], 0, 10), std::stoi(argv[argc - 5], 0, 10), std::stoi(argv[argc - 4], 0, 10), 
    std::stoi(argv[argc - 3], 0, 10), FileLen);

    SPNParser Parse (columnNumber, Name, ID, operation, operand, begin, end, Data, FileLen);  // If there're no exeptions create object
    Parse.CountSPNOutputIntoFile(outputPath, LeadingID);                                   // Output SPN table into file
    Parse.~SPNParser();                                                                 // Delete object

    // Example command for a console that asks for the 6-column table with Torque (ID is 0CF00300), EngineSpeed... from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt:
    //"SPNParser ss_2_00000034.dat.002 6 Torque 0CF00300 40 48 +0 EngineSpeed 0CF00400 24 40 *0.125 Intakeressure 18FEF600 48 56 *2 OutLetNOx 18FF323D 8 24 *0.05 -200 Adblue/10 18FF3300 48 63 \10 InletNox 18FF393D 8 24 *0.05 -200 file.txt 0CF00300";
    // Example command for a console that asks for the 1-column table with Torque (ID is 0CF00300) from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt:
    //"SPNParser ss_2_00000034.dat.002 1 Torque 0CF00300 40 48 +0 file.txt 0CF00300"

    return 0;
}