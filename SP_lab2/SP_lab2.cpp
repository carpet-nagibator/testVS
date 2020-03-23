#include "EvryThng.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <vector>

using namespace std;

#define UNICODE
#define _UNICODE

int mode = 0;
string Errmsg = "";
int Err = 0;

string GetLastErrorStdStr()
{
    if (Err)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            Err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);
        if (bufLen)
        {
            LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
            std::string result(lpMsgStr, lpMsgStr + bufLen);

            LocalFree(lpMsgBuf);

            return result;
        }
    }
    return "";
}
void ShowMenu()
{
    system("CLS");
    const char **MenuNames = new const char*[9];

    MenuNames[0] = "DeleteFile(1)";
    MenuNames[1] = "CopyFile(1,2)";
    MenuNames[2] = "MoveFile(1,2)";
    MenuNames[3] = "MoveFileEx(1,2)";
    MenuNames[4] = "CreateDirectory(1)";
    MenuNames[5] = "RemoveDirectory(1)";
    MenuNames[6] = "SetCurrentDirectory(1)";
    MenuNames[7] = "GetCurrentDirectory";
    MenuNames[8] = "Exit";

    switch (mode)
    {
    case 0:
        for (int i = 0; i < 9; i++)
        {
            printf("%i %s\n", i + 1, MenuNames[i]);
        }
        break;
    case 1:
        printf("Danilov Maxim\nIVT41-16\n");
        break;
    case 2:
        printf("Error %i:\n", Err);
        cout << Errmsg << endl;
        system("pause");
        exit(Err);
        break;
    }
}
void cmd(int ch, char *argv[])
{
    string kek;
    string lul;
    switch (ch)
    {
    case '1':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek;
        if (_taccess(kek.c_str(), 0) == 0)
        {
            printf("Confim delete %s?\n y/n", kek.c_str());
            if (_getch() == 'y')
                if (0 == DeleteFile(kek.c_str()))
                {
                    mode = 2;
                    Err = GetLastError();
                    Errmsg = GetLastErrorStdStr();
                }
        }
        else
        {
            mode = 2;
            Errmsg = "file not found";
        }
        break;
    case '2':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek >> lul;
        if (_taccess(kek.c_str(), 0) == 0)
        {
            bool NoReWrite = true;
            if (_taccess(lul.c_str(), 0) == 0)
            {
                printf("Confim ReWrite %s?\n y/n", lul.c_str());
                if (_getch() == 'y')
                    NoReWrite = false;
            }
            if (0 == CopyFile(kek.c_str(), lul.c_str(), NoReWrite))
            {
                mode = 2;
                Err = GetLastError();
                Errmsg = GetLastErrorStdStr();
            }
        }
        else
        {
            mode = 2;
            Errmsg = "file not found";
        }
        break;
    case '3':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek >> lul;
        if (_taccess(kek.c_str(), 0) == 0)
        {
            if (0 == MoveFile(kek.c_str(), lul.c_str()))
            {
                mode = 2;
                Err = GetLastError();
                Errmsg = GetLastErrorStdStr();

            }
        }
        else
        {
            mode = 2;
            Errmsg = "file not found";
        }
        break;
    case '4':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek >> lul;
        if (_taccess(kek.c_str(), 0) == 0)
        {
            bool ReWrite = false;
            if (_taccess(lul.c_str(), 0) == 0)
            {
                system("CLS");
                printf("Confim ReWrite %s?\n y/n", lul.c_str());
                if (_getch() == 'y')
                    ReWrite = true;
            }
            if (0 == MoveFileEx(kek.c_str(), lul.c_str(), ReWrite))
            {
                mode = 2;
                Err = GetLastError();
                Errmsg = GetLastErrorStdStr();
            }
        }
        else
        {
            mode = 2;
            Errmsg = "file not found";
        }
        break;
    case '5':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek;
        TCHAR Dir_for_Create[MAX_PATH];
        if (0 == GetCurrentDirectory(sizeof(Dir_for_Create), Dir_for_Create))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
            return;
        }
        strcat_s(Dir_for_Create, "\\");
        strcat_s(Dir_for_Create, kek.c_str());
        if (0 == CreateDirectory(Dir_for_Create, NULL))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
        }
        break;
    case '6':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek;
        TCHAR Dir_for_Remove[MAX_PATH];
        if (0 == GetCurrentDirectory(sizeof(Dir_for_Remove), Dir_for_Remove))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
            return;
        }
        strcat_s(Dir_for_Remove, "\\");
        strcat_s(Dir_for_Remove, kek.c_str());
        if (0 == RemoveDirectory(Dir_for_Remove))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
        }
        break;
    case '7':
        system("CLS");
        cout << "¬ведите параметры:" << endl;
        cin >> kek;
        TCHAR Dir_for_Set[MAX_PATH];
        if (0 == GetCurrentDirectory(sizeof(Dir_for_Set), Dir_for_Set))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
        }
        else
        {
            strcat_s(Dir_for_Set, "\\");
            strcat_s(Dir_for_Set, kek.c_str());
            SetCurrentDirectory(kek.c_str());
        }
        break;
    case '8':
        TCHAR Dir[MAX_PATH];
        if (0 == GetCurrentDirectory(sizeof(Dir), Dir))
        {
            mode = 2;
            Err = GetLastError();
            Errmsg = GetLastErrorStdStr();
        }
        else
        {
            system("CLS");
            printf("CurrentDirectory: %s", Dir);
            while (!_kbhit());
        }
        break;
    case '9':
        exit(0);
        break;
    case '?':
        mode = mode == 1 ? 0 : 1;
        break;
    }
}

void main(int argc, char *argv[])
{
    setlocale(0, "RUS");
    if (argc == 1)
    {
        while (true)
        {
            ShowMenu();
            if (_kbhit())
            {
                cmd(_getch(), argv);
            }
            Sleep(500);
        }
    }
    return;
}