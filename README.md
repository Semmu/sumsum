# S.U.M.S.U.M.
Super Unusual Medieval Strategic Ultimate Mission - a minimalist RTS written in C using the SDL library.
(the name is "funky" on purpose)


## About
This is a really minimalistic RTS which was my homework at the university in 2012. The objective is to win the match, controlling one (and then multiple) towns.

Every town has 3 attributes: number of resources, number of peasants, and number of soldiers. Peasants produce resources constantly (the more peasants, the more resources per second), peasants are created using resources, and soldiers are created using peasants and resources.

In the main screen you can type in your name, select color and configure the other variables of a match. See the screenshots for details.

The code itself was written more than 2 years ago, so it could use some refactoring love. But instead of refactoring this one, my plan is to create a version 2 of the game with more features, original artwork, network multiplayer, 3D graphics (maybe), etc. But these are just ideas and I'll only achieve it if I have magically recieve some freetime, so... there are no release dates... :)

## Screenshots
Here: https://www.dropbox.com/sh/abskmldbszi487r/AAA0yFpkeWjUoN1s2icdlV0Za?dl=0

## Install
sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev
make

## License
You can do whatever you want with the code, but the images are likely protected by law. (to be honest I used images from random places on the internet). So monetizing them are really prohibited.

Of course, if you are the owner of the original images, just ask me and I will delete them.
