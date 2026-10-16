if status is-interactive
    # Commands to run in interactive sessions can go here
    glow  Desktop/my-obsedian-notes/my-state.md
end


# Append Cargo bin to PATH
fish_add_path --append ~/.cargo/bin
# Prepend local bin to PATH
fish_add_path --prepend ~/.local/bin

fish_add_path $HOME/.local/share/gem/ruby/3.3.0/bin

# Disable the welcome message
set -g fish_greeting

# Aliases
alias la="ls --all"
alias bat="batcat"

# Open DWM source with micro
alias cdwm="micro ~/suckless-software/dwm-btw/config.h"

# Build DWM source & refresh it
function mdwm
    # Save current directory so we can return to it seamlessly
    set -l current_dir (pwd)
    cd ~/suckless-software/dwm-btw
    sudo make clean install
    cd $current_dir
end

# Sound notifier based on command success or failure
function t
    # Run the arguments passed to 't' as a command
    if eval $argv
        # If it succeeds, play success sound
        paplay /usr/share/sounds/freedesktop/stereo/complete.oga
    else
        # If it fails, play error sound
        paplay /usr/share/sounds/freedesktop/stereo/bell.oga
    end
end
