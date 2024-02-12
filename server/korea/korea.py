from re import S
import time 
import sys 
import tcpClient 
import runServer  

import os 
import db 
from eFrame import *
from tokai_ini import *
from lamp_ini import *

from struct import unpack   # to handle message's payload byte arrays

try:
    from wirepas_mqtt_library import WirepasNetworkInterface
except ModuleNotFoundError:
    print("Please install Wirepas mqtt library wheel: pip install wirepas-mqtt-library==1.0")
    sys.exit(-1)

try:
    import wirepas_mesh_messaging as wmm
except ModuleNotFoundError:
    print("Please install Wirepas mesh messaging wheel: pip install wirepas-mesh-messaging")
    sys.exit(-1)

myClient = tcpClient 
myDb = db.rack()

def saveStatus():
    print('+++++++++++ saveStatus +++++++++++')

def sendToRaspberry():
    print('+++++++++++ sendToRaspberry +++++++++++')

# Application payload endianness.
MSG_PAYLOAD_ENDIANNESS = "little"
# Endpoint where data coming from node running the evaluation application are expected.
UPLINK_PACKET_EVAL_APP_ENDPOINT = 1

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def _bytes_to_str_hex_format(bytes_data):
    """ Returns a string of byte expressed in hexadecimal from a bytearray."""
    return "".join("{:02X} ".format(x) for x in bytes_data)

