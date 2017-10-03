/*********************************************************************
 * async-factory-worker for Node.js
 *
 * Copyright (c) 2018 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-async-factory-worker/blob/master/LICENSE.md>
 ********************************************************************/

#include <async_factory_worker.h>

using namespace Nan;  // NOLINT(build/namespaces)
using namespace Krufky;  // NOLINT(build/namespaces)

class FactoryWorker : public AsyncFactoryWorker<int, int> {
 public:
  FactoryWorker(
      Callback *callback
    , Callback *progress
    , int iters)
    : AsyncFactoryWorker(callback), progress(progress)
    , iters(iters) {}

  ~FactoryWorker() {
    delete progress;
  }

  void Execute (const AsyncFactoryWorker::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Construct(i);
    }
  }

  void HandleProgressCallback(const int *data, size_t count) {
    HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        New<v8::Integer>(*reinterpret_cast<int*>(const_cast<int*>(data)))
    };
    progress->Call(1, argv, async_resource);
  }

 private:
  Callback *progress;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
  Callback *callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  AsyncQueueWorker(new FactoryWorker(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("doProgress").ToLocalChecked()
    , New<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncfactoryworker, Init)
