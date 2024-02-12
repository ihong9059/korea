import threading 
import time 
import socket 
from eFrame import * 
import json 

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

        # while not self.EndThread:
        while True:
            try:
                self.client_socket, self.addr = self.server_socket.accept()
                data = self.client_socket.recv(1024)
                if data:
                    print('Received from', self.addr, data.decode())
                    obj = json.loads(data.decode())
                    # Ctr.area_id = 777
                    Ctr.area_id = int(obj.get("area"))
                    Ctr.rack_id = int(obj.get("rack")) 
                    Ctr.rack_cmd = int(obj.get("ctr"))
                    Ctr.relay = int(obj.get("relay"))
                    Ctr.sensor_0 = 1
                    Ctr.sensor_1 = 1
                    Ctr.rssi = 1
                    Ctr.count = countThread
                    print("&&&&&&&&&&&&& from runServer --> area: {}, rack: {}, ctr: {}, relay: {}".format(
                        Ctr.area_id, Ctr.rack_id, Ctr.rack_cmd, Ctr.relay))
                    Ctr.wpCtrFlag = True 
                    print('MyThread Count: {}'.format(countThread))
                    countThread += 1
                self.client_socket.close()
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


