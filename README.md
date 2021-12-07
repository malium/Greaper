# Greaper

Greaper is an application framework, which enables the creation of these types of applications, real-time applications or event-based applications (In progress). This framework provides from the basic OS abstractions to a full modular approach to application creation.
The main focus of Greaper is the hability to create an application from different modules and enable the interchange of modules at runtime. 
Greaper currently supports Windows using 64bits, 32bits needs testing, and Linux based OS but needs testing too. Mac and another Apple platforms won't be supported.

## Background

I've been a programmer for long time now (about 11 years), I started with programming PICAXE microcontrollers in BASIC and later in C. Then I moved to graphics and game programming in Java and later in C++ and currently I'm developing a Game using Unity therefore C#.
What I mean by explainig my background is that I've touched from very low-level stuff to a fairly high-level, meaning that I can understand the concernings from a Low-level programmer to the concernings from a fairly high-level programmer, and I think that this is implemented in Greaper.

## What is Greaper?

I've been merging all my experince into a library and that how Greaper started! At the beginning was my own toy graphics engine, but it grew probably too much. That's why I say that Greaper is not an Engine because the main target is any type of application but real-time graphics applications will have support first but it will not end there.

## Why so much modularity?

Well you program applications that their scope is big, you need to try its components alone and test different implementations of a component, having an extensive modular framework allows easier testing and easier adaptation to different target platforms.

## Current development status

Right now as I stated, I'm working on a game, also I'm finishing my studies and also I have another job. How can manage all of that? Neither I know.
Will you support Greaper then? Yes, since the firsts stages is just port and adapt my old work to this framework.

## What is included? (Features)

Empty checkboxes represent in progress features.

* [X] Set of tools that provide easy access to information about the basic types, and specialized functions.
* [X] PHAL, a Platform and Hardware Abstraction Layer, a set of tools that detects the Compiler, OS and information about the hardware and provide easy ways of branching the code if the target platform or hardware supports it.
* [X] Easy way of handling Assertions (Verify macros).
* [X] Set of preprocessor definitions in order to solve different platform challenges in a single statement.
* [X] Abstraction of the typical multithread classes and utilities with more debug information, for example, knowing what thread are colliding and report the collision.
* [X] A library protocol that will exchange information about types, properties and interfaces with the main application in order to enable full modularity.
* [X] An interface to handle crashes of the application.
* [X] CommandSystem, that will manage a console and a way to send commands and handle them.
* [X] Two types of Event handling, an instant one, class contains an Event object that manages a list of listeners, and a (TODO)deferred one, theres a system that sends the triggered events to different listeners which subscribe to an EventID.
* [X] DeferredCall Manager that let's you make a call deferred in time (or frames on a realtime application).
* [ ] VirtualFileSystem abstraction that enables the addition of virtual filesystems (like a container, zip, 7z...).
* [ ] TaskManager that handles small tasks distributed in a number of background threads.
* [X] WorkerThread spawner in order to enable background long tasks.
* [ ] ReflectionManager that will store all the type definitions even from the plugins or external libs, via manual reflection.
* [ ] SerializationManager that using the ReflectionManager will de/serialize types into different formats.
* [X] TimeManager that provide time information and frame information for realtime applications.
* [X] WindowManager providing multiple window management and enabling input pooling and rendering.
* [ ] InputManager that will hook to the created windows enabling input pooling and manipulation.
* [ ] ResourceManager that will handle resources, its IE (Import/Export) methods, its resource links, unique IDs and can use SerializationManager to import/export complex resources or use IE to import/export raw resources.
* [X] Properties, a class that will store data like a database but can be easily extended giving own types, also they are (TODO)de/serialized automatically if their configuration allows it as configuration for the application and its libraries.