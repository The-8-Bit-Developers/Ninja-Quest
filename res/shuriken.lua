local shuriken_gravity = 0.3

function OnCreate()

	-- Create shuriken where camera is
	SetTexture(sprite, "shuriken.png")
	SetLayer(sprite, 3)
	SetX(sprite, GetX("camera"))
	SetY(sprite, GetY("camera"))
	AddSpherePhysics(sprite, true, 0.01)
	SetTrigger(sprite)
	--SetFriction(sprite, 1000)

	-- Decide type
	type = GetType()
	if (type == 0) then SetTexture(sprite, "shuriken.png") end
	if (type == 1) then SetTexture(sprite, "Rock.png") end
	if (type == 2) then SetTexture(sprite, "Paper_Sword.png") end
	SetBodyData(sprite, 2 + type)


	SetGravity(sprite, shuriken_gravity)

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

	if (IsTriggered(sprite)) then
		id1, id2, data1, data2 = GetTriggeredData(sprite)
		-- If not player
		if (data1 ~= 1 and data2 ~= 1) then Delete(sprite) end
	end

	if (GetY(sprite) < -1000) then Delete(sprite) end

end
