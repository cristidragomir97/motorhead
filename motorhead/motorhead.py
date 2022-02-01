ADDRESS = 0x76

m1_a = 3
m1_b = 4
m1_pwm = 9
m1_enc_a = 7
m1_enc_b = 13

m2_a = 8
m2_b = 6
m2_pwm = 5
m2_enc_a = 10
m2_enc_b = 11


def init_pins(i2c):
    # Initialize pins for motors and encoders
    i2c.write_word_data(ADDRESS, 0x00, bytes(m1_a)
    i2c.write_word_data(ADDRESS, 0x01, bytes(m1_b))
    i2c.write_word_data(ADDRESS, 0x02, bytes(m1_pwm))
    i2c.write_word_data(ADDRESS, 0x03, bytes(m1_enc_a))
    i2c.write_word_data(ADDRESS, 0x04, bytes(m1_enc_b))

    i2c.write_word_data(ADDRESS, 0x05, bytes(m2_a))
    i2c.write_word_data(ADDRESS, 0x06, bytes(m2_b))
    i2c.write_word_data(ADDRESS, 0x07, bytes(m2_pwm))
    i2c.write_word_data(ADDRESS, 0x0A, bytes(m2_enc_a))
    i2c.write_word_data(ADDRESS, 0x0B, bytes(m2_enc_b))


if __name__ == "__main__":
    bus = smbus2.SMBus(1)
    init_pins(i2c)



 
    
 



    
