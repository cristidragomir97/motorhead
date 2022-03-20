
import constants
import lib 


if __name__ == "__main__":

    pins = {
        'right_a': 4, 
        'right_b': 5,
        'right_pwm': 3,
        'right_enc_a': 10,
        'right_enc_b': 11,
        'left_a': 7,
        'left_b': 8,
        'left_pwm': 6,
        'left_enc_a': 12,
        'left_enc_b': 13
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

            
            mh.set_both(CW, 200)
            time.sleep(2)
            
            mh.set_right(CCW, 128)
            time.sleep(1)

            mh.stop_both()
            time.sleep(5)
        except OSError:
            print("sht hppns")
            pass