import sys
import time
import telnetlib



class neoLed:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port

    def cmd(self,cmdList):
        self.cmd = ":"
        for value in cmdList:
            self.cmd += value + ":"
        self.sendCmd()

    def sendCmd(self):
        tn = telnetlib.Telnet(self.ip, self.port)
        tn.read_until(b"*HELLO*", 2)
        tn.write(self.cmd)
        tn.close()




if __name__ == "__main__":
    # solid = [
    #             action,
    #             cycle,
    #             strip,
    #             startled,
    #             length,
    #             delay,
    #             red,
    #             green,
    #             blue,
    #             dir,
    #             ]
    solid = [
            "solid",
            "0",
            "0",
            "1",
            "20",
            "60",
            "0",
            "127",
            "127",
            "0",
            ]
    # rCycle = [
    #          action,
    #          cycle,
    #          ledstrip,
    #          startled,
    #          length,   #number of leds to use
    #          delay,
    #          Color1 r,
    #          Color1 g,
    #          Color1 b,
    #          Color2 r,
    #          Color2 g,
    #          Color2 b,
    #          dir,
    #          ]
    rCycle = [
             "rCycle",
             "1",
             "0",
             "1",
             "30",
             "100",

             "255",
             "0",
             "0",

             "0",
             "255",
             "0",

             "0",
             ]
    # slide = [
    #         action,
    #         cycle,
    #         ledstrip,
    #         startLed,
    #         length,   #number of leds to use
    #         delay,
    #         Color1 r, #slide color
    #         Color1 g,
    #         Color1 b,
    #         Color2 r, #background color
    #         Color2 g,
    #         Color2 b,
    #         slide length,
    #         dir,
    #         ]
    slide = [
            "slide",
            "1",
            "0",
            "1",
            "10",
            "100",

            "0",
            "0",
            "255",

            "0",
            "255",
            "0",

            "1",
            "0",
            ]
    strip = neoLed("192.168.1.161", 2000)
    #strip.cmd(solidList)
    #time.sleep(3)
    #strip.cmd(rainbowCycleList)
    strip.cmd(rCycle)




