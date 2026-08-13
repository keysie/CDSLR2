import serial
import time
import signal
import math

run_flag = True

def signal_handler(sig, frame):
    sig_enum = signal.Signals(sig)
    print("Program received the signal: ", sig_enum.name)
    global run_flag
    run_flag = False

signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)

def run():
    # 500 rule for sharp images
    FOCAL_LENGTH = 18 #mm
    CROP_FACTOR = 1.5 #1.5 for APS-C; 1 for full-format
    SAFETY_FACTOR = 2.0 #s (float)
    GOOD_EXPOSURE = 500 / (FOCAL_LENGTH * CROP_FACTOR * SAFETY_FACTOR)
    ROUND_EXPOSURE = math.ceil(GOOD_EXPOSURE)

    # allow time for the dslr to write to the sd-card
    WRITE_DELAY = 1 #s (int)
    assert(isinstance(WRITE_DELAY, int))

    # Open serial connection and wait a bit for the arduino to reset
    cdslrr = serial.Serial('/dev/ttyACM0', baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=0.5, xonxoff=0, rtscts=0)

    # Intro
    print('Simple control loop for CDSLR² by Keysie')
    print('')
    print(f'Set to {FOCAL_LENGTH:02}mm at {CROP_FACTOR:1.2} crop with factor of {SAFETY_FACTOR:1.2} as safety.')
    print(f'Recommended exposure for camera: {GOOD_EXPOSURE:1.2}s.' )
    print(f'Setting loop to trigger every ~{ROUND_EXPOSURE+WRITE_DELAY:02}s.')
    print('')
    input('Press Enter to start looping...')

    # Loop
    while run_flag:
        cdslrr.read_all()
        cdslrr.write(b'S1')
        print('TRIGGER')
        for time_remaining in range(ROUND_EXPOSURE+WRITE_DELAY, 0, -1):
            if not run_flag:
                break
            print(f'Next trigger in {time_remaining:02}s    (Ctrl+C to exit)')
            time.sleep(0.5)
            cdslrr.write(b'S0')
            time.sleep(0.5)
    cdslrr.close()


if __name__ == '__main__':
    run()
    print('Exiting...')
