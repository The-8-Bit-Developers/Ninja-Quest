-- The player variable is set in C++

local offset_x = 0
local offset_y = 0
local sensitivity = 0.1
local lerp_speed = 0.005

key_up = 73
key_down = 74
key_left = 71
key_right = 72

function OnCreate() 
	SetX("camera", GetX(player))
	SetY("camera", GetY(player))
end

-- Returns the amount of movement at this stage of the lerp
function LerpVelocity(position, target, speed)
	return (target - position) * speed;
end

-- Does lerping for camera
function Lerp(position, target, speed)
	position = position + LerpVelocity(position, target, speed);
	return position;
end

function OnUpdate(delta)

	-- Follow the player, keeping in mind offset
	-- Lerp so as to make it smooth
	targetX = GetX(player) + offset_x
	targetY = GetY(player) + offset_y
	cameraX = Lerp(GetX("camera"), targetX, delta * lerp_speed)
	cameraY = Lerp(GetY("camera"), targetY, delta * lerp_speed)
	SetX("camera", cameraX)
	SetY("camera", cameraY)

	-- Keyboard input - apply only in debug mode
	if (debug_mode == false) then return end
	if(GetKeyDown(key_up))      then offset_y = offset_y + sensitivity * delta end
	if(GetKeyDown(key_down))    then offset_y = offset_y - sensitivity * delta end
	if(GetKeyDown(key_left))    then offset_x = offset_x - sensitivity * delta end
	if(GetKeyDown(key_right))   then offset_x = offset_x + sensitivity * delta end

end
