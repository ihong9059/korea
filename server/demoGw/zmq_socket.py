#### program sample created by TOKAIGIKEN seri 
# created 2022-08-18
# need install pyzmq
import zmq
import pickle
## constant
MAX_BYTE = 1028
PARKING_CODE = "0099"
AREA_CODE = "0001"
STX = 0x01
ETX = 0x02
time_out = 5 #s
##########
import logging
logger = logging.getLogger(__name__)
FORMAT = "%(asctime)s, %(levelname)s,%(name)s,%(funcName)s,%(message)s"
format = logging.Formatter(FORMAT)
consolelog = logging.StreamHandler()
consolelog.setFormatter(format)
logger.addHandler(consolelog)
logger.setLevel(logging.DEBUG)
#
def zmq_server(host="*",port=8888):
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://%s:%d"%(host,port))
    logger.info("Server waiting for incoming connection...")
    # wait for incoming data
    while True:
        rx_data  = socket.recv(MAX_BYTE)
        rx_size = len(rx_data)
        dict_data  = pickle.loads(rx_data)
        logger.info("[Received Data]: %s, [Data Size]: %d" % (dict_data,rx_size))
        #check request code and parking code
        stx_data = dict_data['stx']
        parking_code = dict_data['parking_code']
        etx_data = dict_data['etx']
        data_size = dict_data['data_size']
        pickle_data = pickle.dumps(dict_data["battery_data"])
        pickle_data_size = len(pickle_data)
        validation = bool(stx_data == STX and parking_code==PARKING_CODE and etx_data == ETX and pickle_data_size == data_size)
        #if sucess
        if validation:
            socket.send('OK'.encode())
        #either failed
        else:
            socket.send('NG'.encode())
#
def zmq_client(data, host="*", port=8888):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.setsockopt(zmq.LINGER, 0)
    socket.connect(f"tcp://{host}:{port}")
    logger.info('send data to server \r\n %s' %data)
    b_data = pickle.dumps(data)
    socket.send(b_data)
    # use poll for timeouts:
    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)
    while True:
        if poller.poll(time_out*1000): 
            #wating for server respond after sending data
            recv_message = socket.recv_string()
            logger.info("received respond %s" % recv_message)
            if recv_message == "OK":
                socket.close()
                context.destroy()
                return True
        return False

if __name__ == "__main__":
    zmq_server()
    pass