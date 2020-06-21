-- The player variable is set in C++

local grounded = false

key_w = 22
key_a = 0
key_s = 18
key_d = 3

key_up = 73
key_down = 74
key_left = 71
key_right = 72

key_space = 57

local speed = 100.0
local jump_force = 2
local camera_speed = 0.2

-- Load sprites in OnCreate
function OnCreate()

	SetTexture(player, "player.png")
	SetLayer(player, 2) -- Put player on top of everything else
	AddPhysics(player, true, 0.01, 32, 40) -- Add physics - dynamic, 0.01 density, X wide, X high
	SetFriction(player, 1000) -- Set friction to a high amount to stop the player moving pretty much insantly

end

-- Main game loop - move player
function OnUpdate(delta)

	-- Get if player is grounded by casting a ray from his position
	-- 20.1 units down (don't ask me why, that number's magic!)
	grounded = RayCast(GetX(player), GetY(player), 0, -20.1)

	-- Move player if only one key is pressed
	if (GetKeyDown(key_w) and GetKeyDown(key_a) == false and GetKeyDown(key_d) == false and grounded) then 
		SetVelocityY(player, jump_force * speed * delta) 
	end
	if (GetKeyDown(key_a) and GetKeyDown(key_w) == false) then SetVelocityX(player, -speed * delta) end
	if (GetKeyDown(key_d) and GetKeyDown(key_w) == false) then SetVelocityX(player,  speed * delta) end

	-- Move player if two keys are pressed
	if (GetKeyDown(key_w) and GetKeyDown(key_a) and grounded) then
		SetVelocityX(player, -speed * delta * 2.5)
		SetVelocityY(player, jump_force * speed * delta)
	end
	if (GetKeyDown(key_w) and GetKeyDown(key_d) and grounded) then
		SetVelocityX(player, speed * delta * 2.5)
		SetVelocityY(player, jump_force * speed * delta)
	end

	-- The physics is weird so if we are in the air we must manually do graity
	-- if moving left or right
	if ((GetKeyDown(key_a) or GetKeyDown(key_d)) and grounded == false and GetVelocityY(player) < 1.0) then 
		if (GetKeyDown(key_a)) then SetVelocityX(player, -speed * delta * 0.25) end
		if (GetKeyDown(key_d)) then SetVelocityX(player,  speed * delta * 0.25) end
		SetVelocityY(player, GetGravity() * -1) 
	end

	-- Change player texture based on movement
	if (GetVelocityX(player) <  0) then SetTexture(player, "playerLeft.png") end
	if (GetVelocityX(player) >= 0) then SetTexture(player, "player.png") end

end
