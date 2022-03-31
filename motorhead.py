import rospy, smbus2 logging,time 
from core.utils import *

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

def write_word(bus, reg, data):
    bus.write_word_data(ADDRESS, reg, data)

def init_pins(bus, pins):
    write_word(bus, PIN_RIGHT_A, pins['right_a'])
    write_word(bus, PIN_RIGHT_B, pins['right_b'])
    write_word(bus, PIN_RIGHT_PWM, pins['right_pwm'])
    write_word(bus, PIN_RIGHT_ENC_A, pins['right_enc_a'])
    write_word(bus, PIN_RIGHT_ENC_B, pins['right_enc_b'])
    
    write_word(bus, PIN_LEFT_A, pins['left_a'])
    write_word(bus, PIN_LEFT_B, pins['left_b'])
    write_word(bus, PIN_LEFT_PWM, pins['left_pwm'])
    write_word(bus, PIN_LEFT_ENC_A, pins['left_enc_a'])
    write_word(bus, PIN_LEFT_ENC_B, pins['left_enc_b'])

class motorhead():
     
    def __init__(self, radius, flip):
        self.radius = radius
        self.flip = flip
        
        try:
            self.bus = smbus2.SMBus(1)
            init_pins(self.bus, pins)
   
            logg(__name__, "INFO", "Motors successfullly initialised")
        except Exception as e:
            logg(__name__, "ERROR", "Exception initialisitng Motorhead Motor Driver {e}")


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

    def update(self, msg):
        sep = 0.295
        angular = float(msg.angular.z)
        linear = float(msg.linear.x)
        radius = float(self.radius)
        
        right_pwm = (linear / radius) + ((angular * sep) / (2.0 * radius)) * 10
        left_pwm = (linear / radius)  - ((angular * sep) / (2.0 * radius)) * 10

        if right_pwm > 250: right_pwm = 250
        if left_pwm > 250: left_pwm = 250

        set_right(self, CW, right_pwm)
        set_left(self, CW, left_pwm)

        logg(__name__, "DEBUG", "updating motor values [angular: {}] [linear: {}] [right_pwm: {}] [left_pwm: {}]".format(angular, linear, right_pwm, left_pwm))
