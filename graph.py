import socket
import sys
from matplotlib import pyplot as plt

#python3 graph.py

HOST = ''
PORT = 8081
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
connection, addr = s.accept()

print("listening on", PORT)
while True:
	data = connection.recv(1024).decode()
	print(data)
	if (data == "Close\n"):
		message = "OK!\n"
		connection.send(message.encode())
		break
	if (data == "Graph\n"):
		
		message = "OK!\n"
		connection.send(message.encode())

s.close() 