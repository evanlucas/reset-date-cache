'use strict'

var bindings = require('bindings')('resetdatecache');

module.exports = bindings.reset

module.exports.registerHook = bindings.setHook;