class RxMsg:
    def __init__(self, payload=None):
        self.raw_data = payload
        self.cmd = eFrame.MSG_ID_PERIODIC_MSG

    def _parse_periodic_message(self):
        print('_parse_periodic_message')
        return True

    def _parse_uttec_message(self):
        print('_parse_uttec_message')
        return True

    def _setFactorFromPayload(self):        
        if Status.myId < max_rack_id:   #set Ble_id to nodeIdList
            Status.area = list(unpack('<h', self.raw_data[1:3])).pop()
            Status.myId = list(unpack('<h', self.raw_data[3:5])).pop()

            Demo.categroy = Status.area
            Demo.myId = Status.myId
            Demo.temp = list(unpack('<h', self.raw_data[5:7])).pop()
            Demo.humid = list(unpack('<h', self.raw_data[7:9])).pop()
            Demo.press = list(unpack('<h', self.raw_data[9:11])).pop()
            Demo.bat = list(unpack('<h', self.raw_data[11:13])).pop()
            Demo.rssi = list(unpack('<h', self.raw_data[13:15])).pop()
            Demo.count = list(unpack('<h', self.raw_data[15:17])).pop()
            
            print("msg_id: ", Status.msg_id)
            print("categroy: ", Status.area)
            print("myId: ", Status.myId)
            print("temp: ", Demo.temp)
            print("humid: ", Demo.humid)
            print("press: ", Demo.press)
            print("bat: ", Demo.bat)
            print("rssi: ", Demo.rssi)
            print("count: ", Demo.count)

            if(Status.nodeIdList[Status.myId] != Status.node_id):
                print('@@@@@@@@@@@@@@@@@@@@@@@@@@@@ first received')
                Status.firstFlag = True
            else:
                Status.firstFlag = False

            Status.nodeIdList[Status.myId] = Status.node_id
            Status.wpReceiveFlag = True #
            print('set BleID lamp: {}, BleID: {}'.format(Status.myId, Status.nodeIdList[Status.myId]))
            if(self.cmd == eFrame.MSG_ID_PERIODIC_MSG):
                tx = '{"id":' + str(Status.myId)
                tx = tx + ',"temp":' + str(Demo.temp) 
                tx = tx + ',"humid":' + str(Demo.humid) 
                tx = tx + ',"press":' + str(Demo.press) 
                tx = tx + ',"bat":' + str(Demo.bat) 
                tx = tx + ',"rssi":' + str(Demo.rssi) 
                tx = tx + '}\r\n'
                print(tx)
                myClient.sendToTest(tx)
            elif(self.cmd == eFrame.MSG_ID_UTTEC_COMMAND_MSG):
                tx = '{"id":' + str(Status.myId)
                tx = tx + ',"status":' + str(Status.status) 
                tx = tx + '}\r\n'
                print(tx)
                # myClient.sendToTest(tx)
        else:
            print('++++++++++++++++++++++++ rack over flow: {} or area id: {}'.format(Status.myId, Status.test1))
            
    def _get_msg_id(self):
        message_id = None
        result = True
        if len(self.raw_data) > 0:
            # Get message ID field from payload (little-endian bytes order and
            # one unsigned byte to parse.
            message_id = unpack('<B', self.raw_data[0:1])
            # convert to int type
            message_id = int.from_bytes(message_id, MSG_PAYLOAD_ENDIANNESS)
            if message_id not in eFrame.MSG_SUPPORTED_LIST:
                print('not in eFrame.MSG_SUPPORTED_LIST:')
                # Unknown or unsupported message ID received so return an invalid value.
                message_id = eFrame.MSG_ID_INVALID_UNSUPPORTED_MSG
                result = False
        else:
            message_id = eFrame.MSG_ID_INVALID_UNSUPPORTED_MSG
            result = False
            print('eFrame.MSG_ID_INVALID_UNSUPPORTED_MSG')

        Status.msg_id = message_id
        return result

    def parse(self):
        parse_status = False
        print('----------- parse payload -----------')
        if (type(self.raw_data) is bytes and self._get_msg_id()):
            if Status.msg_id == eFrame.MSG_ID_PERIODIC_MSG:
                self.cmd = eFrame.MSG_ID_PERIODIC_MSG
                if len(self.raw_data) == eFrame.EXPECTED_LENGTH_PERIODIC_MSG:
                    parse_status = self._parse_periodic_message()
                    print('----- MSG_ID_PERIODIC_MSG -----')
                else:
                    print('----- Fail, MSG_ID_PERIODIC_MSG -----: %d', len(self.raw_data))


            elif Status.msg_id == eFrame.MSG_ID_UTTEC_COMMAND_MSG:
                self.cmd = eFrame.MSG_ID_UTTEC_COMMAND_MSG
                if len(self.raw_data) == eFrame.EXPECTED_LENGTH_UTTEC_MSG:
                    parse_status = self._parse_uttec_message()
                    print('***** MSG_ID_UTTEC_COMMAND_MSG *****')
                else:
                    print('----- Fail, MSG_ID_UTTEC_COMMAND_MSG -----')

            elif Status.msg_id == eFrame.MSG_ID_TEST_MSG:
                if len(self.raw_data) == eFrame.EXPECTED_LENGTH_UTTEC_MSG:
                    parse_status = self._parse_uttec_message()
                    print('***** MSG_ID_TEST_MSG *****')
                else:
                    print('----- Fail, MSG_ID_TEST_MSG:len(self.raw_data)->{},EXPECTED_LENGTH_UTTEC_MSG->{}  -----'.format(len(self.raw_data), eFrame.EXPECTED_LENGTH_UTTEC_MSG))
            else:
                print('invalid or unsupported message ID received.: %d', Status.msg_id)
                # invalid or unsupported message ID received.
                pass
        else:
            print('Invalid payload type: return parsing failed.')
            # Invalid payload type: return parsing failed.
            pass
        return parse_status

    def display(self, gateway_id, sink_id, node_id, timestamp):
        Status.timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(timestamp))

        Status.gw_id = gateway_id
        Status.sink_id = sink_id
        Status.node_id = node_id
        
        if Status.msg_id == eFrame.MSG_ID_PERIODIC_MSG:
            self._setFactorFromPayload()
        elif Status.msg_id == eFrame.MSG_ID_UTTEC_COMMAND_MSG:
            self._setFactorFromPayload()
        elif Status.msg_id == eFrame.MSG_ID_TEST_MSG:
            self._setFactorFromPayload()
        else:
            print("<Unknown or unsupported message>")
        print('')  # separate each new message display by one empty line

