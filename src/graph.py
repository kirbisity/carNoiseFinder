import socket
import sys
from matplotlib import pyplot as plt

#python3 graph.py

"""
recv message and sends ok
@returns: message: the message received from peer
"""
def recv_message():
	data = connection.recv(1024).decode()
	message = "OK!\n"
	connection.send(message.encode())
	return data

"""
@returns: ans: the graph [[name], [int, float]...] in dots
"""
def recv_one_graph():
	ans = []
	data = recv_message()
	while data != "EndGraph\n":
		data.strip("\n")
		datatup = data.split()
		ans.append(datatup)
		data = recv_message()
	return ans

def draw_graphs(graphs):
	names = []
	speeds = []
	noises = []
	for graph in graphs:
		speed = []
		noise = []
		for data in graph[1:]:
			speed.append(round(float(data[0])))
			noise.append(round(float(data[1])))
		names.append(" ".join(graph[0]))
		speeds.append(speed)
		noises.append(noise)

	args = []
	for i in range(len(names)):
		args.append(speeds[i])
		args.append(noises[i])
		args.append('-')
	plt.plot(*args)
	plt.axis([0, 250, 0, 130])
	plt.xlabel('speed (km/h)')
	plt.ylabel('noise (dB)')
	plt.legend(tuple(names), loc='upper right')
	plt.title('Noise/speed graph')
	plt.show()





HOST = ''
PORT = 8081
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
connection, addr = s.accept()

print("listening on", PORT)
while True:
	data = recv_message()
	print(data)
	if (data == "Close\n"):
		break
	if (data == "DrawGraph\n"):
		data = recv_message()
		graphs = []
		while data != "End\n":
			graph = recv_one_graph()
			graphs.append(graph)
			data = recv_message()
		draw_graphs(graphs)


s.close() 