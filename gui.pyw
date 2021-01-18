from tkinter import *
import tkinter as tk
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter import ttk
import os

class LogFormatSettings:
    def __init__(self, sizeX, sizeY, WinName, path):
        self.sizeX = sizeX
        self.sizeY = sizeY
        self.WinName = WinName
        self.path = path
        
        self.AskWin = None
        self.DividionSymbol = None
        self.TimePos = None
        self.DataPos = None
        self.DataLenPos = None
        self.IDPos = None

        self.settings = None 

    def __repr__(self):
        return repr([self.DividionSymbol, self.TimePos, self.IDPos, self.DataLenPos, self.DataPos])
    
    def CreateAskWindow(self):
        self.AskWin = Tk()
        self.AskWin.title(self.WinName)
        self.AskWin.geometry(self.sizeX + 'x' + self.sizeY + '+800+200')
        self.AskWin.resizable(False, False)

        InputFile = open(self.path, 'r')
        SampleString = InputFile.readline()
        SampleList = SampleString.split()

        DataPosBox = ttk.Combobox(self.AskWin, values = SampleList, state = "readonly")
        DataPosBox.grid(column = 0, row = 0, padx = 5, pady = 5)
        DataPosBox.set("-Data Position-")

        DataLenPosBox = ttk.Combobox(self.AskWin, values = SampleList, state = "readonly")
        DataLenPosBox.grid(column = 0, row = 1, padx = 5, pady = 5)
        DataLenPosBox.set("-Data Length Position-")

        DataLenStates = ['Is in a string', 'Always equals to 8']
        DataLenChoosePosBox = ttk.Combobox(self.AskWin, values = DataLenStates, state = "readonly")
        DataLenChoosePosBox.grid(column = 1, row = 1, padx = 5, pady = 5)
        DataLenChoosePosBox.set(DataLenStates[0])

        IDPosBox = ttk.Combobox(self.AskWin, values = SampleList, state = "readonly")
        IDPosBox.grid(column = 0, row = 2, padx = 5, pady = 5)
        IDPosBox.set("-ID Position-")

        TimePosBox = ttk.Combobox(self.AskWin, values = SampleList, state = "readonly")
        TimePosBox.grid(column = 0, row = 3, padx = 5, pady = 5)
        TimePosBox.set("-Time Position-")

        DividionSymbols = ['Spaces', 'Tabs']
        DividionSymbolsBox = ttk.Combobox(self.AskWin, values = DividionSymbols, state = "readonly")
        DividionSymbolsBox.grid(column = 0, row = 4, padx = 5, pady = 5)
        DividionSymbolsBox.set('-Dividion Symbols-')

        def ChangeData(event):
            self.DataPos = SampleList.index(DataPosBox.get())
        def ChangeDataLength(event):
            self.DataLenPos = SampleList.index(DataLenPosBox.get())
        def ChangeID(event):
            self.IDPos = SampleList.index(IDPosBox.get())
        def ChangeTime(event):
            self.TimePos = SampleList.index(TimePosBox.get())
        def ChangeDataLenChoosePos(event):
            if(DataLenChoosePosBox.get() == DataLenStates[1]):
                self.DataLenPos = 255
        def ChangeDividionSymbol(event):
            if(DividionSymbolsBox.get() == DividionSymbols[0]):
                self.DividionSymbol = 0
            else:
                self.DividionSymbol = 1

        DataPosBox.bind("<<ComboboxSelected>>", ChangeData)
        DataLenPosBox.bind("<<ComboboxSelected>>", ChangeDataLength)
        IDPosBox.bind("<<ComboboxSelected>>", ChangeID)
        TimePosBox.bind("<<ComboboxSelected>>", ChangeTime)

        DataLenChoosePosBox.bind("<<ComboboxSelected>>", ChangeDataLenChoosePos)
        DividionSymbolsBox.bind("<<ComboboxSelected>>", ChangeDividionSymbol)

        ContinueButton = Button(self.AskWin, text = 'Continue...')
        ContinueButton.grid(column = 5, row = 1, padx = 20)

        def CloseAndPrint(event):
            self.AskWin.quit()
            self.AskWin.destroy()
            self.settings = str(self.DividionSymbol) + ' ' + str(self.TimePos) + ' ' + str(self.IDPos) + ' ' + str(self.DataLenPos) + ' ' +  str(self.DataPos)
        
        ContinueButton.bind('<Button-1>', CloseAndPrint) # ЛКМ
        self.AskWin.bind('<Return>', CloseAndPrint)
        
        self.AskWin.mainloop()
        return self.settings

