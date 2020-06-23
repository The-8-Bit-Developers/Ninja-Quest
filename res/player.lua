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
local hack_speed = speed * 0.001
local jump_force = 2
local camera_speed = 0.2

-- Load sprites in OnCreate
function OnCreate()

	SetTexture(player, "player.png")
	SetLayer(player, 2) -- Put player on top of everything else
	--AddSpherePhysics(player, true, 0.01, 20) -- Add physics but with a sphere collider - dynamic, 0.01 density, X radius
	AddPhysics(player, true, 0.01, 32, 40)
	SetFriction(player, 1000) -- Set friction to a high amount to stop the player moving pretty much insantly

end

-- Main game loop - move player
function OnUpdate(delta)

	-- Get if player is grounded by casting a ray from his position
	-- 20.1 units down (don't ask me why, that number's magic!)
	grounded = RayCast(GetX(player), GetY(player), 0, -20.1)

	-- Move player if only one key is pressed
	if (GetKeyDown(key_w) and grounded) then 
		SetVelocityY(player, jump_force * speed * delta) 
	end
	
	-- Hack of the cenutry - collision left and right is broken, so do
	-- raycasting instead and hope for the best!
	leftCollision = RayCast(GetX(player), GetY(player), -16.0, 0)
	rightCollision = RayCast(GetX(player), GetY(player), 16.0, 0)
	if (GetKeyDown(key_a) and leftCollision == false) then SetX(player, GetX(player) - hack_speed * delta) end
	if (GetKeyDown(key_d) and rightCollision == false) then SetX(player, GetX(player) + hack_speed * delta) end

	-- The problem is that this hack adversely affects velocity, so
	-- let's fix that in the worst possible way!
	-- (and in C++ too to confuse everyone!)


	-- Change player texture based on movement
	if (GetVelocityX(player) <  0) then SetTexture(player, "playerLeft.png") end
	if (GetVelocityX(player) >= 0) then SetTexture(player, "player.png") end

end
