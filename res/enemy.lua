-- The enemy variable is set in C++, as is the player variable

local rope_height = 32
local number_of_ropes = math.floor(1000 / rope_height)
local ropes = {}
local ropes_offsets = {} -- So each enemy does not hold onto the rope at the exact same position
local global_rope_offset_x = 0
local rope_offset_count = math.random(0, 10000)
local rope_offset_speed = 0.01

local enemy_offset_count = math.random(0, 10000)
local enemy_offset_speed = 0.01

game_over = false
shuriken = 0

-- Load sprites in OnCreate
function OnCreate()

	SetTexture(enemy, "playerLeft.png")
	AddPhysics(enemy, true, 0.01, 32, 40)
	--SetTrigger(enemy)
	SetLayer(enemy, 1)
	SetFriction(enemy, 1000)

	-- Disable gravity
	SetGravity(enemy, 0.0)

	-- Whilst we could have only the required amount of ropes to
	-- minimise memory usage, it would be more costly in terms
	-- of CPU as resizing vectors is sloooooow
	for i=1,number_of_ropes do
		ropes[i] = CreateSprite("rope.png")
	end
	global_rope_offset_x = math.random() * 8
	global_rope_offset_y = math.random() * -50

	SetY(enemy, 150)

	SetBodyData(enemy, 100)

	print("making", enemy)

end

-- Main game loop - move player
function OnUpdate(delta)

	-- Update rope offsets
	rope_offset_count = rope_offset_count + rope_offset_speed * delta
	for i=1,number_of_ropes do
		ropes_offsets[i] = math.sin(rope_offset_count + i) * 1
	end

	-- Update enemy offsets
	enemy_offset_count = enemy_offset_count + enemy_offset_speed * delta
	enemy_offset = math.sin(enemy_offset_count) * 0.00
	SetX(enemy, GetX(enemy) + enemy_offset)

	-- Update rope positions
	enemyX = GetX(enemy)
	enemyY = GetY(enemy)
	ropeOffset = rope_height * 1.5
	for i=1,number_of_ropes do
		SetX(ropes[i], enemyX + global_rope_offset_x + ropes_offsets[i])
		SetY(ropes[i], enemyY + rope_height * i - ropeOffset)
	end

	playerX = GetX(player)
	totalX = playerX - enemyX

	if(totalX < 0) then
		AddForce(enemy, -10000, 0)
	else
		AddForce(enemy, 10000, 0)
	end

	AddY(enemy, -0.01 * delta)

	-- Stop the player moving an enemy
	SetVelocityY(enemy, 0)

	-- Check for collisions
	if (IsTriggered(enemy)) then
		id1, id2, data1, data2 = GetTriggeredData(enemy)
		-- If player or ground
		if (data1 <= 1 or data2 <= 1) then game_over = true end
		if (data1 > 1 and data1 < 100 or data2 > 1 and data2 < 100) then
			if (data1 > 1 and data1 < 100) then shuriken = data1
			else shuriken = data2 end
		end
	end

end