import threading 
import time 
import socket 
from eFrame import * 
import json 

testCount = 0

class MyThread(threading.Thread):
    def __init__(self):
        self.myCount = 0 
        self.EndThread = False 
        self.host = '0.0.0.0'
        # self.host = '192.168.1.59'
        self.port = 3333        
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen()
        # self.client_socket, self.addr = self.server_socket.accept()
        print('start MyThread host:{}, port:{}'.format(self.host, self.port))

        super().__init__() 

    def endThread(self):
        self.EndThread = True 

    def bind(self):
        countThread = 0
        global testCount 

        # while not self.EndThread:
        while True:
            try:
                if testCount <=10 or testCount >= 20:
                    if testCount == 20:
                        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                        self.server_socket.bind((self.host, self.port))
                        self.server_socket.listen()

                    self.client_socket, self.addr = self.server_socket.accept()
                    data = self.client_socket.recv(1024)
                    if data:
                        print('Received from', self.addr, data.decode())
                        obj = json.loads(data.decode())
                        # Ctr.area_id = 777
                        Ctr.rack_id = int(obj.get("id")) 
                        Ctr.rack_cmd = int(obj.get("status"))
                        # Ctr.temperature = 1
                        # Ctr.sensor_0 = 1
                        # Ctr.sensor_1 = 1
                        # Ctr.rssi = 1
                        # Ctr.count = countThread
                        # print("&&&&&&&&&&&&& from runServer --> area: {}, rack: {}, ctr: {}".format(Ctr.area_id, Ctr.rack_id, Ctr.rack_cmd))
                        # Ctr.wpCtrFlag = True 
                        print('MyThread Count: {}'.format(countThread))
                        countThread += 1
                    self.client_socket.close()
                else:
                    print('now test time: {}'.format(testCount))
                    time.sleep(0.5)
                    self.server_socket.close()
            except:
                print('^^^^^^^^^^^^^^^ end of server try ^^^^^^^^^^^')
                self.EndThread = True 
         
        print('end of bind') 
        self.client_socket.close()
        self.server_socket.close()

    def run(self):
        try:
            th = threading.Thread(target=self.bind)
            th.start()
        except:
            print('----------- Thread except')


def main():
    print('now start testServer.py 2023.06.04 11:30')
    myThread = MyThread()
    myThread.daemon = True
    myThread.start() 
    mainCount = 0
    global testCount 

    while True:
        time.sleep(1)
        mainCount += 1
        testCount = mainCount
        print('main: {}'.format(mainCount))

if __name__ == "__main__":
    main()

