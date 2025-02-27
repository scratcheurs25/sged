import pygame
import  gameObject
import key_gestion
import random

pygame.init()
screen = pygame.display.set_mode((500,400))
pygame.display.set_caption("lanch rock")

playerImg = pygame.image.load("player.png")
rockImg = pygame.image.load("rock.png")
enemyImg = pygame.image.load("enemy.png")

gameIsRunning = True

player = gameObject.GameObject(playerImg,215.5,350)
enemy = gameObject.GameObject(enemyImg, random.randint(0,409),-91)

rocks = []

key_right = key_gestion.key()
key_left = key_gestion.key()

def colistion(spr1:gameObject.GameObject,spr2:gameObject.GameObject):
    if (spr1.x + spr1.img.get_width() < spr2.x or
        spr2.x + spr2.img.get_width() < spr1.x or
        spr1.y + spr1.img.get_height() < spr2.y or
        spr2.y + spr2.img.get_height() < spr1.y):
        return False
    else:
        return True

while gameIsRunning:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            gameIsRunning = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                key_right.Key_Down()
            if event.key == pygame.K_LEFT:
                key_left.Key_Down()
            if event.key == pygame.K_SPACE:
                rocks.append(gameObject.GameObject(rockImg,player.x+23,player.y-20))
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT:
                key_right.Key_Up()
            if event.key == pygame.K_LEFT:
                key_left.Key_Up()
    if key_right.Key_state():
        player.Move(10,0)
    if key_left.Key_state():
        player.Move(-10,0)
    enemy.Move(0,5)
    if enemy.y > 400 :
        gameIsRunning = False
    screen.fill(pygame.Color(0,155,255))
    player.show(screen)
    for e in rocks:
        e.show(screen)
        e.Move(0,-10)
        if colistion(e,enemy):
            enemy = gameObject.GameObject(enemyImg, random.randint(0,409),-91)
        if e.y < 0:
            e = None
    enemy.show(screen)
    pygame.display.flip()
    pygame.time.Clock().tick(30)
