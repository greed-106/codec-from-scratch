target("video-encoder")
    set_kind("binary")
    add_includedirs(path.join(os.projectdir(), "include", "core"), {public = true})
    add_files("*.cpp")