import serial 
import threading
import db 
import logger
from eFrame import * 
from tokai_ini import *
# import wirepas 

#  https://periar.tistory.com/230
# tx port: 27, rx port: 28 (at raspberry)
# ttyUSB0
# myDb = db.rack()
rsDb = db.rack()
# myWp = wirepas.Worker()

class RS422(threading.Thread):
    def __init__(self):
        print('start ttyUSB0 rs422 class')

        self.ser = serial.Serial("/dev/ttyUSB0", 9600, timeout = 1)
        # self.ser = serial.Serial("com8", 9600, timeout = 1)
        self.rxFlag = False
        self.rackCtr = [0,0]
        self.testCount = 0
        super().__init__()

    def isRxFlag(self):
        return self.rxFlag

    def getCtr(self):
        return self.rackCtr

    def clearRxFlag(self):
        self.rxFlag = False    
    
    def send(self, tx):
        self.ser.write(tx.encode())
        self.ser.write(b'\r\n')

    def parseRack(self, inData):
        node = cmd = board = rack = 0

        if len(inData) == 16:
            self.rxFlag = True

            org = inData.split()
            cmd = org[0]
            board = org[1]
            rack = org[2]
            logger.printLog('-->' + inData)
            board = int(board) - 1 #rack id = (board - 1)*8 + (rack.find('1') + 1)

            self.testCount += 1
            if (self.testCount % 80) == 0:
                print('cmd: {}, board: {}, rack: {}'.format(cmd, board, rack))
            # self.send(myDb.getRackGroup(0));

            # node = int(board - 1) * 8
            status = rsDb.readStatus(board)
            self.send(status)
            logger.printLog('<--' + status)
            
            node = board * 8
            if rack == '11111111':
                node = all_cmd
                print('--------- all close, address', node) 
                self.rackCtr = [node, 3] #all close cmd
                Ctr.wpCtrFlag = True 
                Ctr.wpCtr = self.rackCtr
                return
            elif rack == '22222222':
                node = all_cmd
                print('--------- all open, address', node) 
                self.rackCtr = [node, 4] #all open cmd
                Ctr.wpCtrFlag = True 
                Ctr.wpCtr = self.rackCtr
                return
            elif rack == '00000000':
                # print('all zero')
                self.rackCtr = [node, 0]
                return
            else:
                print('individual action')

            # myWp.testWorker()
            index = rack.find('1')
            if index > -1:
                node += index + 1 #from 1, 2022.12.24
                print('close: {}'.format(node))
                self.rackCtr = [node, 1]
                Ctr.wpCtrFlag = True 
                Ctr.wpCtr = self.rackCtr

            index = rack.find('2')
            if index > -1:
                node += index + 1 #from 1, 2022.12.24
                print('open: {}'.format(node))
                self.rackCtr = [node, 2]
                Ctr.wpCtrFlag = True 
                Ctr.wpCtr = self.rackCtr

        else:
            print('++++++++++++++++++ error in length')



    def binder(self):
        count = 0
        quit = False
        inData = ''
        try:
            while not quit:
                try:
                    data = self.ser.read()
                    readData = data.decode()

                    if data == b'\r':
                        self.parseRack(inData)
                        inData = ''
                    else:
                        inData = inData + readData

                except:
                    print('error')

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

