# AndroidUtils
Utils for easy c++ and qml integration of common android features

## Features
- Utilizes [androidnative.pri](https://github.com/benlau/androidnative.pri)
	- Adds qmake wrappers and additional code to make integration easier
	- Automatically registers androidnative when registering AndroidUtils
- Adds a singleton for c++/qml
	- Show a toast
	- Trigger haptic feedback
	- Change the statusbar color

## Installation
The package is providet as qpm package, [`de.skycoder42.androidutils`](https://www.qpm.io/packages/de.skycoder42.androidutils/index.html). To install:

1. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing))
2. In your projects root directory, run `qpm install de.skycoder42.androidutils`
3. Include qpm to your project by adding `include(vendor/vendor.pri)` to your `.pro` file
4. Add the line `apply from: "androidutils.gradle"` at the very bottom of your `build.gradle` file
	- That file is located in your android source directory, the directory with your manifest (See [`Demo/android`](Demo/android))
	- If you don't have the folder, you can let QtCreator create one for you. Go to `Projects > [Your Kit] Build > Create Android APK` and create the templates by pressing `Create Templates`

Check their [GitHub - Usage for App Developers](https://github.com/Cutehacks/qpm/blob/master/README.md#usage-for-app-developers) to learn more about qpm.

## Usage
Just include/import (`import de.skycoder42.androidutils 1.0`) the class and use the methods. All the setup and registrations are done automatically.

### JNI_OnLoad
By default, AndroidUtils defines the `JNI_OnLoad` for you. However, if you need to define this method yourself, this will create a conflict! To prevent AndroidUtils from creating the method, add the following line **before** including the vendor.pri

```pro
CONFIG += noJniOnLoad
include(vendor/vendor.pri)
# ...
```

In that case, make shure you initialize androidnative.pri by adding `AndroidNative::SystemDispatcher::registerNatives();` to your `JNI_onLoad` implementation!

### QPM_ROOT
This variable should contain the root directory of qpm, i.e. the `vendor` directory. By default (if you don't set the variable), it is set to `$$_PRO_FILE_PWD_/vendor`. If your qpm vendor folder is not located in the same directory as your pro file, set this variable to the **absolute** path of your vendor folder. If you use the `_PRO_FILE_PWD_`, you can simply make the path relative to this directory, e.g. `QPM_ROOT = $$_PRO_FILE_PWD_/../vendor`.

### BUILD_ROOT
If the project with the android code is part of a subdirs project, i.e. **not** the root project, you need to set `BUILD_PATH` as well. It needs to point to the root build directory. By default this is `OUT_PWD`. If your project is a subdirectory, adjust that path to match the build root. Assuming your android project is a sub project of the root project, you have to set it to: `BUILD_ROOT = $$OUT_PWD/..`