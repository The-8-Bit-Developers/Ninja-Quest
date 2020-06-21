-- The enemy variable is set in C++

-- Load sprites in OnCreate
function OnCreate()

	SetTexture(enemy, "playerLeft.png")
	SetLayer(enemy, 1) -- Put enemy on layer 1 - on top of ground but not player
	AddPhysics(enemy, true, 0.01, 32, 40) -- Add physics - dynamic, 0.01 density, X wide, Y high
	SetFriction(enemy, 1000) -- Set friction to a high amount to stop the enemy moving pretty much insantly

end

-- Main game loop - move player
function OnUpdate(delta)

	-- Do stuff - look at 8BitEngine/include/LuaComponent.h to see all the functions you can do, and ask me :)
	-- Eg SetX(sprite_id, X)
	-- You can create sprites too!
	-- shuriken = CreateSprite("texture.png")

end
