--[[

For this example, we will be modifying the Environmental Armour to include
an attached weapon; in this case, the Gauss Rifle.  The rifle should be
automatically equipped and unequipped when the corresponding armor is put on
or taken off, and should not be otherwise removable from the character.

--]]

-- Hook OnEquip to add the weapon to slot 1 (right hand)
function OnEquip(equipper, item, slot)

	-- Is this an environmental armour?
	if item:GetInternalName() == "bosEnviro" and slot == EQUIP_SLOT_ARMOR then
		
		-- Yes, make a weapon entity and equip it
		local newent = world:CreateEntity("entities/weapons/Rifles/gaussRifle.ent",EQUIP_SLOT_RIGHT)
		local prevequip,errstr = equipper:EquipItem(newent, EQUIP_SLOT_RIGHT)
		if errstr ~= nil then
			-- We failed to equip: This means we got back the entity we tried to
			-- equip, so just drop it
			prevequip:Destruct()
		else
			-- Succeeded on equipping. This means we may have gotten back the
			-- character's previously equipped item.  Put it in the inventory
			-- if we got something that isn't the null entity.
			if prevequip.ClassType ~= "Entity" then
				equipper:AddToInventory(prevequip)
			end
		end
	end
end

-- Hook OnUnequip to remove the weapon from slot 1 and destruct it
function OnUnequip(equipper, item, slot)
	
	-- Is this an environmental armour?
	if item:GetInternalName() == "bosEnviro" and slot == EQUIP_SLOT_ARMOR then
		
		-- Yes, unequip the added item and destruct it
		-- Not destructing the entity will leak resources
		local removedent = equipper:UnequipItem(EQUIP_SLOT_RIGHT)
		removedent:Destruct()
	end
end

-- Hook OnCheckUnequip to disallow removal of the weapon when armor is equipped
-- Note that this code doesn't work perfectly if the item exists independently
-- of the armor; specifically, if the character already has that weapon equipped
-- in slot 1, and tries to equip the armor, it will fail.

function OnCheckUnequip(equipper, item, slot)
	
	-- Is this a gauss rifle in slot 1?
	if item:GetInternalName() == "gaussRifle" and slot == EQUIP_SLOT_RIGHT then
		
		-- Is character wearing environmental armour?
		if equipper:GetEquippedItem(EQUIP_SLOT_ARMOR):GetInternalName() == "bosEnviro" then
		
			-- Yes, disallow the operation
			return "cannotRemove"
		end
	end
	return
end

