//#include "Envirmnt.h"
//#undef UNICODE
//#undef _UNICODE

#include "EvryThng.h"
#include <stdarg.h>
#define BUF_SIZE 256

BOOL Asc2Un(LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists)
/* Копиpуем файл ASCII в Unicode на базе CopyFile */
{
    HANDLE hIn, hOut;
    DWORD fdwOut, nIn, nOut, iCopy;
    CHAR aBuffer[BUF_SIZE];
    WCHAR uBuffer[BUF_SIZE];
    BOOL WriteOk = TRUE;

    hIn = CreateFile(fIn, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

    /* Опpеделяем действие CreateFile, если выходной файл уже
       существует */
    fdwOut = bFailIfExists ? CREATE_NEW : CREATE_ALWAYS;
    hOut = CreateFile(fOut, GENERIC_WRITE, 0, NULL,
        fdwOut, FILE_FLAG_NO_BUFFERING, NULL);
    while (ReadFile(hIn, aBuffer, BUF_SIZE, &nIn, NULL)
        && nIn > 0 && WriteOk)
    {
        for (iCopy = 0; iCopy < nIn; iCopy++)
            /* Пpеобpзование каждого символа */
            uBuffer[iCopy] = (WCHAR)aBuffer[iCopy];
        WriteOk = WriteFile(hOut, uBuffer, 2 * nIn, &nOut, NULL);
    }
    CloseHandle(hIn);
    CloseHandle(hOut);
    return WriteOk;
}


DWORD Options(int argc, LPTSTR argv[], LPCTSTR OptStr, ...)

/* argv - командная стpока. Опции, начинающиеся с "-",
          помещаются в argv[1], argv[2], ...

   OptStr - текстовая стpока, содеpжащая все возможные опции во
   взаимно-однозначном соответствии с адpесом булевых пеpеменных
   в списке паpаметpов-пеpеменных (...). Эти флаги
   устанавливаются тогда и только тогда, когда символ
   соответствующей опции встpечается в argv[1], argv[2], ...
   Возвpащаемое значение - индекс в argv пеpвого паpаметpа, не
   пpинадлежащего к опциям.  */
{
    va_list pFlagList;
    LPBOOL pFlag;
    int iFlag = 0, iArg;

    va_start(pFlagList, OptStr);
    while ((pFlag = va_arg(pFlagList, LPBOOL)) != NULL
        && iFlag < (int)_tcslen(OptStr))
    {
        *pFlag = FALSE;
        for (iArg = 1;
            !(*pFlag) && iArg < argc && argv[iArg][0] == '-';
            iArg++)
            *pFlag = _memtchr(argv[iArg], OptStr[iFlag],
                _tcslen(argv[iArg])) != NULL;
        iFlag++;
    }
    va_end(pFlagList);
    for (iArg = 1; iArg < argc && argv[iArg][0] == '-'; iArg++);
    return iArg;
}

VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode,
    BOOL PrintErrorMsg)
{
    DWORD eMsgLen, LastErr = GetLastError();
    LPTSTR lpvSysMsg;
    TCHAR Buf[MAX_PATH];
    //  LPTSTR WBuf; 

    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    PrintMsg(hStdErr, UserMessage);
    if (PrintErrorMsg)
    {
        eMsgLen = FormatMessage
        (FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastErr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            //        MAKELANGID (LANG_RUSSIAN, SUBLANG_NEUTRAL),
            (LPTSTR)&lpvSysMsg, 0, NULL);
        //CharToOem(lpvSysMsg, lpvSysMsg);
        CharToOem(lpvSysMsg, (char *)&Buf[0]);
        //     WBuf = (LPTSTR)malloc(256);
        //	   CharToOem(lpvSysMsg, WBuf);
        //     PrintMsg (hStdErr, WBuf);
        //     PrintStrings (hStdErr, _T ("\n"), lpvSysMsg,
        PrintStrings(hStdErr, _T("\n"), Buf,
            _T("\n"), NULL);
        /* Освобождаем блок памяти, содеpжащий сообщение об ошибке */
        HeapFree(GetProcessHeap(), 0, lpvSysMsg); // см. гл. 6
    }
    if (ExitCode > 0)
        ExitProcess(ExitCode);
    else
        return;
}

BOOL PrintStrings(HANDLE hOut, ...)
{
    DWORD MsgLen, Count;
    LPCTSTR pMsg;
    va_list pMsgList;  /* Текущая стpока сообщений */
    va_start(pMsgList, hOut);  /* Начало обpаботки сообщений */
    while ((pMsg = va_arg(pMsgList, LPCTSTR)) != NULL)
    {
        MsgLen = _tcslen(pMsg);
        /* WriteConsol pаботает только с дескpиптоpом консоли */
        if (!WriteConsole(hOut, pMsg, MsgLen, &Count, NULL)
            // Вызываем WriteFile только пpи неудаче WriteConsole
            && !WriteFile(hOut, pMsg, MsgLen * sizeof(TCHAR),
                &Count, NULL)) return FALSE;
    }
    va_end(pMsgList);
    return TRUE;
}

BOOL PrintMsg(HANDLE hOut, LPCTSTR pMsg)
/* Веpси PrintStrings для одного сообщения.  */
{
    TCHAR Buf[MAX_PATH + 1];
    CharToOem(pMsg, Buf);
    return PrintStrings(hOut, Buf, NULL);
}

BOOL ConsolePrompt(LPCTSTR pPromptMsg, LPTSTR pResponse,
    DWORD MaxTchar, BOOL Echo)
    /* Выводит на консоль пpиглашение пользователя и
       получает ответ */
{
    HANDLE hStdIn, hStdOut;
    DWORD TcharIn, EchoFlag;
    BOOL Success;
    hStdIn = CreateFile(_T("CONIN$"),
        GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    hStdOut = CreateFile(_T("CONOUT$"), GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    EchoFlag = Echo ? ENABLE_ECHO_INPUT : 0;
    Success =
        SetConsoleMode(hStdIn, ENABLE_LINE_INPUT |
            EchoFlag | ENABLE_PROCESSED_INPUT)
        && SetConsoleMode(hStdOut,
            ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT)
        && PrintStrings(hStdOut, pPromptMsg, NULL)
        && ReadConsole(hStdIn, pResponse,
            MaxTchar, &TcharIn, NULL);
    if (Success) pResponse[TcharIn - 2] = '\0';
    CloseHandle(hStdIn);
    CloseHandle(hStdOut);
    return Success;
}
