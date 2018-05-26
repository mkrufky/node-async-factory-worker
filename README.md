# async-factory-worker

Asyncronously construct arbitrary C++ objects and classes even without a public copy constructor

[![NPM](https://nodei.co/npm/async-factory-worker.png?downloads=true&downloadRank=true)](https://nodei.co/npm/async-factory-worker/)

[![Build Status](https://api.travis-ci.org/mkrufky/node-async-factory-worker.svg?branch=master)](http://travis-ci.org/mkrufky/node-async-factory-worker)
[![Build status](https://ci.appveyor.com/api/projects/status/p5cgmixxpftd8hxa?svg=true)](https://ci.appveyor.com/project/mkrufky/node-async-factory-worker)

async-factory-worker is a header package for building c++ native addons for node.js that is meant to complement **[nan](https://github.com/nodejs/nan)**

<a name="usage"></a>

## Usage

Simply add **nan** and **async-factory-worker** as dependencies in the *package.json* of your Node addon:

``` bash
$ npm install --save nan
$ npm install --save async-factory-worker
```

Pull in the paths to **nan** and **async-factory-worker** in your *binding.gyp* so that you can use `#include <async_factory_worker.h>` in your *.cpp* files:

``` python
"include_dirs" : [
    "<!(node -e \"require('nan')\")",
    "<!(node -e \"require('async-factory-worker')\")"
]
```

This works like a `-I<path-to-async-factory-worker>` when compiling your addon.

<a name="api"></a>

## API

`AsyncFactoryWorker` is an _abstract_ class that you can subclass to have much of the annoying asynchronous queuing and handling taken care of for you. It can even store arbitrary V8 objects for you and have them persist while the asynchronous work is in progress.

This class internally handles the details of creating an [`AsyncResource`](https://github.com/nodejs/nan/tree/master/doc/node_misc.md#AsyncResource), and running the callback in the
correct async context. To be able to identify the async resources created by this class in async-hooks, provide a
`resource_name` to the constructor. It is recommended that the module name be used as a prefix to the `resource_name` to avoid
collisions in the names. For more details see [`AsyncResource`](https://github.com/nodejs/nan/tree/master/doc/node_misc.md#AsyncResource) documentation.  The `resource_name` needs to stay valid for the lifetime of the worker instance.

`AsyncFactoryWorker` is an _abstract_ class template that extends [`Nan::AsyncWorker`](https://github.com/nodejs/nan/blob/master/doc/asyncworker.md#api_nan_async_worker) and adds additional progress reporting callbacks that can be used during the asynchronous work execution to provide progress data back to JavaScript.

`AsyncFactoryWorker` behaves exactly the same as [`Nan::AsyncProgressQueueWorker`](https://github.com/nodejs/nan/blob/master/doc/asyncworker.md#api_nan_async_progress_queue_worker), except `AsyncFactoryWorker` avoids the copy.  Data is constructed once and delivered to the receiving thread.  Just as [`Nan::AsyncProgressQueueWorker`](https://github.com/nodejs/nan/blob/master/doc/asyncworker.md#api_nan_async_progress_queue_worker), all events are queued and delivered to the main thread.

Definition:

```c++
template<class T>
class AsyncFactoryWorker<T> : public AsyncWorker {
 public:
  explicit AsyncFactoryWorker(Callback *callback_, const char* resource_name = "nan:AsyncFactoryWorker");

  virtual ~AsyncFactoryWorker();

  void WorkProgress();

  class ExecutionProgress {
   public:
    void Send(const T* data, size_t count) const;
  };

  virtual void Execute(const ExecutionProgress& progress) = 0;

  virtual void HandleProgressCallback(const T *data, size_t count) = 0;

  virtual void Destroy();
};
```

This works just like the [`Nan::AsyncProgressQueueWorker`](https://github.com/nodejs/nan/blob/master/doc/asyncworker.md#api_nan_async_progress_queue_worker), but instead of `std::copy`ing the data, the `progress.Construct` function passes along the object constructor arguments, constructing and delivering the object to the main thread.

It allows us to asyncronously construct C++ objects even if they lack a public copy constructor, although it also does support copy constructors.

Sure, the constructor arguments end up getting copied, but not the entire object, which only gets constructed once.  Depending on the code being built around this worker, this could make things much more efficient.

Since `AsyncFactoryWorker` uses a variadic template, one can use it in many different ways.  Pass no constructor arguments to use the default constructor.  One can even use `&&T` as the constructor arguments and the worker will use the `move` constructor!!

<a name="tests"></a>

### Tests

To run the async-factory-worker tests do:

``` sh
npm install
npm run-script rebuild-tests
npm test
```

Or just:

``` sh
npm install
make test
```

## Licence &amp; copyright

Copyright (c) 2018 Michael Ira Krufky

async-factory-worker is licensed under an MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.
