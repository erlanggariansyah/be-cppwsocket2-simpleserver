#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
	struct sockaddr_in server;
	int server_length, server_port = 8080, buffer_size = 30720;

	WSADATA wsa_data;
	SOCKET server_socket = INVALID_SOCKET, client_socket = INVALID_SOCKET;
	
	if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
		std::cout << "Gagal inisialisasi." << std::endl;
		
		return -1;
	}
	
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET) {
		std::cout << "Gagal membuat socket." << std::endl;
		
		return -1;
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(server_port);
	server_length = sizeof(server);
	
	if (bind(server_socket, (SOCKADDR*)&server, server_length) != 0) {
		std::cout << "Gagal bind socket." << std::endl;
		
		return -1;
	}
	
	if (listen(server_socket, 20) != 0) {
		std::cout << "Gagal listen." << std::endl;
		
		return -1;
	}
	
	std::cout << "Server listen 127.0.0.1:8080" << std::endl;
	
	int bytes = 0;
	while(true) {
		client_socket = accept(server_socket, (SOCKADDR*)&server, &server_length);
		if (client_socket == INVALID_SOCKET) {
			std::cout << "Tidak dapat menerima request" << std::endl;
			
			return -1;
		}
		
		char buff[30720] = { 0 };
		bytes = recv(client_socket, buff, buffer_size, 0);
		if (bytes < 0) {
			std::cout << "Tidak dapat membaca request" << std::endl;
			
			return -1;
		}
		
		std::string response_meta = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
		std::string response = "<html><h1>Hello World</h1></html>";
		
		response_meta.append(std::to_string(response.size()));
		response_meta.append("\n\n");
		response_meta.append(response);
		
		int bytes_sent = 0;
		int total_bytes_sent = 0;
		while (total_bytes_sent < response_meta.size()) {
			bytes_sent = send(client_socket, response_meta.c_str(), response_meta.size(), 0);
			if (bytes_sent < 0) {
				std::cout << "Gagal mengirim response." << std::endl;
				
				return -1;
			}
			
			total_bytes_sent += bytes_sent;
		}
		
		std::cout << "Response berhasil dikirim ke klien: " << response_meta <<  std::endl;
		closesocket(client_socket);
	}
	
	closesocket(server_socket);
	WSACleanup();
	
	return 0;
}
