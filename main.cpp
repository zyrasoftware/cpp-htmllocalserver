
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <string>


using namespace std;

int main() {
	cout << "Bu senin localhost oluþturmaný saðlar";
	setlocale(LC_ALL, "turkish");
	SOCKET wsocket;
	SOCKET new_wsocket;
	WSADATA wsaData;
	struct sockaddr_in server;
	int server_len;
	int BUFFER_SIZE = 30720;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << " Baþlatýlamadý.";
	}


	wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (wsocket == INVALID_SOCKET) {
		cout << "Soket oluþturulamadý.";
	}


	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8000);
	server_len = sizeof(server);


	if (bind(wsocket, (SOCKADDR*)&server, server_len) != 0) {
		cout << "Soket baðlanamadý";
	}

	if (listen(wsocket, 20) !=0 ) {
		cout << " Sunucu Dinlenemiyor";
	}

	cout << " 127.0.0.1:8000 Dinleniyor";
	int bytes = 0;
	while (true) {
		new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
		if (new_wsocket == INVALID_SOCKET) {
			cout << "Kabul Edilemedi";
		}


		char buff[30720] = { 0 };
		bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
		if (bytes < 0) {
			cout << "Ýstemci isteði okunamadý";
		}

		string serverMessage = "HTTP/1.1 200 OK\n Content-Type: text/html \nContent-Length: ";
		string response = "<html><h1> Sunucu</h1><html>";
		serverMessage.append(to_string(response.size()));
		serverMessage.append("\n\n");
		serverMessage.append(response);

		int bytesSent = 0;
		int totalBytesSent = 0;

		while (totalBytesSent < serverMessage.size())  {
			bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
			if (bytesSent < 0) {
				cout << "Yanýt gönderilemedi";
		}
		
		
			totalBytesSent += bytesSent;
		}
		cout << " Uygulamaya Yanýt Gönderildi.";
		closesocket(new_wsocket);
	}

	closesocket(wsocket);
	WSACleanup();
	
	return 0;
}