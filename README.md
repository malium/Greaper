# Greaper

## What is Greaper?

Greaper is a super modular application framework, with cross-platform between Windows and Linux.
This framework enables the creation of these types of applications, real-time applications or event-based applications. Provides utilities from the basic OS abstractions to a full modular approach to application creation.
The main focus of Greaper is the ability to create an application from different modules, these modules can be interchanged at runtime.
Greaper currently supports Windows using 64bits, 32bits (needs testing), and Linux based OS. Mac and another Apple platforms won't be supported.

## Why so much modularity?

Well you program applications that their scope is big, you need to try its components alone and test different implementations of a component, having an extensive modular framework allows easier testing and easier adaptation to different target platforms.

## Background

I've been a programmer for long time now (about 12 years), I started with programming PICAXE microcontrollers in BASIC and later in C. Then I moved to graphics and game programming in Java and later in C++ and currently I'm developing a Game using Unity therefore C#.
What I mean by explainig my background is that I've touched from very low-level stuff to a fairly high-level, meaning that I can understand the concernings from a Low-level programmer to the concernings from a fairly high-level programmer, and I think that this is implemented in Greaper.
I've been merging all my experince into a library and thats how Greaper started! At the beginning was my own toy graphics engine, but it grew probably too much.

## Current development status

Currently Greaper is very unstable, as I'm constantly pushing updates that can change the whole project, this will slowly be reduced as Greaper gets more mature. When the project reaches a certain stability, a development branch will be created and, the main branch will only have each release version changes.

## What is included? (Features)

Empty checkboxes represent in progress features.

* [X] Set of tools that provide easy access to information about the basic types, and specialized functions.
* [X] PHAL, a Platform and Hardware Abstraction Layer, a set of tools that detects the Compiler, OS and information about the hardware and provide easy ways of branching the code if the target platform or hardware supports it.
* [X] Easy way of handling Assertions (Verify macros).
* [X] Set of preprocessor definitions in order to solve different platform challenges in a single statement.
* [X] Abstraction of the typical multithread classes and utilities with more debug information, for example, knowing what thread are colliding and report the collision.
* [X] A library protocol that will exchange information about types, properties and interfaces with the main application in order to enable full modularity.
* [X] A native way (only Windows currently) to handle crashes and report a minidump and stacktrace information.
* [X] A CommandManager, from which launch registerd commands with input parameters, an external console or other Managers can call those commands.
* [X] Two types of Event handling, an instant one, class contains an Event object that manages a list of listeners, and a (TODO)deferred one, theres a system that sends the triggered events to different listeners which subscribe to an EventID.
* [ ] VirtualFileSystem abstraction that enables the addition of virtual filesystems (like a container, zip, 7z...).
* [X] TaskManager that handles small tasks distributed in a number of background threads.
* [X] A reflection system, all types, whether they are complex or just as simple as an int, can be reflected to gain information about them and serialize them into raw binary data, JSON, String.
* [ ] WindowManager providing multiple window management and enabling input pooling and rendering.
* [ ] InputManager that will hook to the created windows enabling input pooling and manipulation.
* [ ] ResourceManager that will handle resources, its IE (Import/Export) methods, its resource links, unique IDs and can use SerializationManager to import/export complex resources or use IE to import/export raw resources.
* [X] Properties, a class that will store data like a database but can be easily extended giving own types, also they are de/serialized automatically if their configuration allows it as configuration for the application and its libraries.
* [X] Application, a class that handles the loading of the different Managers and Libraries, being the main part of the program.
* [X] A LogManager that can log messages to multiple LogWriters asyc or serialy.

## License

I want Greaper to be open source, at least the whole application framework, so any one can make applications on top of it and custom modules.
In the future I'll make some modules that I'll not share their implementations but I will share their interfaces so they can be implemented by other people or even extended.

## Wanna collaborate?

I would be really thankfull if that's your willing! Greaper is a huge project and can scare a lot of developers to start, but I will inquire you, once is stable, to develop a module and test it!
I urge you to follow this [Code Style](CodeStyle.md) if you want to collaborate on the Greaper project.

## Contact

Have you found a bug? open a ticket on github or message me throgh here I'll also have my twitter were I sometimes answer technical stuff, were I'm open for DMs.
