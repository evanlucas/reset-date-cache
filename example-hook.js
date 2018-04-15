var resetCache = require('reset-date-cache');

resetCache.registerHook(() => console.log(new Date().toString()));
console.log(new Date().toString());

//The timezone notifier is not referenced, set a dummy interval to keep running
setInterval(()=>{},1000); 