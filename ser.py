import serial
import subprocess

ser = serial.Serial('/dev/ttyUSB0', 115200)

def read_o():
    process=subprocess.Popen(["./pulse_freq.out"],stdout=subprocess.PIPE, universal_newlines=True)
    for stdout_line in iter(process.stdout.readline, ""):
        yield stdout_line

values_before = [0,0,0,0,0,0,0,0]
for item in read_o():
    computed_values = [0,0,0,0,0,0,0,0]
    values_now = item.split(" ") 
    for i in range(0,8):  
        val = abs(int(float(values_now[i]) - float(values_before[i])))
        computed_values[i] = val if val < 5 else 5
    value = "".join(str(v) for v in computed_values)
    value += '\n'
    print (value)
    ser.write(bytes(value,'utf-8'))
    values_before = item.split(" ")
