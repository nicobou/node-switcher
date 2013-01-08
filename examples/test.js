/*
 * Copyright (C) 2012-2013 Nicolas Bouillot (http://www.nicolasbouillot.net)
 *
 * This file is part of node-switcher.
 *
 * node-switcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * node-switcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with node-switcher.  If not, see <http://www.gnu.org/licenses/>.
 */

var switcher_addon = require('../build/Release/switcher_addon');

//please quit switcher properly
process.on('exit', function () {
    switcher_addon.close();
    console.log('About to exit.');
});
process.on('SIGINT', function () {
    switcher_addon.close();
    console.log('Got SIGINT.  About to exit.');
    process.exit(0);
});


//creation without a name
console.log('unamed create returned:', 
	     switcher_addon.create("audiotestsrc") );
//creation with a name
console.log('named create returned:', 
	     switcher_addon.create("audiotestsrc","myaudiotest"));

console.log('get myaudiotest property returned:', 
	    switcher_addon.get("myaudiotest", "audiotestsrc/freq"));

console.log('set myaudiotest property to 111 returned:', 
	    switcher_addon.set("myaudiotest", "audiotestsrc/freq", "111"));

console.log('get myaudiotest property returned:', 
	    switcher_addon.get("myaudiotest", "audiotestsrc/freq"));

//remove "myaudiotest
console.log('remove myaudiotest returned:', 
	    switcher_addon.remove("myaudiotest"));


