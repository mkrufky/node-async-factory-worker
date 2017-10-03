{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
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
        "target_name" : "asyncfactoryworkerstream"
      , "sources"     : [ "cpp/asyncfactoryworkerstream.cpp" ]
    }
]}
