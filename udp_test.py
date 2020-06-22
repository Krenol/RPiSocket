import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(str.encode('hello'), ('127.0.0.1', 8889))