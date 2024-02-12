import threading 
import time 
import socket 
from eFrame import * 
import json 
from tokai_ini import *

class MyThread(threading.Thread):
    def __init__(self):
        print('testThread start')
        super().__init__() 

    def bind(self):
        countThread = 0
        timeDelay = 3
        while True:
            print('threadCount: {}'.format(countThread))
            Ctr.wpCtrFlag = True 
            # Ctr.wpCtr = [1, all_cmd, 3] #all close cmd
            # time.sleep(timeDelay)
            # Ctr.wpCtrFlag = True 
            # Ctr.wpCtr = [1, all_cmd, 4] #all open cmd
            Ctr.wpCtr = [1, all_cmd, 5] #all open cmd
            time.sleep(timeDelay)

            countThread += 1

    def run(self):
        try:
            th = threading.Thread(target=self.bind)
            th.start()
        except:
            print('----------- Thread except')


