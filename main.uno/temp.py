import os
import struct


pipe = open('/tmp/my_pipe', 'rb')


while ...:
    number1 = struct.unpack('d', pipe.read(8))[0]
    number2 = struct.unpack('d', pipe.read(8))[0]
    number3 = struct.unpack('d', pipe.read(8))[0]
    number4 = struct.unpack('d', pipe.read(8))[0]

    print(number1, number2, number3, number4)

pipe.close()

