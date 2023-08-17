import asyncio
import websockets
import socket
import json
import struct

PROXY_PORT = 8080
TCP_HOST = 'localhost'  # Replace with the actual TCP server IP or hostname
TCP_PORT = 12345  # Replace with the actual TCP server port

async def handle_proxy_client(websocket, path):
    try:
        tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tcp_client.connect((TCP_HOST, TCP_PORT))
        print('Connected to TCP server')

        async for message in websocket:
            print('Received from frontend:', message)

            # Parse JSON data received from the frontend
            data = json.loads(message)
            speed = data.get('speed', 0)
            turnRate = data.get('turnRate', 0)

            # Pack speed and turnRate into bytes (using 'b' format for signed char)
            packet = struct.pack('bb', speed, turnRate)
            tcp_client.sendall(packet)

    except Exception as e:
        print('Error:', e)

    finally:
        print('Proxy connection closed')
        tcp_client.close()

async def main():
    server = await websockets.serve(handle_proxy_client, 'localhost', PROXY_PORT)
    print(f'WebSocket proxy server is listening on port {PROXY_PORT}')

    await server.wait_closed()

if __name__ == '__main__':
    asyncio.run(main())