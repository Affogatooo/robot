import socket
import threading
import struct

def receive_data(client_socket):
    while True:
        try:
            data = client_socket.recv(2)  # Receive 2 bytes of data (speed and turnRate)
            if not data:
                break

            # Unpack the received bytes into speed and turnRate (using 'b' format for signed char)
            speed, turnRate = struct.unpack('bb', data)

            print(f"[{speed},{turnRate}]")
        except ConnectionResetError:
            break

def main():
    host = '127.0.0.1'  # localhost
    port = 12345       # same port used in the server

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((host, port))
        print(f"[*] Connected to {host}:{port}")

        receive_thread = threading.Thread(target=receive_data, args=(client_socket,))
        receive_thread.start()

        while True:
            # Read speed and turnRate from user as integers (values between -127 and 127)
            speed = int(input("Enter speed (-127 to 127): "))
            turnRate = int(input("Enter turn rate (-127 to 127): "))

            # Pack the speed and turnRate as bytes (using 'b' format for signed char)
            data = struct.pack('bb', speed, turnRate)
            client_socket.sendall(data)

    except ConnectionRefusedError:
        print("[!] Connection to the server was refused.")
    except KeyboardInterrupt:
        pass
    finally:
        print("[*] Closing the client.")
        client_socket.close()

if __name__ == "__main__":
    main()