class AskWindow:
    def __init__(self, WinName, sizeX, sizeY):
        self.WinName = WinName
        self.sizeX = sizeX
        self.sizeY = sizeY

        self.AskWin = None
        self.NameText = None
        self.IDText = None
        self.Begin = None
        self.End = None
        self.OperationSequence = None

        self.commandText = None

    def __repr__(self):
        return repr(self.commandText)
    
    def getWin(self):
        return self.AskWin
    def getCommandText(self):
        return self.commandText
    def getID(self):
        return self.ID

    def CreateAskWindow(self):
        self.AskWin = Tk()
        self.AskWin.title(self.WinName)
        self.AskWin.geometry(self.sizeX + 'x' + self.sizeY + '+800+200')
        self.AskWin.resizable(False, False)

        Label(self.AskWin, text = 'Name:').grid(column = 0, row = 0, pady = 5)
        Label(self.AskWin, text = 'ID:').grid(column = 1, row = 0, pady = 5)
        Label(self.AskWin, text = 'Begin:').grid(column = 2, row = 0, pady = 5)
        Label(self.AskWin, text = 'End:').grid(column = 3, row = 0, pady = 5)
        Label(self.AskWin, text = 'Operation Sequence:').grid(column = 4, row = 0, pady = 5)
        
        self.NameText = Text(self.AskWin, width = 20, height = 1)
        self.NameText.grid(column = 0, row = 1, padx = 5, pady = 5)

        self.IDText = Text(self.AskWin, width = 10, height = 1)
        self.IDText.grid(column = 1, row = 1, padx = 5, pady = 5)

        self.Begin = Text(self.AskWin, width = 2, height = 1)
        self.Begin.grid(column = 2, row = 1, padx = 5, pady = 5)

        self.End = Text(self.AskWin, width = 2, height = 1)
        self.End.grid(column = 3, row = 1, padx = 5, pady = 5)

        self.Ops = Text(self.AskWin, width = 12, height = 1)
        self.Ops.grid(column = 4, row = 1, padx = 5, pady = 5)

        ContinueButton = Button(self.AskWin, text = 'Continue...')
        ContinueButton.grid(column = 5, row = 1, padx = 20)

        def GetOpEvent(event):
            ID = self.IDText.get(1.0, END)[0:-1]
            Name = self.NameText.get(1.0, END)[0:-1]
            End = str(64 - int(self.Begin.get(1.0, END)[0:-1]))
            Begin = str(64 - int(self.End.get(1.0, END)[0:-1]))
            Ops = self.Ops.get(1.0, END)[0:-1]

            if(End == '64'):
                End = '63'
            if(Begin == '64'):
                Begin == '63'
            
            self.commandText = Name + ' ' + ID + ' ' + Begin + ' ' + End + ' ' + Ops
            self.ID = ID
            self.AskWin.quit()
            self.AskWin.destroy()

        def GetOpEventEnter(event):
            ID = self.IDText.get(1.0, END)[0:-1]
            Name = self.NameText.get(1.0, END)[0:-1]
            End = str(64 - int(self.Begin.get(1.0, END)[0:-1]))
            Begin = str(64 - int(self.End.get(1.0, END)[0:-1]))
            Ops = self.Ops.get(1.0, END)[0:-2]

            if(End == '64'):
                End = '63'
            if(Begin == '64'):
                Begin == '63'
            
            self.commandText = Name + ' ' + ID + ' ' + Begin + ' ' + End + ' ' + Ops
            self.ID = ID
            self.AskWin.quit()
            self.AskWin.destroy()

        ContinueButton.bind('<Button-1>', GetOpEvent) # ЛКМ
        self.AskWin.bind('<Return>', GetOpEventEnter)

