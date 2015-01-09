PLaZa
=====

Emanuele Faranda                                black.silver@hotmail.it

![Alt text](/../screenshots/images/in_action.png?raw=true)

Introduction
------------
PLaZa chat (abbr PLaZa) is intended to be a program to allow system local
GNU/Linux users to chat each other in a pleasant way.
Currently it is possible to use command like "write" or "wall" to send
messages to another logged user, but these utilities have the following
cons:
* An history of the previous messages is not hold
* Messages are not 'safe'; a logged out user won't receive them when
  he eventually logs in
* Interface is really minimal: your messages are messed up with the
  others'
* Wrapping these commands into a more sophisticated system is weird
  (e.g. wall is a blocking command and whould require 'expect' command
  hacks)
PLaZa suits well in a remote shell scenario where each user logs in as a
different system user. If a Secure Shell is being used and a remote PLaZa
client is run, it is effectively possible to exchange encrypted chat
messages between a group of people.

Features
--------
* ncurses clean interface with two main windows:
    * Message window : shows the incoming messages
    * Command window : interactive prompt which is used to compose
      messages
* Different ncurses colors to each of these groups:
    * Client title
    * Messages text
    * Messages author if is you
    * Messages author if is not you
    * Command prompt
* Easy tweaks and customizations (e.g. colors) can be applied modifying
  'settings.h' file
* Responsive GUI with cursor control interface
* Automatic and transparent new messages retrieving
* Latest messages filter (see settings.h for configuration)
* Automatic windows resize

Version II:
* Unicode characters support
* Full featured line editor

Requirements
------------
On a debian based system:
* Linux kernel >= 2.6.13 (for inotify) (tested on 3.16.0-4)
* libncurses5-dev (tested on 5.9+20140913-1)
* gcc (tested on 4.9.1)
* build-essential (tested on 11.7)

Build & Run
-----------
To build just use the canonical command `make`and then just `./plaza` to run.

To customize your name you can use `PLAZANICK=altnick ./plaza`

In order to get unicode support, be sure your system is using a utf-based
locale.

Shortcuts
---------
* Mouse wheel up / Page up : scrolls messages up
* Mouse wheel down / Pade down : scrolls messages down
* CTRL+C : cleans the prompt (it does NOT quit the program)
* ESC : quits the program
* Cursor control: moves the cursor back, forth, begin, end

Known Issues
------------
* Message scrolling is not perfect when multiline messages are shown
