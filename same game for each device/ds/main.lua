playerImg = Image.load("player.png",VRAM)
rockImg = Image.load("rock.png",VRAM)
enemyImg = Image.load("enemy.png",VRAM)

randomSeed = os.time()
math.randomseed(randomSeed)

player = {}
player.x = 128-35
player.y = 192-35
enemy = {}
enemy.x = math.random(1, 256-35)
enemy.y = 0

GameIsRunnig = true

rocks = {}

-- Function to detect collision between two rectangles
function checkCollision(x1, y1, w1, h1, x2, y2, w2, h2)
    return x1 < x2 + w2 and
           x2 < x1 + w1 and
           y1 < y2 + h2 and
           y2 < y1 + h1
end

while GameIsRunnig do
    Controls.read()
    randomSeed = randomSeed + 0.1
    math.randomseed(randomSeed)

    if Keys.held.Right then
        player.x = player.x + 5
    end
    if Keys.held.Left then
        player.x = player.x - 5
    end
    if Stylus.held then
        if Stylus.X > player.x then
            player.x = player.x + 5
        end
        if Stylus.X < player.x then
            player.x = player.x - 5
        end
    end

    if Keys.newPress.A  or Stylus.newPress then
        table.insert(rocks,{x = player.x+15,y = player.y})
    end
    enemy.y = enemy.y + 2
    if enemy.y > 384 then
        GameIsRunnig = false
    end

    screen.drawFillRect(SCREEN_DOWN,0,0,256,192,Color.new(0,21,31))
    screen.drawFillRect(SCREEN_UP,0,0,256,192,Color.new(0,21,31))

    for i ,e in ipairs(rocks) do
        screen.blit(SCREEN_DOWN,e.x,e.y,rockImg)
        screen.blit(SCREEN_UP,e.x,e.y+192,rockImg)
        e.y = e.y - 10
        if checkCollision(e.x,e.y+192,8,11,enemy.x,enemy.y,35,35) then
            enemy.x = math.random(1, 256-35)
            enemy.y = 0
        end
        if e.y < -192 then
            table.remove(rocks,i)
        end
    end


    screen.blit(SCREEN_DOWN,enemy.x,enemy.y-192,enemyImg)
    screen.blit(SCREEN_UP,enemy.x,enemy.y,enemyImg)

    screen.blit(SCREEN_DOWN,player.x,player.y,playerImg)
    render()
end

Image.destroy(playerImg)
Image.destroy(rockImg)
Image.destroy(enemyImg)
playerImg = nil
enemyImg = nil
rockImg = nil
player = nil