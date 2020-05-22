# LUA Scripting

Behavior of the Fallout Tactics game can be customized by adding a "ftse.lua" file to the Fallout: Tactics directory. The script defines changes to the game behavior.

Behavior changes are added by implementing functions described in the @{Hooks} section. The FTSE engine will call these functions whenever the specific event happens in the game.

Parameters to hooks may include @{Entity|Entities} in addition to other state information. Each @{Entity} class type has helper functions which allow for information about the entity to be retrieved, and in some cases changed.

See the Examples section for examples of how to use the scripting engine.
