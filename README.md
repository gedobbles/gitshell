# gitshell
A helper program for more efficient shell git use.

As I saw that there have been, according to the statistics, already 42 unique
people who cloned this repository, I want to take this as an invitation, to leave
a short message to those:

If you like my implementation of a git shell, don't hesitate to tell others and me.
I do really appreciate any feedback.
And furthermore, if you have *any* suggestions, ideas, questions or concerns
regarding it's future development, please leave a note in form of an issue.

Thank you.

## How to install
A simple `make install` compiles and installs **gitshell** to `/usr/local/bin/gitsh`.
For it depends on the readline library, you have to have `libreadline-dev` or similar installed on your system.
In order to copy the binary to `/usr/local/bin/` and make it executable via chmod +x, it asks for your sudo password.
See the [makefile](./makefile) for details.
You can uninstall it via `make uninstall`.

If you don't want it in `/usr/local/bin/`, just type `make` and do what you want.

## How to use
**gitshell** is very easy to use: Once installed, just type `gitsh` on your terminal and there you are.

To execute a git command, just type it out, if you haveforgotten one just hit *Enter* without having typed anything to see the git help.
You can scroll the history with the *up* and *down* arrow keys in a familiar way.

Be aware of the two different possibilities to change the working directory:
 * `cd` just changes the working directory, your history will still be stored in the directory you launched **gitshell** in.
 * `cdp` changes the working directory **and** saves your history and stores the future history in the directory you `cpd`ed in.

To execute a program rather than a git command, precede it with a colon, e.g. `:echo Hi`.


## Similar projects
Obviously I was not the first one to make an interactive shell for git:

* [git-sh][] - A customised bash shell with a Git prompt, aliases, and
  completion.
* [gitsh][] - A simple Git shell written in Perl.
* [repl][] - Wraps any program with subcommands in a REPL.

[git-sh]: https://github.com/rtomayko/git-sh
[gitsh]: https://github.com/caglar/gitsh
[repl]: https://github.com/defunkt/repl
