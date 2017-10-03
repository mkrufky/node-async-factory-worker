/*********************************************************************
 * async-factory-worker for Node.js
 *
 * Copyright (c) 2018 Michael Ira Krufky
 *
 * MIT License <https://github.com/mkrufky/node-async-factory-worker/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncfactoryworker' });

test('asyncfactoryworker', function (t) {
  // test with no sleep
  var worker = bindings.doProgress
    , progressed = 0
  worker(5, function(i) {
    t.ok(i === progressed, 'got the progress updates #' + i);
    progressed++;
  }, function () {
    t.ok(progressed === 5, 'got all progress updates')
    t.end()
  })
})
