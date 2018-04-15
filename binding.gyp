{
  "targets": [{
    "target_name": "resetdatecache",
    "include_dirs": [
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/monitor.cc",
      "src/binding.cc"
    ]
  }]
}
