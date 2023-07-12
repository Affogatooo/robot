import socket
from flask import Flask, render_template, request

# Configuración del servidor
TCP_IP = socket.gethostbyname(socket.gethostname())   # Dirección IP del servidor
TCP_PORT = 1234                                         # Puerto del servidor

# Creamos un socket TCP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Enlazamos el socket al puerto y dirección IP
server_socket.bind((TCP_IP, TCP_PORT))

# Ponemos el socket en modo de escucha
server_socket.listen(1)

print(f"Servidor escuchando en {TCP_IP}:{TCP_PORT}")

app = Flask(__name__)

# Ruta principal para mostrar la página web
@app.route('/')
def index():
    return render_template('index.html')

# Ruta para recibir el mensaje enviado desde la página web
@app.route('/enviar', methods=['POST'])
def enviar():
    # Recibimos los datos enviados desde la página web
    message = request.form['message']

    # Enviamos el mensaje al cliente (ESP32)
    conn.sendall(message.encode('utf-8'))

    return 'Mensaje enviado: ' + message

# Ruta para mostrar los mensajes recibidos
@app.route('/mensajes')
def mensajes():
    mensajes_recibidos = []

    while True:
        # Recibimos los datos enviados por el cliente (ESP32)
        data = conn.recv(1024).decode('utf-8')

        if not data:
            # Si no se reciben datos, el cliente ha cerrado la conexión
            break

        mensajes_recibidos.append(data)

    return render_template('mensajes.html', mensajes=mensajes_recibidos)

# Cerramos la conexión y el socket del servidor al finalizar
@app.teardown_appcontext
def close_socket(exception=None):
    server_socket.close()

if __name__ == '__main__':
    # Esperamos a que llegue una conexión
    conn, addr = server_socket.accept()
    print(f"Conexión establecida desde: {addr}")

    # Ejecutamos la aplicación Flask
    app.run()