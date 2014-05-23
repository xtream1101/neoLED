import sys
import time
import telnetlib



IP = "192.168.1.161"
PORT = 2000


def sendCmd(cmd):
	tn = telnetlib.Telnet(IP, PORT)
	tn.read_until(b"*HELLO*", 2)
	tn.write(cmd)
	time.sleep(.5)
	tn.close()




if __name__ == "__main__":
	cmd = ":"
	for arg in sys.argv:
		if arg != sys.argv[0]:
			cmd += arg + ":"
	sendCmd(cmd)


