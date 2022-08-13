# Lua Script API

## Components
#### ComponentTypeID.Camera
#### ComponentTypeID.SpriteRenderer
#### ComponentTypeID.CircleRenderer
#### ComponentTypeID.BoxCollider2D
#### ComponentTypeID.CircleCollider2D
#### ComponentTypeID.Rigidbody2D

## LuaEntity
#### LuaEntity:addComponent(number [, ...])
Adds component of the specified type to the entity.

Parameters
  - id representing component type. see [Components](#Components)
  - variable number of arguments needed to construct particular component

Returns
  - Newly addded component 

#### LuaEntity:getComponent(number)
Gets component of the specified type attached to the entity.

Parameters
  - id representing component type. see [Components](#Components)

Returns
  - The component 

#### LuaEntity:setPosition(number, number, number)
Sets the translation component of the entity's transform.

Parameters
  - x coordinate
  - y coordinate
  - z coordinate

#### LuaEntity:getPosition()
Gets the translation component of the entity's transform.

Returns
  - x coordinate
  - y coordinate
  - z coordinate

#### LuaEntity:getScale()
Gets the scale component of the entity's transform.

Returns
  - x coordinate
  - y coordinate
  - z coordinate

#### LuaEntity:move(number, number, number)
Adds to the translation component of the entity's transform.

Parameters
  - x coordinate offset
  - y coordinate offset
  - z coordinate offset

#### LuaEntity:scale(number, number, number)
Multiplies the scale component of the entity's transform.

Parameters
  - x coordinate multiplier
  - y coordinate multiplier
  - z coordinate multiplier

## LuaScene
#### LuaScene:createEntity()
Creates new entity.

Returns
  - Newly created entity
