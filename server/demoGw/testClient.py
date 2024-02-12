import time 
import socket 


# testHost = '0.0.0.0'
# testHost = '192.168.1.3'
testHost = '192.168.0.200'
testPort = 2000

def main():
    mainCount = 0
    while True:
        print('client: {}'.format(mainCount))
        mainCount += 1
        tx = '{"id":' + str(mainCount)
        tx = tx + ', "status": ' + str(mainCount)
        tx = tx + '}'
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((testHost, testPort))
            client_socket.sendall(tx.encode())
            client_socket.close()

        except:
            print('Fail to send date to server:{}, \
                  port:{}'.format(testHost, testPort))

        time.sleep(1)

if __name__ == '__main__':
    main()

