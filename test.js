'use strict'

delete process.env.TZ

const test = require('tap').test
const reset = require('./')

test('should be able to reset the timezone', function(t) {
  var d = new Date('10/20/2015 8:50:00 AM UTC')
  var orig = d.toString()

  process.env.TZ = 'US/Hawaii'
  reset()

  var d2 = new Date('10/20/2015 8:50:00 AM UTC')
  t.equal(d2.toString(), 'Mon Oct 19 2015 22:50:00 GMT-1000 (Hawaii-Aleutian Standard Time)')

  delete process.env.TZ

  reset()

  var d3 = new Date('10/20/2015 8:50:00 AM UTC')
  t.equal(d3.toString(), orig)

  reset()
  t.end()
})
