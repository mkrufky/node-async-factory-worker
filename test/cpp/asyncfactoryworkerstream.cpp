/*********************************************************************
 * async-factory-worker for Node.js
 *
 * Copyright (c) 2018 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-async-factory-worker/blob/master/LICENSE.md>
 ********************************************************************/

#include <async_factory_worker.h>

using namespace Nan;  // NOLINT(build/namespaces)
using namespace Mkrufky;  // NOLINT(build/namespaces)

// Custom data type: This serves as an example of how external
// libraries could be hooked in, populate their objects and send them to JS.
struct data_t {
  int index;
  int data;
};

template<typename T>
class FactoryWorker : public AsyncFactoryWorker<T, T> {
 public:
  FactoryWorker(
      Callback *callback
    , Callback *progress
    , int iters)
    : AsyncFactoryWorker<T, T>(callback), progress(progress)
    , iters(iters) {}

  ~FactoryWorker() {
    delete progress;
  }

  void Execute (
    const typename AsyncFactoryWorker<T, T>::ExecutionProgress& progress) {
    T data;
    for (int i = 0; i < iters; ++i) {
      data.index = i;
      data.data = i * 2;
      progress.Construct(data);
    }
  }

  void HandleProgressCallback(const T *data, size_t count) {
    HandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Nan::Set(
      obj,
      Nan::New("index").ToLocalChecked(),
      New<v8::Integer>(data->index));
    Nan::Set(
      obj,
      Nan::New("data").ToLocalChecked(),
      New<v8::Integer>(data->data));

    v8::Local<v8::Value> argv[] = { obj };
    progress->Call(1, argv, this->async_resource);
  }

 private:
  Callback *progress;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
  Callback *callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  AsyncQueueWorker(new FactoryWorker<data_t>(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("doProgress").ToLocalChecked()
    , New<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncfactoryworkerstream, Init)
