import mraa as m
from evdev import InputDevice, ecodes

device = InputDevice('/dev/input/event2')

# tmp variables for speed, direction and carstate
car_on = False
speed = 0.0
direction = 0.0

# Motor A layout
ain1 = m.Gpio(45)
ain2 = m.Gpio(46)
apwm = m.Pwm(0)

# Motor B layout
bin1 = m.Gpio(47)
bin2 = m.Gpio(48)
bpwm = m.Pwm(14)

# Car standby layout
stby = m.Gpio(15)

# set mode
ain1.dir(m.DIR_OUT)
ain2.dir(m.DIR_OUT)
bin1.dir(m.DIR_OUT)
bin2.dir(m.DIR_OUT)
stby.dir(m.DIR_OUT)

# initialize states
ain1.write(0)
ain2.write(0)
apwm.enable(True)

bin1.write(0)
bin2.write(0)
bpwm.enable(True)

stby.write(0)


def controlSpeed(speed):
    if speed >= -0.04 and speed <= 0.04: # car hold
        ain1.write(0)
        ain2.write(0)
        apwm.write(0)
    elif speed > 0: # car drive forward
        ain1.write(1)
        ain2.write(0)
        apwm.write(speed)
    elif speed < 0: # car drive backward
        ain1.write(0)
        ain2.write(1)
        apwm.write(-speed)

		
def controlDirection(direction):
    if direction >= -0.04 and direction <= 0.04: # car direction straight
        bin1.write(0)
        bin2.write(0)
        bpwm.write(0)
    elif direction > 0: # car direction right
        bin1.write(1)
        bin2.write(0)
        bpwm.write(direction)
    elif direction < 0: # car direction left
        bin1.write(0)
        bin2.write(1)
        bpwm.write(-direction)


for event in device.read_loop():
    if event.type == ecodes.EV_ABS:
        if event.code == ecodes.ABS_X:
            controlDirection(((event.value - 128.0) / 128.0) / 2)
        elif event.code == ecodes.ABS_RZ:
            controlSpeed((event.value - 128.0) / 128.0)

    elif event.type == ecodes.EV_KEY:
        if event.code == ecodes.BTN_TL:
           if car_on == False:
                print("Motor wurde eingeschaltet!")
                car_on = True
                stby.write(1)
        elif event.code == ecodes.BTN_TR:
            if car_on == True:
                print("Motor wurde ausgeschaltet!")
                car_on = False
                stby.write(0)
