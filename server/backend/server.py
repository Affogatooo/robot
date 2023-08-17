import socket
import threading
import struct

connected_clients = []

def handle_client(client_socket):
    global connected_clients
    while True:
        try:
            data = client_socket.recv(2)  # Receive 2 bytes of data (speed and turnRate)
            if not data:
                break

            # Unpack the received bytes into speed and turnRate (using 'b' format for signed char)
            speed, turnRate = struct.unpack('bb', data)

            print(f"Received from client: speed={speed}, turnRate={turnRate}")

            for client in connected_clients:
                if client != client_socket:
                    try:
                        data_str = f"{speed} {turnRate}\n"
                        client.sendall(data_str.encode())
                    except:
                        client.close()
                        connected_clients.remove(client)
        except ConnectionResetError:
            break

def main():
    host = '192.168.1.67'  # localhost
    port = 12345        # choose any available port

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_socket.bind((host, port))
    server_socket.listen(5)

    print(f"[*] Listening on {host}:{port}")

    try:
        while True:
            client_socket, addr = server_socket.accept()
            print(f"[*] Accepted connection from {addr[0]}:{addr[1]}")

            connected_clients.append(client_socket)  # Add the new client to the list

            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()
    except KeyboardInterrupt:
        print("[*] Server shutting down.")
        server_socket.close()

if __name__ == "__main__":
    main()