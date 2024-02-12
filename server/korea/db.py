firstFlag = True 
rackStatus = list()

from tokai_ini import *

class rack:
    def __init__(self):
        print('create rack db')
        global firstFlag
        max_board = int(max_rack_id / 8)
        
        if firstFlag:
            firstFlag = False
            for i in range(0, max_board):
                board = '00000000'
                rackStatus.append(board)
            # rackStatus[0] = '22222222'    

    def testDb(self):
        rackStatus[3] = '11111111'
        rackStatus[4] = '22222222'
        print('size of rackStatus: {}'.format(len(rackStatus)))
        print('positon3: {}'.format(rackStatus[3]))
        print('positon4: {}'.format(rackStatus[4]))
        self.readStatus(4)

        self.setStatus(1, '5')
        self.setStatus(11, '4')
        self.setStatus(21, '3')
        self.setStatus(31, '6')
        
        for x in range(5):
            print('{} board: {}'.format(x, rackStatus[x]))

    def displayDb(self):
        for i in range(70):
            print('{} rackStatus : {}'.format(i, rackStatus[i]))

    def clearStatus(self):
        for i in range(0, 70):
            board = '00000000'
            rackStatus[i] = board 

    def displayStatusAtRack(self, rack):
        board = rack // 8
        print('board: {}, status: {}'.format(board, rackStatus[board]))

    def readStatusAtRack(self, rack):
        board = rack // 8
        return rackStatus[board]

    def readStatus(self, board):
        """ 
            print('board: {}, rackStatus: {}'.format(board, rackStatus[board]))
        """
        return rackStatus[board]

    def setStatusByRack(self, rack, status):
        if rack <= 0:
            print('------------- rack number error')
            rack = 1
        rack = rack - 1 #for rack list structure 
        board = rack // 8
        position = rack % 8 
        str_list = list(rackStatus[board])
        str_list[position] = status
        rackStatus[board] = "".join(str_list)
        return rackStatus[board]


def main():
    print('db Test program. 2022.06.30')
    myRack = rack() 
    myRack.testDb()
    # myRack.clearStatus()
    myRack.displayDb()
    yourRack = rack()
    yourRack.displayDb() 

if __name__ == "__main__":
    main()

