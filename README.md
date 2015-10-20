# reset-date-cache

[![Build Status](https://travis-ci.org/evanlucas/reset-date-cache.svg)](https://travis-ci.org/evanlucas/reset-date-cache)
[![Coverage Status](https://coveralls.io/repos/evanlucas/reset-date-cache/badge.svg?branch=master&service=github)](https://coveralls.io/github/evanlucas/reset-date-cache?branch=master)

Reset V8's date cache on demand.

The underlying call being made is quite expensive so it should only be used
where absolutely necessary.

## Install

```bash
$ npm install reset-date-cache
```

## Test

```bash
$ npm test
```

## Example

```js
const reset = require('reset-date-cache')
const assert = require('assert')

var d = new Date('10/20/2015 8:50:00 AM UTC')
var orig = d.toString()

process.env.TZ = 'US/Hawaii'
reset()

var d2 = new Date('10/20/2015 8:50:00 AM UTC')
assert.equal(d2.toString(), 'Mon Oct 19 2015 22:50:00 GMT-1000 (HST)')

delete process.env.TZ

reset()

var d3 = new Date('10/20/2015 8:50:00 AM UTC')
assert.equal(d3.toString(), orig)

reset()
```


## Author

Evan Lucas

## License

MIT (See `LICENSE` for more info)
