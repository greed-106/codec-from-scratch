add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_encodings("utf-8")
if is_plat("windows") then
    add_defines("NOMINMAX")
end

set_languages("cxx20")

add_includedirs("include", {public = true})

includes(
    "src/video-encoder",
    "src/jpeg-encoder"
)