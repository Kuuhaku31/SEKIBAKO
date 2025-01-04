
// debuger.h

#pragma once

class Debuger
{
public:
    static Debuger& Instance();

public:
    void ImGuiWin_Debug(bool* is_open = nullptr);

private:
    Debuger()                          = default;
    ~Debuger()                         = default;
    Debuger(const Debuger&)            = delete;
    Debuger& operator=(const Debuger&) = delete;

    static Debuger* instance;
};
