import zmq_socket
import time 

def main():
    zmq_socket.zmq_server()
    zmqMainCount = 0

    while True:
        print('zmqMainCount:{}'.format(zmqMainCount))
        time.sleep(1)

if __name__ == '__main__':
    main()
