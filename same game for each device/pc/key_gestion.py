class key:
    def __init__(self):
        self.keyPress = False
    def Key_Down(self):
        self.keyPress = True
    def Key_Up(self):
        self.keyPress = False
    def Key_state(self):
        return  self.keyPress
