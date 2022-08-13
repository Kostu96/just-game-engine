CameraController = LuaScript:create()

CameraController.Speed = 10

function CameraController:onUpdate(dt)
    local xVel = 0
    local yVel = 0

    if (Input.isKeyPressed(Input.Key.A)) then
        xVel = -self.Speed
    elseif (Input.isKeyPressed(Input.Key.D)) then
        xVel = self.Speed
    end

    if (Input.isKeyPressed(Input.Key.W)) then
        yVel = self.Speed
    elseif (Input.isKeyPressed(Input.Key.S)) then
        yVel = -self.Speed
    end

    self:move(xVel * dt, yVel * dt)
end

return CameraController
