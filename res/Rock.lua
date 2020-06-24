local delete_delay = 1000
local delete_time = 0

local rock_gravity = 0.5

function OnCreate()

	-- Create shuriken where camera is
	SetTexture(sprite, "Rock.png")
	SetLayer(sprite, 3)
	SetX(sprite, GetX("camera"))
	SetY(sprite, GetY("camera"))
	AddSpherePhysics(sprite, true, 0.01)
	SetTrigger(sprite)
	--SetFriction(sprite, 1000)

	SetGravity(sprite, rock_gravity)

	-- Work out velocity, asuming the camera's
	-- position is the centre of the world
	targetX = GetMouseX() - GetX("camera")
	targetY = GetMouseY() - GetY("camera")
	
	-- Normalise velocity and apply it
	magnitude = math.sqrt(targetX*targetX + targetY*targetY)
	SetVelocityX(sprite, targetX * magnitude)
	SetVelocityY(sprite, targetY * magnitude)

end

-- Main game loop - move player
function OnUpdate(delta)

	delete_time = delete_time + delta

	if (IsTriggered(sprite) and delete_time > delete_delay) then
		Delete(sprite)
	end

	if (GetY(sprite) < -1000) then Delete(sprite) end

end
