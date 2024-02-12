import RPi.GPIO  as  GPIO
import  time
LedPin = 11         # pin11 

def setup():      
    GPIO.setmode(GPIO.BOARD)       # Set the board mode  to numbers pins by physical location
    GPIO.setup(LedPin, GPIO.OUT)   # Set pin mode as output
    GPIO.output(LedPin, GPIO.HIGH) # Set pin to high(+3.3V) to off the led


def main():
    print('now test Gpio at raspberry pi. 2022.12.24 10:00')
    setup()
    mainCount = 0
    while True:
        time.sleep(1)
        GPIO.output(LedPin, GPIO.HIGH)
        
        time.sleep(1)
        GPIO.output(LedPin, GPIO.LOW)
        
        print('mainCount: {}'.format(mainCount))
        mainCount += 1 

if __name__ == "__main__":
    main()

