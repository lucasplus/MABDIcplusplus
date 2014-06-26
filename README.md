# AllProjects 

On this page:

  * [What is this?](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-what-is-this)
  * [Build Notes](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-build-notes)
    * [Required software and source libraries](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-required-software-and-source-libraries)
    * [Generating documentation from source](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-generating-documentation-from-source)
    * [Additional notes](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-additional-notes)
  * [Notes](https://bitbucket.org/lucasplus/allprojects/overview/#markdown-header-notes)

## What is this? 

Repository for all development towards my final thesis code. 

## Build notes  

MABDI uses [CMake](http://www.cmake.org/) as the build process generator. If you
are unfamiliar with CMake you should do some quick tutorials, but the idea is
that you write a script for a process that looks at your source code and
generates a build solution. By build solution, I mean a project that can be
compiled and ran. For example, if you are on Windows you will use CMake to
build a Visual Studio project of if you are on a Mac you can generate an Xcode
project or a Makefile. This can be very useful, for example, if you want to get
the new Visual Studio version. The idea is that you change your CMake generator
(which compiler to use) and your source code stays the same. 

The complexity of the CMake system varies largely from project to project.
MABDI's current system is very simple. It is meant to look for all the required
third party libraries, generate the targets, and appropriatly link the targets
to the libraries they may need. 

Folder Structure:

    Build - Only in .gitignore. User makes this folder. Use as a build area.
    Src - My code for MABDI
    Tests - Code to test ideas
    Utilities - Has cmake scripts

### Third party libraries 

A note about configuring, building, and installing third party libraries:
Accomplishing this can be a true headache. This is because sometimes there are build
errors and you have no idea why. Or, especially if you are new to CMake, you
don't know how to configure a build. Also, sometimes you don't know what to
configure. This is normal. You can do it. The key is to step back, be calm,
perform a well thought google search, read what you feel is relevant (make sure
not to make the mistake of trying to uderstand everything), and then try things.
That is the best thing you can do. Try different things. Don't get stuck and
fustrated.  Keep track of your steps, and you'll be able to go back if
something really doesn't work.

MABDI makes use of several third party libraries. I choose to do this for the
whole "standing on the shoulders of giants" philosophy. It is assumed that you
have built and installed the needed third party libraries before running CMake
on MABDI. Current list of required libraries:

* CMake 2.8.12 - Build generator 
* Qt 5.1 - GUIs
* OpenGL- Graphics rendering
* PCL 1.7.1 (Point Cloud Library) - Generating mesh from point sets
  * Boost 1.55.0 - needed by many things including MABDI code
  * Eigen 3.2.1 - Template library for matrix definition and methods
  * Flann 1.8.4 - A library for performing fast approximate nearest neighbor searches.
  * Qhull 2012.1 - Used in pcl surface
  * Qt 4.8.5 - A gui development framework. Also used by Mabdi
    * ICU 52 - Library providing unicode and globalization support for applications. 
  * VTK 5.10.0 - Visualization library 

* SDL version 2 - For joystick control
* Doxygen - Documentation generator (optional)
* LaTex - Needed by doxygen (optional)
* Graphviz - Needed by doxygen (optional)

We are going to prefer dynamic link libraries because that seems to be the
usual default when installing third party libraries. We will also prefer 32 bit
builds of libraries to avoid incompatibilities.

#### Installing third party libraries on Windows

Here are some links and notes about how to build and install the third party
libraries on windows. Specifically, my platform is Windows 7 and I am building
for 32-bit Visual Studio 2012. 

Big picture notes: It is up to you where you want to place the third party
libraries. I chose to install them all in a folder named `C:\3rdparty\` This
allowed me to think of all my tools being near the base of my file tree so that
I could easily incorporate them into new projects that I would have elsewhere.
TODO: Also, I added the binary folders of the libraries to my path, so this
kept the path names short.

Building each third party library can be quite different. In general, I followed these steps: 

1. Downloaded a .zip file of the latest stable version of the library to some
   folder in my work area. Then unzip the files. 
2. Ran CMake on the library and configured the project for my needs. Here is
   where you define static or dynamic, debug or release, paths to other needed
   thirdparty libraries, where to build the binaries, and other build
   properties. 
3. Moved the compiled library to folder with all third party libraries. Note,
   this does not mean all of the source code. Just the output, which can vary
   for each project, but often it's the contents of the folder where you told
    CMake to build the binaries. 

Once again, the above is a very general guideline, below are some library
specific notes. Not every library is mentioned, just the ones that I have some
notes on.

Most of the libraries I needed to build were for the Point Cloud Library (PCL).
I mostly followed [this pcl
tutorial](http://pointclouds.org/documentation/tutorials/compiling_pcl_dependencies_windows.php)
in order to install the pcl specific library dependencies.

Notes on my environment:

  * Installed [Python 2.7](https://www.python.org/download/releases/)
  * Installed [Perl](http://strawberryperl.com/)
  * Installed [Ruby](http://www.rubyinstaller.org/)
  * Installed [jom](http://qt-project.org/wiki/jom)
  * Compiled and installed [ICU 52](http://site.icu-project.org/download)
  * Python, Perl, and Ruby I installed by using the installer provided by the project. This created the folders in my `C:\` directory and added the paths to the binaries to my PATH variable.
  * I installed jom by unzipping the folder and adding the path to my PATH variable.

Notes on version choice. I would have prefered to use Qt 5 rather than Qt 4.
This turned out to be a problem because I also wanted a full PCL build. PCL
1.7.0 has a problem with the VTK 6. I then switched to VTK 5 which then
required me to switch to Qt 4. 

1. Boost 1.55.0

  * Boost is needed by both PCL and MABDI. Boost can be thought of as a set of generally useful tools for C++. I am using version 1.55.
  * I followed the [Boost installation instructions](http://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html)
  * I set the toolset (toolset=msvc) and left the rest as default. Run b2 --help to see default settings. This creates a dynamic library to be linked to.

2. Qt 4.8.5

  * Qt is needed by PCL, VTK, and MABDI.
  * Go to the [Qt download page](http://qt-project.org/downloads). If your platform is supported, the easiest thing to do is to download the installer for your platform. This will install the binaries with no compiling necessary. If you are on Visual Studio, you will need the OpenGL supported build (not Angle) this is needed because of both VTK and MABDI. 
  * For me, I had to build the Qt project from source. Be warned, I had quite a few problems and also the build takes a few hours.
  * For the most part I followed [this write up from Qt](http://qt-project.org/wiki/Building_Qt_5_from_Git)
  * I had to configure my environment for this install, so see "Notes on my environment" above.
  * `configure -opensource -confirm-license -fast -debug-and-release -platform win32-msvc2012 -mp -opengl desktop -no-qt3support -no-xmlpatterns -no-multimedia -no-phonon -no-accessibility -no-openvg -no-webkit -no-script -no-scripttools -no-dbus -no-declarative`
  * Then run `nmake`

### Generating documentation from source 

My CMake system automates documentation generation from the source. It uses
CMake's add_custom_target() to create a target who will call upon the Doxygen
executable to generate the documentation in the build directory. All you have
to do is build the MakeDoc target. 

There are two ways to view the documentation:

* Html - open Doc/html/index.html in your favorite browser
* Pdf - If on unix machine: run make on Doc/latex/Makefile. If on windows: run
  Doc/latex/make.bat. After this, you can open the generated refman.pdf. 

### Additional notes 

* lib/x64 of SDL2 had to be manually deleted. CMake insisted on finding the 64
  bit library instead of the 32. 
* CMAKE_PREFIX_PATH is hard coded for my particular setup 

## Notes 

My memory is poor and if I don't write down all the ideas and facts swimming in
my head it bogs down the main process a terrible amount. Result: many text
files with a bunch of simple notes. Here is one for MABDI.

### Build Strategy 

I will be using a PIMPL Idiom, also known as a private implementation
technique, to write the classes in my code. This technique is designed for
large code databases, using it for MABDI is a bit overkill but will be good
practice of a technique in use by most companies. 

In essence, the implementation is done completely in the body of the class
using member data containers of a single object that is defined within the
body. That single class is the implementation, usually named Impl, and in the
header of the class we just need to forward declare a pointer to the object. We
can use different types of pointers:

  * Use unique_ptr when you want a single pointer to an object that will be
    reclaimed when that single pointer is destroyed.  
  * Use shared_ptr when you want multiple pointers to the same resource.

Change of plan: Running out of time. Would of liked to use PIMPL patterns. The new goal
is: as simplistic as possible. Classes will be no fancier than needed.

### Doxygen 

Functionality I should make use of:

* \\todo command will generate a todo list for me
* \\test command adds to tests in test list
* Expert->Dot->DOT_PATH is a field to find the dot executable. For when it can't be found on PATH
* Expert->LaTeX->LATEX_SOURCE_CODE is a flag to include source code. 
* Expert->Input->IMAGE_PATH is a path for images I specify with the \image command
* Expert->Input->EXAMPLE_PATH is a path for example code fragments that are included with the \include command 
* REPEAT_BRIEF - Don't repeat brief in detailed description

State of my current configuration:

* BRIEF_MEMBER_DESC - is on - will include brief member descriptions after the members that are listed in the file
* REPEAT_BRIEF - no
* JAVADOC_AUTOBRIEF - is yes 
* HIDE_UNDOC_MEMBERS - no 

### SDL2 map to xbox controller 

Axes map:

[ LSJSH LSJSV RSJSH RSJSV LT RT ]

  * Read LSJSH as "Left side joystick horizontal"
  * T stands for trigger.
  * Joystick axes values range from -32768 to 32767. 
  * Right and down are positive for the joystick axes.
  * Triggers are -32768 until pressed, then headed to positive. 

Buttons:

  * 0: up
  * 1: down
  * 2: left
  * 3: right
  * 4: start
  * 5: back
  * 6: left joystick press down
  * 7: right joystick press down
  * 8: left bumper 
  * 9: right bumper
  * 10: A
  * 11: B
  * 12: X
  * 13: Y
  * 14: X button
