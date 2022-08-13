# Lua Script API

### Components
#### Component.Tag
#### Component.Transform
#### Component.Camera
#### Component.SpriteRenderer
#### Component.CircleRenderer
#### Component.BoxCollider2D
#### Component.CircleCollider2D
#### Component.Rigidbody2D

### LuaEntity
#### LuaEntity:addComponent(number [, ...])
Adds component to the entity.

Parameters
  - id representing component type. see [Components](#Components)

Returns
  - Newly addded component 

#### LuaEntity:setPosition(number, number, number)
Sets position of the entity's TransformComponent

Parameters
  - x coordinate
  - y coordinate
  - z coordinate

#### LuaEntity:getPosition()
Gets position of the entity's TransformComponent

Returns
  - x coordinate
  - y coordinate
  - z coordinate
