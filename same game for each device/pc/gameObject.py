import pygame.transform


class GameObject:
    def __init__(self,img,x,y):
        self.img = img
        self.x = x
        self.y = y
    def show(self,screen):
        screen.blit(self.img,(self.x,self.y))
    def Set_pos(self,x,y):
        self.x = x
        self.y = y
    def Move(self,vx,vy):
        self.x += vx
        self.y += vy


