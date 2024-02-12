class eFrame: #ethernet frame, send, receive

    # "Periodic message" message ID.
    MSG_ID_PERIODIC_MSG = 0
    # "Echo command response" message ID.
    MSG_ID_ECHO_RESPONSE_MSG = 2

    #Evaluation app "Periodic message period set" message ID.
    MSG_ID_PERIODIC_MSG_PERIOD_SET_MSG = 128

    # "Echo command" message ID.
    MSG_ID_ECHO_COMMAND_MSG = 131

    # "uttec command" message ID.
    MSG_ID_UTTEC_COMMAND_MSG = 133

    # "uttec command" message ID.
    MSG_ID_TEST_MSG = 200

    # List of all supported message ID by this script.
    MSG_SUPPORTED_LIST = [MSG_ID_PERIODIC_MSG_PERIOD_SET_MSG,
                        MSG_ID_ECHO_RESPONSE_MSG,
                        MSG_ID_UTTEC_COMMAND_MSG,
                        MSG_ID_PERIODIC_MSG,
                        MSG_ID_TEST_MSG]

    # Unknown or unsupported message ID value.
    MSG_ID_INVALID_UNSUPPORTED_MSG = -1

    # "Periodic message" expected byte length.
    EXPECTED_LENGTH_PERIODIC_MSG = 17
    # "Periodic message" expected byte length.
    EXPECTED_LENGTH_UTTEC_MSG = EXPECTED_LENGTH_PERIODIC_MSG


    STANDARD_PERRIOD = 10000

    SENSOR_LEVEL = 600
    BATTERY_ALARM_LEVEL = 800

    BIKE_LOCK = 2
    BIKE_UNLOCK =3

    ALL_NODE = 0xffffffff

    # port = "/dev/ttyACM0"
    port = "/dev/ttyUSB0"

class Ctr:
    cmd = eFrame.MSG_ID_UTTEC_COMMAND_MSG
    period = eFrame.STANDARD_PERRIOD
    area_id = 0                     #1
    rack_id = 0                     #2    
    rack_cmd = 0                     #3
    relay = 0                        #4
    sensor_0 = 0                    #5    
    sensor_1 = 0                    #6
    rssi = 0                        #7
    count = 0                       #8
    dst = eFrame.ALL_NODE    
    wpCtrFlag = False

class Status:
    msg_id = eFrame.MSG_ID_UTTEC_COMMAND_MSG
    timestamp = ''
    gw_id = ''
    sink_id = ''
    node_id = 0
    
    area = 0        #1    
    myId = 0        #2
    status =  0     #3
    temperature = 0 #4
    sensor_0 = 0    #5
    sensor_1 = 0    #6
    rssi = 0        #7
    count = 0       #8
    firstFlag = False 
    wpReceiveFlag = False 
    
    nodeIdList = list()
    remain_37 = list()
    remain_74 = list()
    alarm_3_2 = list()
    alarm_2_9 = list()
    
class Demo:
    categroy = 0        #1    
    myId = 0            #2
    temp =  0          #3
    humid = 0           #4
    press = 0           #5
    bat = 0           #6
    rssi = 0            #7
    count = 0         #8
# status: switch, temp, humidity, battery, pitch, roll, rssi        
        


