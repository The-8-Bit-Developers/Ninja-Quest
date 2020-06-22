-- The player variable is set in C++

local offset_x = 0
local offset_y = 0
local sensitivity = 0.1

key_up = 73
key_down = 74
key_left = 71
key_right = 72

function OnCreate()

	
end

function OnUpdate(delta)

    -- For now, simply follow the player, but keep in mind an
    -- offset changed by keyboard input
    SetX("camera", GetX(player) + offset_x)
    SetY("camera", GetY(player) + offset_y)

    -- Keyboard input - apply only in debug mode
    if (debug_mode == false) then return end
    if(GetKeyDown(key_up))      then offset_y = offset_y + sensitivity * delta end
    if(GetKeyDown(key_down))    then offset_y = offset_y - sensitivity * delta end
    if(GetKeyDown(key_left))    then offset_x = offset_x - sensitivity * delta end
    if(GetKeyDown(key_right))   then offset_x = offset_x + sensitivity * delta end

end
