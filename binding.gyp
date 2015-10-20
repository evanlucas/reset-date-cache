{
  "targets": [{
    "target_name": "resetdatecache",
    "include_dirs": [
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/binding.cc"
    ]
  }]
}
