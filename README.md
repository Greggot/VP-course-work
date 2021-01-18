# SPN-Parser

### 26.11.2020

Example command for a console that asks for the 6-column table with Torque (ID is 0CF00300), EngineSpeed... from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt: `SPNParser ss_2_00000034.dat.002 6 Torque 0CF00300 40 48 +0 EngineSpeed 0CF00400 24 40 *0.125 Intakeressure 18FEF600 48 56 *2 OutLetNOx 18FF323D 8 24 *0.05 -200 Adblue/10 18FF3300 48 63 \10 InletNox 18FF393D 8 24 *0.05 -200 file.txt 0CF00300`


Example command for a console that asks for the 1-column table with Torque (ID is 0CF00300) from ss_2_00000034.dat.002 file with Torque's ID leading, saves result in a file.txt: `SPNParser ss_2_00000034.dat.002 1 Torque 0CF00300 40 48 +0 file.txt 0CF00300`

### 01.12.2020 - Update Note

- Parser can parse files with user-defined linues: using gui.pyw, user can form a string type using "Format Settings" button. This button will read first line of a log and using listst user can point to the right structures in the line. Thus it's possible parse logs from all kinds of bus-reading software. 

- Creation of an SPN command is within gui now as well. Name, ID, bit interval and operation sequence is easy to set.

- Line format and SPN commands can be applied to a long sequence of files.

- Data-String structure is formed in a unique way: ID-DATA-TIME, all lines which data length is smaller then 8 is filled from the end to the 8th position with zeros.
