-- The enemy variable is set in C++o
local player = (10)

-- Load sprites in OnCreate
function OnCreate()

	SetTexture(enemy, "playerLeft.png")
	SetLayer(enemy, 1) -- Put enemy on layer 1 - on top of ground but not player
	AddPhysics(enemy, true, 0.01, 32, 40) -- Add physics - dynamic, 0.01 density, X wide, Y high
	SetFriction(enemy, 1000) -- Set friction to a high amount to stop the enemy moving pretty much insantly
	

end

-- Main game loop - move player
function OnUpdate(delta)
	playerX = GetX(player)
	enemyX = GetX(my_id)
	totalX = playerX - enemyX
	
	if(totalX < 0) then
		AddForce(enemy, -10000, 0)
	else
		AddForce(enemy, 10000, 0)
	end

	-- Do stuff - look at 8BitEngine/include/LuaComponent.h to see all the functions you can do, and ask me :)
	-- Eg SetX(sprite_id, X)
	-- You can create sprites too!
	-- shuriken = CreateSprite("texture.png")

end
