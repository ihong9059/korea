import threading 
import time 
import socket 
from eFrame import * 
import json 
import serial 

whyBizEthernet = '192.168.0.200'
whyBizPort = 2000

raspIp = '192.168.0.4'
raspPort = 7777

serverIp = '0.0.0.0'
serverPort = 10000 


class MyThread(threading.Thread):
    def __init__(self):
        self.myCount = 0 
        self.EndThread = False 
        self.host = '0.0.0.0'
        self.port = serverPort        
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen()
        print('start MyThread host:{}, port:{}'.format(self.host, self.port))

        super().__init__() 

    def endThread(self):
        self.EndThread = True 

    def bind(self):
        countThread = 0

        # while not self.EndThread:
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen()
        testCount = 0        
        while True:
            try:
                client_socket, self.addr = self.server_socket.accept()
                data = client_socket.recv(1024)
                if data:
                    tx = ''
                    print('Received from', self.addr, data.decode())
                    # if (countThread % 2) == 1:
                    #     tx = '{"ca":13,"se":1,"va":1}'
                    # else:
                    #     tx = '{"ca":13,"se":1,"va":0}'
                    print('tx: ', tx)
                    # client_socket.send(tx.encode())
                client_socket.close()

                rasp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                rasp.connect((raspIp, raspPort))
                rasp.sendall(data)
                rasp.close()

                print("countThread: ", countThread)

                countThread += 1

            except Exception as e:
                print(e)
                print('^^^^^^^^^^^^^^^ end of server try ^^^^^^^^^^^')
                self.EndThread = True 
        self.server_socket.close()
         
    def run(self):
        try:
            th = threading.Thread(target=self.bind)
            th.start()
        except:
            print('----------- Thread except')



def main():
    mainCount = 0
    print("Whybiz Ethernet test by uttec. 2023.12.22")

    myThread = MyThread()
    myThread.daemon = True
    myThread.start() 
    print("Receive server start")

    toggle = True 

    # client_socket.sendall(data)
    # client_socket.close() 

    while True:
        # toggle = not toggle
        # if toggle:
        #     tx = '{"ca":13,"se":1,"va":1}'
        # else:   
        #     tx = '{"ca":13,"se":1,"va":0}'

        # client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # client_socket.connect((whyBizEthernet, whyBizPort))
        # print("connected to server: {}, {}".format(whyBizEthernet, whyBizPort) )

        # data = client_socket.recv(1024)
        # print("received: {}".format(data))

        # client_socket.close() 

        time.sleep(1)

if __name__ == '__main__':
    main()

