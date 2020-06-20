local enemy

-- Load sprites in OnCreate
function OnCreate()

	enemy = CreateSprite("playerLeft.png")
	SetLayer(enemy, 1) -- Put player on top of everything else
	AddPhysics(enemy, false) -- Add physics - dynamic, 0.01 density, X wide, X high
	SetFriction(enemy, 1000) -- Set friction to a high amount to stop the player moving pretty much insantly

end

-- Main game loop - move player
function OnUpdate(delta)

	-- Do stuff - look at 8BitEngine/include/LuaComponent.h to see all the functions you can do, and ask me :)

end
