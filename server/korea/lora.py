import threading 
import time 
import socket 
from eFrame import * 
import json 
import serial 

testHost = '127.0.0.1'
testPort = 20000

class RS422(threading.Thread):
    def __init__(self):
        print('start ttyUSB0 rs422 class')

        self.ser = serial.Serial("com5", 115200, timeout = 1)
        super().__init__()

    def isRxFlag(self):
        return self.rxFlag

    def getCtr(self):
        return self.rackCtr

    def clearRxFlag(self):
        self.rxFlag = False    
    
    def send(self, tx):
        self.ser.write(tx.encode())
        # self.ser.write(b'\r')

    def binder(self):
        count = 0
        quit = False
        inData = ''
        try:
            while not quit:

                data = self.ser.readline()
                if(data):
                    print(data)
                    try:
                        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                        client_socket.connect((testHost, testPort))
                        client_socket.sendall(data)
                        client_socket.close()
                    except:
                        print('Fail to send date to server:{}, \
                            port:{}'.format(testHost, testPort))

        except Exception as e:
            print("---------- binder except : ")
            print(str(e))
        finally:
            print('finally passed binder')
            pass 

    def run(self):
        try:
            th = threading.Thread(target=self.binder)
            th.start()
        except:
            print('++++ run except :')
        finally:
            print('finally passed run')
            pass 

