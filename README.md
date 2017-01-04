# staticpendulum_gui
## What is this?
A pursuit in learning more about modern C++11/14, Qt/QML, QMAKE and CMAKE build tools, and organizing a real project with unit tests and external library dependencies. The primary goal is to both learn about and document through this repository a C++14 Qt application utilizing QML to create a rich GUI.

## What does it do?
Numeric integration of a static pendulum system to generate fractal maps and benchmark heavy floating point computations (see [staticpendulum](https://github.com/jbuckmccready/staticpendulum) for the original command line based physics project). It may evolve over time into a more general fractal image generator or systems integrator...

## How is it useful?
There are many excellent QML and Qt framework examples provided by The Qt Company, but it can be difficult to find much beyond "hello world" like examples. This is understandable given the breadth of different components available, but I wanted to pursue actually creating a fully fledged project. Creating an organized small to medium sized project that utilizes many different things from the Qt framework and QML serves as both an entertaining side project, and possibly a useful bit of documentation.

## Why C++, Qt and QML?
I have always had enthusiasm for C++, especially with the changes that have come with C++11/14, and after swimming around in the .NET C#/F#/XAML universe for some time I wanted to learn another full stack framework that was both portable and could more easily utilize C++. QML has been exceptionally fun to learn and surprisingly powerful, it captures both the scriptability of JavaScript and the organizability of a declarative language.

## That's nice... So what can I find in this repo?
* A C++ QMAKE project sub divided to organize tests and external dependencies (may switch to CMAKE in the future).
* A QML GUI that interacts with C++ objects for performance and non-GUI related code.
* Submodules used to control external dependencies (currently just googletest).
* Testing (and soon benchmarking) code utilizing googletest and Qt Test.
* Different Qt framework uses: QtConcurrent with QFutures (for parallel integration of points), QImage (for creating the fractals), QAbstractListModel (for creating C++ models to bind to QML), and Qt JSON (for reading/writing object data to JSON).
* Creating QML instantiable types from C++ and exposing C++ singletons to QML.
* Examples of Qt Quick Model-View-Delegate pattern, Qt Quick Controls 2, heavy use of QML property bindings, and some dynamic QML object creation in JavaScript.

## Sectional Documentation of the Project
WIP...
