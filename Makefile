TOPLEVEL ?= $(dir $(lastword $(MAKEFILE_LIST)))
CPPLINT ?= $(TOPLEVEL)/cpplint.py
PYTHON ?= python
BUILDTYPE ?= Release

MODULES = asyncfactoryworker \
	asyncfactoryworkerstream \
	asyncfactoryworkervarargs

SOURCES = $(MODULES:%=test/cpp/%.cpp)
ADDONS = $(MODULES:%=test/build/$(BUILDTYPE)/%.node)

LINT_SOURCES = \
	test/cpp/asyncfactoryworker.cpp \
	test/cpp/asyncfactoryworkerstream.cpp \
	async_factory_worker.h

FILTER = -whitespace/parens

.PHONY: lint

lint:
	cd $(TOPLEVEL) && $(PYTHON) $(CPPLINT) --filter=$(FILTER) $(LINT_SOURCES)

test: $(ADDONS)
	npm test

forcetest:
	cd test && ../node_modules/.bin/node-gyp rebuild && cd ..
	npm test

$(ADDONS): async_factory_worker.h test/binding.gyp $(SOURCES)
	cd test/ && ../node_modules/.bin/node-gyp rebuild
