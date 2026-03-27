# Storing custom values in an Entity

Starting with FTSE 0.60a, entities can now
hold script-defined variables. These values are
properly serialized in a saved game, and will
persist on game reload and map change.

Values are stored using the @{Entity:SetCustomValue}
function, and retrieved with @{Entity:GetCustomValue}.

As of FTSE 0.60a, the following is supported:

* Only Actors and persistent entity types (containers, doors, etc.) can have
values saved. Items which can go into an inventory
will not work in this release.

* Supported Lua types for values are: boolean, numeric, string, and nested tables containing these types.

* Saving entity IDs is dangerous - IDs can change on reload
or map change. A future version will support saving
permanent references to entities in custom variables.

* Be aware that, when storing a Lua table with @{Entity:SetCustomValue}, the value returned by @{Entity:GetCustomValue} will be a reference
to the stored table. This means that any modifications to the returned table object will actually be copied into the saved version of the table as well. 