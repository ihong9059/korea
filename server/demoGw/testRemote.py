import socket
import time 
from datetime import datetime 

# HOST = '127.0.0.1'  
HOST = '1.234.23.229'  
PORT = 54321       

def sendClient(tx):
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        client_socket.sendall(tx.encode())        
        # client_socket.sendall('Good Mornig'.encode())        
        # data = client_socket.recv(1024)
        # print('Received', repr(data.decode()))
        client_socket.close()
    except:
        print('server not connected: -h{},-p{}'.format(HOST, PORT))
    finally:
        pass
        # print('finally next:')

def main():
    mainCount = 0
    print('now test remote communication. 2023.02.19 12:00')
    while True:
        now = datetime.now()
        msg = '{}, {}'.format(now, mainCount)
        sendClient(msg)
        print(msg)
        mainCount += 1
        time.sleep(30)

if __name__ == "__main__":
    main()
