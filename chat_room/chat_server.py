import socket
import select
from threading import *
import _thread
import sys


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
"""
the first argument AF_INET is the address domain of the socket. This is used when we have an Internet Domain
with any two hosts
The second argument is the type of socket. SOCK_STREAM means that data or characters are read in a continuous flow
"""
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
if len(sys.argv) != 3:
    print("Correct usage: script, IP address, port number")
    exit()
IP_address = str(sys.argv[1])
Port = int(sys.argv[2])
# binds the server to IP address and port. The client must be aware of these parameters
server.bind((IP_address, Port))
# listens for 100 active connections. This number can be increased as per convenience
server.listen(100)
list_of_clients=[]
name_dict = {}

def clientthread(conn, addr):
    conn.send(str.encode("Welcome to this chatroom! Please tell me your name first:"))
    print(f"now there are {len(list_of_clients)} clients connected")
    number_msg_thread = 0
    while True:
        try:
            message = conn.recv(2048).decode()
            if message:
                if number_msg_thread == 0:
                    name_dict[addr] = message
                    message_to_send = f"<Admin> Welcome {message}!"
                    conn.send(str.encode(message_to_send))
                    broadcast(message_to_send,conn)
                else:
                    # print the message and address of the user who just sent the message on the server terminal
                    print("<" + name_dict[addr] + "> " + message)
                    message_to_send = "<" + name_dict[addr] + "> " + message
                    # add any logic you want here
                    broadcast(message_to_send,conn)
                number_msg_thread += 1
            else:
                remove(conn)
        except:
            print("some error")
            continue

def broadcast(message,connection):
    for clients in list_of_clients:
        if clients != connection:
            try:
                clients.send(str.encode(message))
            except:
                clients.close()
                remove(clients)

def remove(connection):
    if connection in list_of_clients:
        list_of_clients.remove(connection)

while True:
    conn, addr = server.accept()
    """
    Accepts a connection request and stores two parameters, conn which is a socket object for that user, and addr which contains
    the IP address of the client that just connected
    """
    # maintain a list of clients for ease of broadcasting a message to all available people in the chatroom
    list_of_clients.append(conn)
    print(addr[0] + " connected")
    # create individual thread for every user that connects
    _thread.start_new_thread(clientthread,(conn,addr))

conn.close()
server.close()
