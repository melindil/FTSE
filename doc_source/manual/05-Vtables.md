# Calling and Hooking Entity Vtable Functions

This section describes how to use the Entity virtual table (vtable) hook and call functionality added in FTSE version 0.55a.

**NOTE that this functionality is still considered experimental.  It is a good idea to use a backup save file or mod branch when using features described here.**

## What is the Entity Vtable?

(What follows is a high-level description of how C++ virtual functions/tables work, with a focus on how they are used within Fallout Tactics.  For a detailed, low-level description of how virtual function tables work, you can check Wikipedia [here](https://en.wikipedia.org/wiki/Virtual_method_table). )

For games developed in object-oriented languages (like C++), it is common for the objects in the game which the player can interact with to be implemented as a set of class types, each implementing the behavior that the particular type of object should exhibit.
In Fallout Tactics, nearly all objects which the player can interact with in the game world are based on a single parent class named @{Entity}. From @{Entity}, all other interactable game objects (e.g. @{Actor}, @{Weapon}, @{Vehicle}, etc.) are derived.

The @{Entity} class contains data and code which are common to all entity types.  Part of this code is a set of functions for which there is a "default" behavior, but for which it is expected that other entity types are expected to replace or extend. In object-oriented languages, this is often 
implemented as a form of "polymorphism"; that is, a set of expected behaviors or capabilities described at a high level, but implemented separately for different types of objects. An example would be to have a "Draw" function defined at the @{Entity} level, and for each subclass to have the
appropriate behavior for "Draw" based on its type (e.g. @{Actor}s and @{Vehicle}s need animation capability, @{Light}s need to modify the color and intensity of surrounding objects, etc.).  In C++, the mechanism provided for enabling polymorphism is a "virtual function".  In the example case, Entity would
describe the Draw function as virtual, and may also provide a default implementation for it. Each subclass that needs special behavior would then override this function with its own implementation.

At a machine-code level, most (all?) C++ compilers implement virtual functions by means of a "virtual function table" (Vtable).  Each object of a class will contain data pointing to the specific set of virtual functions that apply to that instance of the class (depending on exactly which class that
object was created as).  Whenever part of the code needs to call one of these virtual functions for an object, the compiler will generate code that will look up the proper function in the object's vtable pointer, and branch execution to that point. This decision can be made at runtime for every
object, so there's no need to recompile if the set of objects change (e.g. depending on which game level is loaded).

## What features does FTSE provide to game modifications by utilizing the Entity Vtable?

FTSE scripts, implemented in Lua, can use the Entity Vtable in two ways:

1. By calling virtual table functions on entities, the mod script can retrieve or change game state in ways beyond the existing interface functions provided in the FTSE class definitions.  Over time, most of the useful Vtable functions will eventually be implemented directly into FTSE, for performance
reasons if nothing else. However, this will take considerable time and reverse engineering to provide a stable set of functionality. The vtable calls can be experimented with immediately, and any learning by the community on these functions can then be converted into better and more stable code in the
future.

2. By placing a hook on a virtual table function, the mod script can be notified whenever the actual game code tries to use the same function to change state.  This gives considerably more flexibility to scripts to request notification on game events, in a way that is far easier to implement than
the existing "trampoline" style events already provided in FTSE.  In particular, both calling and hooking a vtable function only requires knowing the expected parameter and return types, while a trampoline style hook will usually require hand-coded machine language built into the FTSE code (see the HookInstaller
module in the FTSE C++ code base for an example of what these hand-coded routines look like). 

## Calling Vtable Functions

Calls to Vtable functions are made using the new CallVtable function provided in the FTSE @{Entity} class definition. The basic steps for doing so are:

1. Look up the desired function in the @{Entity_Vtable|Entity Vtable function list}. This list provides the vtable function ID number, required parameters, and expected return values.

2. Call the entity:CallVtable function. The first parameter will be the Vtable function ID number. Following parameters will vary depending on which function is being called.

3. If a return value is expected, assign the return from the entity:CallVtable call to a variable, and use the output as appropriate.

## Hooking Vtable Functions

Installing a hook for a Vtable function is done by creating an appropriate Lua function as the hook code, and then using the @{hookexecutor:InstallVtableHook} function to set that function as a hook. The basic steps are:

1. Look up the desired function to hook in the @{Entity_Vtable|Entity Vtable function list}. In particular, take note of the expected parameters and return values.

2. Create the hook function, that will be executed whenever the game code calls the desired vtable function. The function must take an entity as the first parameter, followed by the parameters which are defined for the specific vtable function being hooked.

3. As part of the hook function, it may be desired to call the original vtable function code (e.g. if the script only wants to be notified of an event, and wants to leave the default behavior in place). This can be done by calling the entity:CallOrigVtable function, passing in the vtable ID as first parameter, followed by other expected parameters for that particular function. **NOTE that calling entity:CallVtable from inside a vtable hook, calling the same vtable function, will likely result in an infinite loop and game crash.**

4. Ensure that the hook function returns the correct type(s) and value(s) so that the game code can continue as desired.

5. Once the hook function is created, use @{hookexecutor:InstallVtableHook}, giving the class name to install the hook for (or ALL for all Entity types), the vtable ID number, and the name of the hook function.

## Caveats

Note that, while the game implements a number of vtable functions which just return information about an object, it is not **guaranteed** that all parts of the game code which check that information will always use the vtable function. Specifically, if part of the code knows how to read the data
for an object directly, it will often do so rather than call the vtable function, as this often has a performance benefit. As such, vtable hooks on many of the "Get" or "Is" type vtable functions will rarely be useful. The same applies for "Set" type functions, but these are somewhat more likely
to use the vtable functions, as changing many forms of entity data will have side effects (e.g. changing position of an entity also changes bounding box and collision data), and using the vtable functions is easier from a coding perspective than duplicating those side effects everywhere else.  Finally, many
of the "action" types of functions (e.g. which trigger behaviors at a game level) will almost always use the vtable, as these kinds of interactions often involve multiple object types, which may not know how to directly interact with the data of the other object types.

## Example

The @{vtableexample.lua|Vtable Example} provided shows how to use Vtable hooks and calls to change game behavior of an existing class type; in this case, the existing Bullhorn object is extended such that a character with a high Science skill can use that skill on the bullhorn to disable all
surrounding active traps. This is done by installing a hook on the game vtable functions which handle skill use on objects, and under the correct condition calling the vtable function corresponding to "disable trap" on all active Trap entities found nearby.