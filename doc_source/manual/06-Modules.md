# Lua Module support

Starting in FTSE 0.60a, Lua modules are now fully supported. This allows
independent units of functionality to be developed with minimal
conflict. Modules can implement their own version of each hook 
function (including Entity vtable hooks). 

## Installing a hook within a module

To install an instance of a regular hook from within a module, it is best
to use a local function (or, if the function must be global, use a name
different than the normal hook function name). Then, use the @{hookexecutor:InstallHook}
function to register the hook.

To install an entity vtable hook, use the same @{hookexecutor:InstallVtableHook}
function that is normally used.

Hooks are executed in the order they are defined, with the global
function being called last. If a module depends on being called in
a specific order, the order of the module "require" statements should
reflect the desired order that hook functions are called.

## Returning from a hook

Since multiple hook functions can potentially be called for the same event,
the return value for each function is important:

* For a regular hook function with no return value: 
Each hook should normally return with no value. This will allow the next
defined hook for the event to run. If a hook wants to prevent later
defined hooks from running (e.g. the hook fully implements the behavior
of the event), then it can return the value NO_CHAIN.

* For a regular hook function with a return value:
Each hook function should determine if the event should be handled
by that module, or left for the next module. If the hook function
is to process the event, then the appropriate value should be returned.
Otherwise, an empty return can be used to let the next hook function
(if any) handle the event.

* For an entity vtable hook function:
The same considerations apply for functions with expected return and
no return. An additional consideration is the original vtable
function. If the last hook executed issues an empty return, then
the original vtable function will automatically be called (this behavior
differs from 0.56a). So in most cases, explicitly calling the @{Entity:CallOrigVtable} function will
not be necessary. A hook can still prevent the original vtable function
from being called by either returning a valid return value, or NO_CHAIN.
