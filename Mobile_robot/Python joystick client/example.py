import joystickapi
import msvcrt
import time
import requests

url = 'http://192.168.1.45'
def sendCommandToRobot(myobj):
    x = requests.post(url, json = myobj)

from enum import Enum
class SteeringMode(Enum):
    PARALLEL = 1
    SPIN = 2


print("start")
num = joystickapi.joyGetNumDevs()
ret, caps, startinfo = False, None, None
for id in range(num):
    ret, caps = joystickapi.joyGetDevCaps(id)
    if ret:
        print("gamepad detected: " + caps.szPname)
        ret, startinfo = joystickapi.joyGetPosEx(id)
        break
else:
    print("no gamepad detected")

angleRange = [-50, 50]
currentSteering = 0
distanceRange = [-100, 100]
originalRange = [-32768, 32768]
steeringMode = SteeringMode.PARALLEL
sendCommandToRobot({"command": "sp0\n"})


def map(value, range, inverse = False) :
    if (inverse):
        value *= -1
    value = (value - originalRange[0]) / (originalRange[1] - originalRange[0])
    value = value * (range[1] - range[0]) + range[0]
    return int(value)


run = ret
while run:
    time.sleep(0.1)
    if msvcrt.kbhit() and msvcrt.getch() == chr(27).encode(): # detect ESC
        run = False

    ret, info = joystickapi.joyGetPosEx(id)
    if ret:
        btns = [(1 << i) & info.dwButtons != 0 for i in range(caps.wNumButtons)]
        axisXYZ = [info.dwXpos-startinfo.dwXpos, info.dwYpos-startinfo.dwYpos, info.dwZpos-startinfo.dwZpos]
        axisRUV = [info.dwRpos-startinfo.dwRpos, info.dwUpos-startinfo.dwUpos, info.dwVpos-startinfo.dwVpos]
        if info.dwButtons:
            print("buttons: ", btns)
            if btns[4]:
                if steeringMode == SteeringMode.PARALLEL:
                    steeringMode = SteeringMode.SPIN
                else:
                    steeringMode = SteeringMode.PARALLEL
        if any([abs(v) > 10 for v in axisXYZ]):
            print("axis:", axisXYZ)
            v = map(axisXYZ[0], angleRange)
            if steeringMode == SteeringMode.PARALLEL:
                currentSteering += v
                if currentSteering > angleRange[1]:
                    currentSteering = angleRange[1]
                elif currentSteering < angleRange[0]:
                    currentSteering = angleRange[0]
                sendCommandToRobot({"command": "sp" + str(currentSteering) + "\n"})
            elif steeringMode == SteeringMode.SPIN:
                sendCommandToRobot({"command": "ds" + str(v) + "\n"})
        if any([abs(v) > 10 for v in axisRUV]):
            print("roation axis:", axisRUV)
            v = map(axisRUV[0], distanceRange, inverse = True)
            sendCommandToRobot({"command": "dp" + str(v) + "\n"})
            steeringMode = SteeringMode.PARALLEL
print("end")
