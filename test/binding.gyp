{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
        "defines": [ "V8_DEPRECATION_WARNINGS=1" ],
        "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "<!(node -e \"require('..')\")"
        ]
    },
  "targets": [
    {
        "target_name" : "asyncfactoryworker"
      , "sources"     : [ "cpp/asyncfactoryworker.cpp" ]
    }
  , {
        "target_name" : "asyncfactoryworkervarargs"
      , "sources"     : [ "cpp/asyncfactoryworkervarargs.cpp" ]
    }
  , {
        "target_name" : "asyncfactoryworkerstream"
      , "sources"     : [ "cpp/asyncfactoryworkerstream.cpp" ]
    }
]}
