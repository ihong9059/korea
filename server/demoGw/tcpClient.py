import socket
import time 

import zmq
from tokai_ini import *
from lamp_ini import * 

import pickle 

# 서버의 주소입니다. hostname 또는 ip address를 사용할 수 있습니다.
HOST = '127.0.0.1' 
PORT = 9999 #for raspberry test       
raspPort = 9911 #for raspberry chart
# testHost = '192.168.1.2'
# testHost = '127.0.0.1'
# testPort = 2222
testHost = lamp_gateway_ip
testPort = lamp_gateway_port

# testHost = '1.234.23.229'
# testPort = 30000

def sendToRaspberryForTest(tx):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))
    client_socket.sendall(tx.encode())
    # 메시지를 수신합니다. 
    # data = client_socket.recv(1024)
    # print('Received', data.decode())
    client_socket.close()

def sendToRaspberryForChart(tx):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, raspPort))
    client_socket.sendall(tx.encode())
    client_socket.close()

def sendToTest(tx):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((testHost, testPort))
    client_socket.sendall(tx.encode())
    client_socket.close()
    # print('send to Raspberry for status report: {}'.format(tx))

def zmq_client(data, host=server_ip, port=server_port):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER, 0)
    socket.connect(f"tcp://{host}:{port}")
    print('send data to server:')
    # print(data)
    # logger.info('send data to server \r\n %s' %data)
    b_data = pickle.dumps(data)
    socket.send(b_data)
    # use poll for timeouts:
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    while True:
        if poller.poll(time_out*1000): 
            #wating for server respond after sending data
            recv_message = socket.recv_string()
            print("received respond %s" % recv_message)
            if recv_message == "OK":
                socket.close()
                context.destroy()
                return True
        return False


def sendZmq(data):
    BleId = format(data[1], '013')
    BleId = 'ID' + BleId 

    remain = format(data[2], '03')

    data = {
        data[0]:[BleId, remain]
    }

    print(data)

    pickle_data = pickle.dumps(data)
    data_size = len(pickle_data)
    rx_data = {
        "stx":stx,
        "custom_code":custom_code,
        "parking_code":parking_code,
        "area_code":area_code,
        "battery_data":data,
        "data_size":data_size,
        "etx":etx
        }

    zmq_client(rx_data, server_ip, server_port)


def main():

    mainCount = 0
    while True:
        msg = '{}, {}\r\n'.format(mainCount%10, mainCount)
        # sendClient(msg)
        print(msg)
        mainCount += 1
        time.sleep(1)

if __name__ == "__main__":
    main()
