import socket
import threading

# Configuración del servidor
TCP_HOST = 'localhost'  # Dirección IP del servidor
TCP_PORT = 1234      # Puerto del servidor

def start_server():
    # Crear un socket TCP/IP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((TCP_HOST, TCP_PORT))
    server_socket.listen(1)

    print('Servidor TCP escuchando en {}:{}'.format(TCP_HOST, TCP_PORT))

    while True:
        # Esperar una nueva conexión
        client_socket, address = server_socket.accept()
        print('Cliente {} conectado'.format(address))

        # Recibir datos del cliente
        data = client_socket.recv(1024).decode()
        print('Cient: {}'.format(data))

        while True:
            user_input = input('> ')

            if user_input.lower() == 'exit':
                break

            client_socket.send(user_input.encode())

        client_socket.close()

start_server()