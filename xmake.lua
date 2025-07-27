add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

if is_plat("windows") then
    add_cxxflags("/utf-8")
    add_defines("NOMINMAX")
end

set_languages("cxx20")

includes(
    "src/video-encoder"
)