def on_message_event_data_received_callback(data):
    msg = RxMsg(data.data_payload)
    
    if msg.parse():
        msg.display(data.gw_id, data.sink_id, data.source_address, (data.rx_time_ms_epoch/1000))
    else:
        print("Invalid message received !\n")

def callbackGw(gw_error_code, param):
    print('callbackGw: ', param)
    if gw_error_code != wmm.GatewayResultCode.GW_RES_OK:
        print("Message sending failed: res=%s. Caller param is %s" % (gw_error_code, param))

class Worker:

    def __init__(self):
        print('start Worker thread')
        self.wni = WirepasNetworkInterface('localhost', 1883, None, None, True)
        self.wni.register_data_cb(on_message_event_data_received_callback,
                                                    src_ep=UPLINK_PACKET_EVAL_APP_ENDPOINT,
                                                    dst_ep=UPLINK_PACKET_EVAL_APP_ENDPOINT
                                                    )
        super().__init__()

    def sendCtrToNode(self):
        print('+++++++++ sendCtrToNode: Node:{}, Ctr:{}'.format(Ctr.rack_id, Ctr.rack_cmd))
        try:
            Ctr.dst = Status.nodeIdList[Ctr.rack_id]
            if Ctr.dst != 0:
                payload = (Ctr.cmd.to_bytes(1, MSG_PAYLOAD_ENDIANNESS) + 
                        Ctr.area_id.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +    #1
                        Ctr.rack_id.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +   #2
                        Ctr.rack_cmd.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +    #3
                        Ctr.relay.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +   #4
                        Ctr.sensor_0.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +    #5
                        Ctr.sensor_1.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +   #6
                        Ctr.rssi.to_bytes(2, MSG_PAYLOAD_ENDIANNESS) +    #7
                        Ctr.count.to_bytes(2, MSG_PAYLOAD_ENDIANNESS))    #8
                res = self.wni.send_message('uttec',
                                            'sink1',
                                            Ctr.dst,
                                            UPLINK_PACKET_EVAL_APP_ENDPOINT,
                                            UPLINK_PACKET_EVAL_APP_ENDPOINT,
                                            payload,
                                            cb=callbackGw)
                print('=========== Ctr.dst:{}'.format(Ctr.dst))
            else:
                print('################ error destination ###################')

        except Exception as e:
            print(str(e))

import RPi.GPIO as GPIO #2022.12.24

def main():

    myWp = Worker()

    for i in range(0, max_rack_id):
        Status.nodeIdList.append(0)
        Status.remain_37.append(0)
        Status.remain_74.append(0)
        Status.alarm_3_2.append(0)
        Status.alarm_2_9.append(0)

    print('------------- zmq test send -- ip: {}, port: {}'.format(server_ip, server_port))

    data = [123, 1234567890123, 77]
    myClient.sendZmq(data)

    myThread = runServer.MyThread()
    myThread.daemon = True
    myThread.start() 

    ctrCount = 0
    statusCount = 0

    while True:
        try:
            if Ctr.wpCtrFlag:   #from rs422 Control command
                print('&&&&& myWp.sendCtrToNode(Ctr.wpCtr)')
                myWp.sendCtrToNode()
                Ctr.wpCtrFlag = False

            if Status.wpReceiveFlag: # from wirepas module, 
                statusCount += 1
                Status.wpReceiveFlag = False
                flag.wpFlag = True

                print('-------- rack:{}, status: {}, node_id: {}'.format(Status.myId, Status.status, Status.nodeIdList[Status.myId]))
                if  Status.msg_id == eFrame.MSG_ID_PERIODIC_MSG:
                    saveStatus()
                    sendToRaspberry()
                elif  Status.msg_id == eFrame.MSG_ID_UTTEC_COMMAND_MSG:   
                    saveStatus()
                else:
                    print('error data -----------------')

        except KeyboardInterrupt:
            print('KeyboardInterrupt.')
            pid = os.getpid()
            os.kill(pid, 2)

if __name__ == "__main__":
    main()
