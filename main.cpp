#include "Parser.h"

uint8_t nulloperator [1] = {0};
double nulloperand [1] = {0.f};

int main(int argc, char *argv[])    // Format: PROGRAM_NAME PATH_TO_FILE {NAME-ID-BEGIN-END-SIGN_OPERAND-SIGN_OPERAND...} {NAME-ID-BEGIN-END-SIGN_OPERAND...} ... SAVE_PATH LEADING_ID
{                                   // ATTENTION! All operations are executed not by algebra rules - by console given order

    char DivSymb = ' ';
    if (std::stoi(argv[argc - 7], 0, 10) != 0)
        DivSymb = '\t';

    uint32_t FileLen = 0;
    dataString* Data = Parser::ReadDataString(argv[1], DivSymb, std::stoi(argv[argc - 6], 0, 10), std::stoi(argv[argc - 5], 0, 10), std::stoi(argv[argc - 4], 0, 10), 
    std::stoi(argv[argc - 3], 0, 10), FileLen);
    
    Parser::dataStringOut(Data[0]);
    std::cout << "...\n";
    Parser::dataStringOut(Data[FileLen - 1]);
    
    system("pause");                                                                // Delete object

    // Example command for a console that asks for the 6-column table with Torque (ID is 0CF00300), EngineSpeed... from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt:
    //"SPNParser ss_2_00000034.dat.002 6 Torque 0CF00300 40 48 +0 EngineSpeed 0CF00400 24 40 *0.125 Intakeressure 18FEF600 48 56 *2 OutLetNOx 18FF323D 8 24 *0.05 -200 Adblue/10 18FF3300 48 63 \10 InletNox 18FF393D 8 24 *0.05 -200 file.txt 0CF00300";
    // Example command for a console that asks for the 1-column table with Torque (ID is 0CF00300) from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt:
    //"SPNParser ss_2_00000034.dat.002 1 Torque 0CF00300 40 48 +0 file.txt 0CF00300"

    return 0;
}