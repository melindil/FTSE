$compress = @{
LiteralPath= "FTSESetup.exe", "FTSESetup.exe.config", "Newtonsoft.Json.dll"
CompressionLevel = "Fastest"
DestinationPath = $args[0]
}
Compress-Archive @compress