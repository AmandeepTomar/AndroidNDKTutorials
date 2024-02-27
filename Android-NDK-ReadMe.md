# Android NDK (Native Development Kit)

### NDK can be useful

**NDK:** -> A set of tools that allow you to c/c++ code on Android.
**CMake:** -> An External build tool that works alongside with gradle to build your native
libraries.You do not need this component if you only plan to use ndk-build.
**LLDB** -> Debugger Android studio used to debug native code.

These all need to installed from android SDK manager. this one is the initial step to create an app
that support NDK.

1. Squeeze extra performance out of a device to run computationally intensive applications, such as
   games or physics simulations.
2. Share code between other devices , written in c or c++

### Components

`Native shared libraries:` The NDK builds these libraries, or `.so` files, from your C/C++ source
code.

`Native static libraries:` The NDK can also build static libraries, or` .a` files, which you can
link
into other libraries.

`Java Native Interface (JNI):` the Java and C++ components talk to one another.

`Application Binary Interface (ABI):`  The ABI defines exactly how your app's machine code is
expected to interact with the system at runtime.

`Manifest` If you are writing an app with no Java component to it, you must declare the
NativeActivity class in the manifest.

#### Q. Why we need NDK to use audio processing in android App?

We can use java and kotlin to process the audio, but if the processing is less and do not heavy
operations that it would works fine.
Coz we know that audio processing will be fast if we able to communicate to the audio driver
directly, so call the function at machine level , C and C++ comes in picture. it work most
efficiently if we compile it directly on machine code.

### Create the `CmakeLists.txt` file

- this file has some configurations like minimum cmake version required , project name ,
  add_library,
  target_link_libraries

```
# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

project name that can be accessable by CMAKE_PROJECT_NAME and PROJECT_NAME
project("androidndktutorials")

add_library(${CMAKE_PROJECT_NAME} SHARED
    # List C/C++ source files with relative paths to this CMakeLists.txt.
    androidndktutorials.cpp)
    
// All the library that will be required to target library will be added here. 
target_link_libraries(${CMAKE_PROJECT_NAME}
    # List libraries link to the target library
    android
    log)    

```

- I want to say where the target finds its header files. I use `target_include_directories` to pull
  in
  the folder with all of its headers and denote these as `PUBLIC headers`.
- we tell CMake to make `helloworld-c` depend on `native_app_glue`, `android`, and the library we
  called
  `log-lib` using `target_link_libraries`.

#### How can be call C++ code from kotlin.

1. `external ` Function in kotlin
2. `native` Function in java
2. JNI, Java Native interface

Steps

1. Define the designed function in kotlin module as `external function` or `navtive in java`.
2. Create a c++ source file.
3. Write the definitions of the previously declared function in C++ using appropriate function
   signature defined by the JNI.
4. create a build script for the C++ file.
5. Adjust the build.gradle script file
6. load the c++ shared library statically in kotlin.

## Supported Build tools

1. Android Studio's default build tool to compile native libraries is `CMake.`
2. Android Studio also supports `ndk-build` due to the large number of existing projects that use
   the
   build toolkit.
3. recommended is CMake build tool to compile native libraries.

### `#include<Jni.h>` it will provide all the sets of tools that we need to communicate to java and kotlin from C++.

#### C++ file Important Parts

1. ### What is does

```
extern "C"{
}

```

This will provide c linkage specification.
this allow us to write the code inside in C++ and that will compile in that way that it will be
compatible with C and call with the Jni.
extern "C"{}, we’re just telling the linker that we need to treat everything between those curly
braces as C. You can still write C++ code inside of there, but the functions will look like C
functions to the rest of our program.

1. `JniEnv *env` -> Pointer of the JniEnvironment.
2. `JObject` -> access of the object that call that method.

## #pragma once

It means it included only one.
It is used to create the header file in C++ code and that will be written inside the `include`
package folder. inside `app/main/cpp/include`.

#### In AndroidManifest.xml need to tell where to find the code that we want to run.