MainWin = Tk()
MainWin.title("SPN Parser")                # Название окна
MainWin.geometry('400x500+200+100')    # Размеры окна и расстояние от краёв экрана
MainWin.resizable(True, True)        # Запрет на изменение размеров окна

Command = 'SPNParser '
OperationsLabels = []
Operations = []
IDs = []

IDBox = ttk.Combobox(MainWin, values = None, state = "readonly")
IDBox.grid(column = 2, row = 0, padx = 27)
IDBox.set("-Select leading ID-")

Label(MainWin, text = 'Operation Sequence:').grid(column = 2, row = 4, pady = 5)
        
NameText = Text(MainWin, width = 15, height = 1)
NameText.grid(column = 2, row = 5, padx = 5, pady = 5)

def CreateWin():
    Ask = AskWindow('SPN Settings', '600', '80')
    Operations.append(Ask)
    Ask.CreateAskWindow()
    
    Ask.getWin().mainloop()
    
    commandLabel = Label (MainWin, text = Ask.getCommandText())
    OperationsLabels.append(commandLabel)
    commandLabel.grid()
    
    IDs.append(Ask.getID())

    temp = IDs
    IDBox.set("-Select leading ID-")
    IDBox['values'] = temp
    
def DoCommands():
    InputFilePath = askopenfilename(filetypes=[("Log File", "*.log"), ("Txt File", "*.txt"), ("002 File", "*.002")])
    InputFilePath = InputFilePath + ' '
    
    temp = ''
    for ops in Operations:
        if ops.getCommandText() != None:
            temp = temp + ops.getCommandText() + ' '
    
    if(InputFilePath != ' '):
        OutputFilePath = asksaveasfilename(filetypes=[("Txt File", "*.txt")])
        OutputFilePath = OutputFilePath + '.txt '
        if(OutputFilePath != '.txt ' and IDBox.get() != "-Select leading ID-" and NameText.get(1.0, END)[0:-1] != ''):
                print(Command + InputFilePath + str(len(Operations)) + ' ' + temp + NameText.get(1.0, END)[0:-1] + ' ' + OutputFilePath + IDBox.get())
                os.system(Command + InputFilePath + str(len(Operations)) + ' ' + temp + NameText.get(1.0, END)[0:-1] + ' ' + OutputFilePath + IDBox.get())

def Clear():
    for widget in OperationsLabels:
        widget.grid_forget()
    Operations.clear()
    IDs.clear()
    IDBox['values'] = IDs
    IDBox.set("-Select leading ID-")

def CreateSettingsWin():
    InputFilePath = askopenfilename(filetypes=[("Log File", "*.log"), ("Txt File", "*.txt"), ("002 File", "*.002")])
    if(InputFilePath != ''):
        Ask = LogFormatSettings('400', '300', 'Log Format Settings', InputFilePath)
        settings = Ask.CreateAskWindow()
        NameText.delete(1.0, END)
        NameText.insert(1.0, settings)
    
Button(MainWin, text = 'Format Settings', command = CreateSettingsWin).grid(column = 0, row = 0, pady = 5, padx = 5)        
Button(MainWin, text = 'Create SPN-command', command = CreateWin).grid(column = 0, row = 1, pady = 5, padx = 5)
Button(MainWin, text = ' Clear SPN-command ', command = Clear).grid(column = 0, row = 2, pady = 0, padx = 5)
Button(MainWin, text = '   Do SPN-command   ', command = DoCommands).grid(column = 0, row = 3, pady = 5, padx = 5)
Label(MainWin, text = 'Commands: ').grid(column = 0, row = 5, padx = 5, pady = 5)

MainWin.mainloop()
