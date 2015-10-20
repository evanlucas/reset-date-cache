'use strict'

const bindings = require('bindings')('resetdatecache')

module.exports = function() {
  bindings.reset()
}
