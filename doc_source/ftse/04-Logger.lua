--- logger is used to log information to the FTSE.log file for debugging Lua scripts.
-- The class is defined by a single global object named "logger". The global object is available for any Lua script function to use.
-- @classmod logger

--- log writes the given string to the FTSE.log file.  Log messages are automatically timestamped.
-- @param msg The log message to write to the log file.
-- @return None.
function logger:log(msg)
end
