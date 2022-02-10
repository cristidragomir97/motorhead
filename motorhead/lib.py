import smbus2
import time 
import constants

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
