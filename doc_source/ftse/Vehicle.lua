--- Vehicle is the @{Entity} definition for a vehicle.
-- Subclass of @{Entity}.
-- @entity Vehicle

--- GetAttribute returns the value of the given attribute for the vehicle.
-- @param attrib The attribute to check, as a string. Should match attribute strings from the editor.
-- @return An integer indicating the vehicle's attribute value.
function Vehicle:GetAttribute(attrib)
end

--- GetHP returns the current HP of the vehicle.
-- @return An integer indicating the current HP.
function Vehicle:GetHP()
end

--- GetDriver returns the current driver of the vehicle.
-- @return An @{Actor} object indicating the current driver, or nil if no driver.
function Vehicle:GetDriver()
end

--- GetGunner returns the current gunner of the vehicle.
-- @return An @{Actor} object indicating the current gunner, or nil if no gunner.
function Vehicle:GetGunner()
end

--- GetController returns the @{VehicleController) object for the vehicle.
-- @return A @{VehicleController} object for the vehicle.
function Vehicle:GetController()
end

--- GetPassenger returns the given numbered passenger of the vehicle.
--- @param num Which numbered passenger to retrieve.
-- @return An @{Actor} object for the passenger if there is one, else nil.
function Vehicle:GetPassenger(num)
end

--- GetRepairObject returns the repair object for this vehicle.
-- @return A @{Collectable} object for the repair object.
function Vehicle:GetRepairObject()
end

--- GetVehicleWeapon returns the weapon on this vehicle.
-- @return A @{VehicleWeapon} object if the vehicle has a weapon, else nil.
function Vehicle:GetVehicleWeapon()
end

--- GetDriverLeftHandItem returns the saved left hand item for the driver.
-- @return An @{Entity} object for the driver's left hand object.
function Vehicle:GetDriverLeftHandItem()
end

--- GetGunnerLeftHandItem returns the saved left hand item for the gunner.
-- @return An @{Entity} object for the gunner's left hand object.
function Vehicle:GetGunnerLeftHandItem()
end

--- GetInventory returns the inventory of the vehicle.
-- @return An array of @{Collectable} objects indicating the vehicle inventory.
function Vehicle:GetInventory()
end

--- GetNumPassengers returns the number of non-driving and non-gunning passengers in the vehicle.
-- @return An integer indicating the number of passengers.
function Vehicle:GetNumPassengers()
end

--- GetTotalOccupants returns the total number of occupants of the vehicle (driver, gunner, and passengers).
-- @return An integer indicating the number of occupants.
function Vehicle:GetTotalOccupants()
end

--- GetMaxPassengers returns the maximum number of non-driving and non-gunning passengers in the vehicle.
-- @return An integer indicating the maximum number of passengers.
function Vehicle:GetMaxPassengers()
end

--- GetMaxTotalOccupants returns the maximum number of total occupants of the vehicle (driver, gunner, and passengers).
-- @return An integer indicating the maximum number of occupants.
function Vehicle:GetMaxTotalOccupants()
end

--- GetTurnRate returns the turn rate of the vehicle.
-- @return An integer indicating the turn rate.
function Vehicle:GetTurnRate()
end

--- GetCriticalChance returns the critical hit chance if the vehicle rams a target.
-- @return An integer indicating the critical hit chance on ramming.
function Vehicle:GetCriticalChance()
end

--- GetDamageMultiplier returns the damage multiplier if the vehicle rams a target.
-- @return A numeric value indicating the damage multiplier on ramming.
function Vehicle:GetDamageMultiplier()
end

--- GetAcceleration returns the acceleration of the vehicle.
-- @return A numeric value indicating the acceleration.
function Vehicle:GetAcceleration()
end

--- GetMaxSlowSpeed returns the maximum "slow" speed of the vehicle.
-- @return A numeric value indicating the slow speed.
function Vehicle:GetMaxSlowSpeed()
end

--- GetMaxHighSpeed returns the maximum speed of the vehicle.
-- @return A numeric value indicating the maximum speed.
function Vehicle:GetMaxHighSpeed()
end

--- GetEndTurnTime returns the animation clock time of the end of the vehicle's change in direction.
-- @return A numeric value indicating the time of finishing a requested change of direction.
function Vehicle:GetEndTurnTime()
end

--- NeedsRepairObject returns true if the vehicle needs repair.
-- @return A boolean value indicating repair is needed.
function Vehicle:NeedsRepairObject()
end

--- HasWeapon returns true if the vehicle has a weapon.
-- @return A boolean value indicating the vehicle has a weapon.
function Vehicle:HasWeapon()
end

--- IsRunSpeed returns true if the vehicle is at high speed.
-- @return A boolean value indicating the vehicle is at high speed.
function Vehicle:IsRunSpeed()
end

--- IsReverse returns true if the vehicle is in reverse.
-- @return A boolean value indicating the vehicle is in reverse.
function Vehicle:IsReverse()
end

--- IsDestinationReached returns true if the vehicle is at its destination.
-- @return A boolean value indicating the vehicle is at its destination.
function Vehicle:IsDestinationReached()
end

--- IsTurnReached returns true if the vehicle has finished changing direction.
-- @return A boolean value indicating the vehicle has finished changing direction.
function Vehicle:IsTurnReached()
end

--- IsHavingTurn returns true if the vehicle is turning.
-- @return A boolean value indicating the vehicle is turning.
function Vehicle:IsHavingTurn()
end

--- GetCurrentAction returns the current animation action for the vehicle.
-- @return A string indicating the current action animation.
function Vehicle:GetCurrentAction()
end

--- GetButtonSprite returns the sprite to use for the vehicle controller.
-- @return A string indicating the vehicle control sprite.
function Vehicle:GetButtonSprite()
end

--- GetExitPoint returns the exit point of actors in the vehicle.
-- @return A table containing numeric values "x", "y", and "z" indicating the exit point.
function Vehicle:GetButtonSprite()
end

--- GetDestination returns the destination of the vehicle.
-- @return A table containing numeric values "x", "y", and "z" indicating the destination.
function Vehicle:GetDestination()
end

--- GetVelocity returns the directional velocity of the vehicle.
-- @return A table containing numeric values "x", "y", and "z" indicating the velocity.
function Vehicle:GetVelocity()
end

--- GetDimensions returns the linear dimensions of the vehicle.
-- @return A table containing numeric values "x", "y", and "z" indicating the vehicle dimensions.
function Vehicle:GetDimensions()
end
