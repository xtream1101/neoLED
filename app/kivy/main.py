import telnetlib
import time

from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.properties import ObjectProperty
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout


class HomeScreen(Screen):
    def sendCmd(self, command):
        tn = telnetlib.Telnet("192.168.1.161", 2000)
        tn.read_until(b"*HELLO*", 2)
        tn.write(command)
        time.sleep(1)
        tn.close
    def setRainbow(self):
        self.sendCmd("rainbowCycle:25")

    def setRedL(self):
        self.sendCmd("wipe:TL:10:255:0:0;")
        
    def setGreenL(self):
        self.sendCmd("wipe:TL:10:0:255:0;")
        
    def setBlueL(self):
        self.sendCmd("wipe:TL:10:0:0:255;")
        
    def setRedR(self):
        self.sendCmd("wipe:TR:10:255:0:0;")
        
    def setGreenR(self):
        self.sendCmd("wipe:TR:10:0:255:0;")
        
    def setBlueR(self):
        self.sendCmd("wipe:TR:10:0:0:255;")
        

class NeoLEDApp(App):

    def build(self):
        return HomeScreen()



if __name__ == '__main__':
    NeoLEDApp().run()