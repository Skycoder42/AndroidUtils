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
- Adds the `ContentDevice` class (android only)
	- QIODevice class for android contents
	- Allows you to open genric `content` uris
	- This includes local files, as well as data from services like Dropbox, Google Drive, etc.
- Adds the `FileChooser` class
	- The Android equivalent of a file dialog
	- Shows an android file chooser activity to get/create a content uri
	- Provides `FileDialog` qml type that provides a file dialog for all platforms
		- On Android, uses the FileChooser
		- On other Platforms, the [Qt labs FileDialog](https://doc.qt.io/qt-5/qml-qt-labs-platform-filedialog.html) is used

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

## Android URI permissions
To open generic content uris, you need to get permissions for those. For local files, this are the normal read/write external storage permissions. For other content, i.e. from Dropbox, those permissions are granted for every uri. Those permissions will stay as long as your application is running, but if you want to access them later, e.g. after a restart, you need to persist the permissions. The FileChooser can do this for you, by setting the required `chooserFlags`. If you get Security exceptions when trying to read/write a file, this is what you need to adjust.