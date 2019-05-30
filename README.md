# sy_chess_engine
A chess engine, largely in C with some C++ randomly dropped in, made many years ago while learning C/C++. The project that's always been in progress, in theory. 

I was following [this](https://www.youtube.com/playlist?list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg) tutorial very heavily, this was a fantastic resource, highly recommend it. Props to you 'Bluefever Software' if you ever happen to come across this for some reason. Looks like you're even still uploading code tutorials, good stuff

This project is probably the reason I feel at all comfortable with low level code and have a feeling for what a language is 'actually doing'. That plus the bit packing and random bit magic snippets to build and manipulate bitboards are pretty entertaining. This project is infact intended to be updated over time. One day, this *will* be 'finished'

I always thought of this project as being in three high level stages

* Represent the board and the rules
* Represent the position and possible moves
* Evaluate a given position

At the time, the intention was to continue following the guide, implement A/B trees, various pruning techniques etc., and implement a basic rule-based evaluation system. Given that I'm now studying machine learning, and based off of the success of DeepMind's AlphaZero and deep reinforcement learning in general, points 3 and maybe 2 also could be replaced with a basic RL system. I've got no real idea on ML/DL packages in C++, but obviously they exist in general. 

In terms of the state of the project, at the time of repo creation/initial pushing of the old project I'm fairly sure I had just successfully completed the [Perft testing](https://www.youtube.com/watch?v=ioaPTMKU3zg&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=44&t=0s)), i.e. completed point one, representation of the board. 

### Plans for this 

* Refactor the fuck out of it, and probably get it into a passable C++ state. I'll leave a stable version of the initial push as a separate branch, initstate. 
* Plug it into some kind of basic chess gui for better play testing, as opposed to the ASCII art ches board currently drawn...
* Then I'll look into bolting on some kind of RL framework. 


### Some cool links

Looking back at my very old bookmarks folder in relation to this, here's some cool links if you're interested. There may very well be far more up to date and relevant alternatives to these links, indeed a lot of my old links lead to dead pages. 

* [Micro-Max](http://home.hccnet.nl/h.g.muller/max-src2.html), a *tiny* chess engine
* [TSCP](http://tckerrigan.com/Chess/TSCP/), Decent educational example of a C engine
* [BTH](http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious), A guide to the random bit magic hacks used in this project and a bunch more. They're a good read