```xml

<activity android:configChanges="orientation|keyboardHidden" android:label="@string/app_name"
    android:name="android.app.NativeActivity">
    <meta-data android:name="android.app.lib_name" android:value="helloworld-c" />
</activity>
```

### "JavaVM" and "JNIEnv"

- JNI defines two key data structures, "JavaVM" and "JNIEnv". Both of these are essentially pointers
  to pointers to function tables.
- `The JavaVM` provides the `invocation interface` functions, which allow you to create and destroy
  a JavaVM. In theory you can have multiple JavaVMs per process, but Android only allows one.
- `The JNIEnv` provides most of the JNI functions. Your native functions all receive a JNIEnv as the
  first argument,The JNIEnv is used for thread-local storage. For this reason, you cannot share a
  JNIEnv between threads.

### jclass, jmethodID, and jfieldID

- Get the class object reference for the class with `FindClass`
- Get the field ID for the field with `GetFieldID`
- Get the contents of the field with something appropriate, such as `GetIntField`

### Local and global references

KeyWords

- `jObject , jclass, jstring , jarray`
- `NewGlobalRef , NewWeakGlobalRef and DeleteGlobalRef`
- Every argument passed to a native method, and almost every object returned by a JNI function is
  a `"local reference"`.
- Even if the object itself continues to live on after the native method returns, the reference is
  not valid.
- `jobject, including jclass, jstring, and jarray`.
- The only way to get non-local references is via the functions `NewGlobalRef and NewWeakGlobalRef`
- If you want to hold on to a reference for a longer period, you must use a "global" reference. The
  `NewGlobalRef` function takes the local reference as an argument and returns a global one. The
  global reference is guaranteed to be valid until you call `DeleteGlobalRef`.
- To see if two references refer to the same object, you must use the `IsSameObject function`
- Never compare references with `==` in native code.
- Do not use `jobject` values as keys

```kotlin
jclass localClass = env->FindClass("MyClass");
jclass globalClass = reinterpret_cast < jclass >(env->NewGlobalRef(localClass));
```

### Primitive arrays

- JNI provides functions for accessing the contents of array objects. While arrays of objects must
  be accessed one entry at a time, arrays of primitives can be read and written directly as if they
  were declared in C.

- The `Get<PrimitiveType>ArrayElements` family of calls allows the runtime to either return a
  pointer to
  the actual elements, or allocate some memory and make a copy.

### 0

Actual: the array object is un-pinned.
Copy: data is copied back. The buffer with the copy is freed.

### JNI_COMMIT

Actual: does nothing.
Copy: data is copied back. The buffer with the copy is not freed.

### JNI_ABORT

Actual: the array object is un-pinned. Earlier writes are not aborted.
Copy: the buffer with the copy is freed; any changes to it are lost.

- `Get<Type>ArrayElements and GetStringChars` that may be very helpful when all you want to do is
  copy
  data in or out

```kotlin
jbyte * data = env->GetByteArrayElements(array, NULL);
if (data != NULL) {
    memcpy(buffer, data, len);
    env->ReleaseByteArrayElements(array, data, JNI_ABORT);
}

// achieve the same 

env->GetByteArrayRegion(array, 0, len, buffer);

```

- you can use the `Set<Type>ArrayRegion` call to copy data into an array, and `GetStringRegion or
  GetStringUTFRegion` to copy characters out of a `String`.

### Exceptions

