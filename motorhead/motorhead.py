import smbus2
import time 

GO = 1 
STOP = 0
CW = 1 
CCW = 0

PIN_RIGHT_A = 0x00
PIN_RIGHT_B = 0x01
PIN_RIGHT_PWM = 0x02
PIN_RIGHT_ENC_A = 0x03
PIN_RIGHT_ENC_B = 0x04
PIN_LEFT_A = 0x05
PIN_LEFT_B = 0x06
PIN_LEFT_PWM = 0x07
PIN_LEFT_ENC_A = 0x0A
PIN_LEFT_ENC_B = 0x0B

MOTOR_RIGHT_SPEED = 0x10
MOTOR_RIGHT_DIR = 0x11
MOTOR_RIGHT_GO = 0x12

MOTOR_LEFT_SPEED = 0x20
MOTOR_LEFT_DIR = 0x21
MOTOR_LEFT_GO = 0x22

ADDRESS = 0x76

right_a = 3
right_b = 4
right_pwm = 9
right_enc_a = 7
right_enc_b = 13

left_a = 8
left_b = 6
left_pwm = 5
left_enc_a = 10
left_enc_b = 11

def write_word(bus, reg, data):
    bus.write_word_data(ADDRESS, reg, data)

def init_pins(bus, pins):
    write_word(bus, PIN_RIGHT_A, pins['right_a'])
    write_word(bus, PIN_RIGHT_B, right_b)
    write_word(bus, PIN_RIGHT_PWM, right_pwm)
    write_word(bus, PIN_RIGHT_ENC_A, right_enc_a)
    write_word(bus, PIN_RIGHT_ENC_B, right_enc_b)
    
    write_word(bus, PIN_LEFT_A, left_a)
    write_word(bus, PIN_LEFT_B, left_b)
    write_word(bus, PIN_LEFT_PWM, left_pwm)
    write_word(bus, PIN_LEFT_ENC_A, left_enc_a)
    write_word(bus, PIN_LEFT_ENC_B, left_enc_b)

class MotorHead():
    def __init__(self, pins):
        self.bus = smbus2.SMBus(1)
        init_pins(self.bus, pins)

    def set_right(self, direction, speed):
        write_word(self.bus, MOTOR_RIGHT_DIR, direction)
        write_word(self.bus, MOTOR_RIGHT_SPEED, speed)
        write_word(self.bus, MOTOR_RIGHT_GO, 1)

    def set_left(self, direction, speed):
        write_word(self.bus, MOTOR_LEFT_DIR, direction)
        write_word(self.bus, MOTOR_LEFT_SPEED, speed)
        write_word(self.bus, MOTOR_LEFT_GO, GO)
    
    def set_both(self, direction, speed):
        self.set_left(direction, speed)
        self.set_right(direction, speed)

    def stop_right(self):
        write_word(self.bus, MOTOR_RIGHT_GO, STOP)
    
    def stop_left(self):
        write_word(self.bus, MOTOR_LEFT_GO, STOP)

    def stop_both(self):
        self.stop_left()
        self.stop_right()

if __name__ == "__main__":

    pins = {
        'right_a': 3, 
        'right_b': 4,
        'right_pwm': 9,
        'right_enc_a': 7,
        'right_enc_b': 13,
        'left_a': 8,
        'left_b': 6,
        'left_pwm': 5,
        'left_enc_a': 10,
        'left_enc_b': 11
    }

    mh = MotorHead(pins)

    while True:

        try:
            mh.set_both(CW, 255)
            time.sleep(2)

            for i in range(0, 255, 10):
                mh.set_right(CW, i)
                mh.set_left(CCW, i)

                time.sleep(0.2)

            
            mh.set_both(CW, 255)
            time.sleep(2)
            
            mh.set_right(CCW, 128)
            time.sleep(1)

            mh.stop_both()
            time.sleep(5)
        except OSError:
            print("sht hppns")
            pass