- `You must not call most JNI functions while an exception is pending`. Your code is expected to
  notice the exception (via the function's return value,` ExceptionCheck, or ExceptionOccurred`) and
  return, or clear the exception and handle it.
- The only JNI functions that you are allowed to call while an exception is pending are:

``` 
    - DeleteGlobalRef
    - DeleteLocalRef
    - DeleteWeakGlobalRef
    - ExceptionCheck
    - ExceptionClear
    - ExceptionDescribe
    - ExceptionOccurred
    - MonitorExit
    - PopLocalFrame
    - PushLocalFrame
    - Release<PrimitiveType>ArrayElements
    - ReleasePrimitiveArrayCritical
    - ReleaseStringChars
    - ReleaseStringCritical
    - ReleaseStringUTFChars
```

- if you call a method `(using a function like CallObjectMethod)`, `you must always check for an
  exception`, because the return value is not going to be valid if an exception was thrown.
- The `JNI Throw and ThrowNew` instructions just set an `exception pointer in the current thread`.
- Native code can "catch" an exception by calling `ExceptionCheck or ExceptionOccurred`, and clear
  it
  ` with ExceptionClear`. As usual, discarding exceptions without handling them can lead to
  problems.

#### CMake

- The CMake toolchain file
    - The NDK support CMake via toolchain file. it customize the behaviour of the toolchain for
      cross compiling.
    - located at `<NDK>/build/cmake/android.toolchain.cmake`

### How to add the .so library in android.

- we can add multiple `add_library()` in CMakeLists.txt files, if we have more than one library to
  add.

Integrating and using a `.so library (shared library)` in an Android application involves two main
steps: adding the library to your project and using it through the Java Native Interface (JNI).
Here's an example covering both aspects:

1. `Adding the .so library:`

`Create the jniLibs folder:` In your Android project's root directory, create a folder named
jniLibs.
This folder will hold your .so libraries for different architectures.
Place the .so files: Inside the jniLibs folder, create subfolders corresponding to the supported
architectures (e.g., armeabi-v7a, arm64-v8a, x86). Put the corresponding .so file for each
architecture in its respective subfolder.

### Difference between ARMEABI-v7a and arm64-8a

ARM architecture:

armeabi-v7a: Targets ARMv7-A architecture. `This architecture supports 32-bit instructions and is
commonly found in older Android devices`, including many mid-range and low-end smartphones and
tablets.
arm64-v8a: Targets ARMv8-A architecture `with 64-bit instructions. `This architecture offers
improved
performance, better support for modern features, and compatibility with 32-bit code through ARM's
AArch32 execution state. It's commonly found in newer Android devices, particularly flagship
smartphones and tablets.

2. `Using the library with JNI:`

`Create a JNI wrapper class:` This class will act as a bridge between your Java code and the native
code in the .so library. The wrapper class will have methods that map to the functions you want to
call from the library.
Example: (Assuming your library is named mylib.so and has a function named add that takes two
integers and returns their sum)

1. `jniLibs Folder Structure:`

jniLibs/
armeabi-v7a/
libmylib.so
arm64-v8a/
libmylib.so
x86/
libmylib.so

2. Java wrapper class (MyJniWrapper.java):

```java
public class MyJniWrapper {

    static {
        System.loadLibrary("mylib"); // Load the library based on the system architecture
    }

    public native int add(int a, int b); // Declare a native method that maps to the 'add' function in the library

}
```

Java

Use code with caution.

3. Using the library in your code:

```java

MyJniWrapper jniWrapper=new MyJniWrapper();
        int result=jniWrapper.add(5,3);
        System.out.println("Result: "+result);
```

## What is use of JNI_OnLoad()

- this function is called by dalvik of Vms as the code get executed , it will use to perform the any
  configuration task that we need to0 execute as the libs get loaded.
- Example we have mapping of methods in JNI with java and c++ so we can call that table of mapping
  function in `JNI_ONLoad(JavaVM *vm , void *reserved)`

### Examples

```java


#include <jni.h>

extern "C" JNIEXPORT jintArray JNICALL
Java_com_example_SignalProcessor_processSignal(JNIEnv *env, jobject thiz, jintArray inputSignal) {
    jint *inputArray = env->GetIntArrayElements(inputSignal, NULL);
    // Perform signal processing tasks using inputArray
    
    // For demonstration purposes, let's just square each element
    int length = env->GetArrayLength(inputSignal);
    jintArray outputSignal = env->NewIntArray(length);
    jint *outputArray = env->GetIntArrayElements(outputSignal, NULL);
    for (int i = 0; i < length; ++i) {
        outputArray[i] = inputArray[i] * inputArray[i];
    }
    
    env->ReleaseIntArrayElements(inputSignal, inputArray, 0);
    env->ReleaseIntArrayElements(outputSignal, outputArray, 0);
    
    return outputSignal;
}

```


